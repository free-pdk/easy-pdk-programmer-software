/*
Copyright (C) 2019-2021  freepdk  https://free-pdk.github.io

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fpdkusb.h"
#include "fpdk.h"
#include "fpdkuart.h"
#include "main.h"
#include "usbd_cdc_if.h"

#include <string.h>
#include <inttypes.h>

static const char FPDKVER[] = "FREE-PDK EASY PROG - HW:" __FPDKHW__ " SW:" __FPDKSW__ " PROTO:" __FPDKPROTO__ " " __FPDKSUB__ "\n";
static const char FPDKMSG[] = "HWVAR:%" PRIx32 " HWMOD:%" PRIx32 "\n";

static const uint32_t FPDK_LED_UART_RX = 1;
static const uint32_t FPDK_LED_UART_TX = 2;
static const uint32_t FPDK_LED_IC      = 3;

static uint8_t  _packetbuf[256+2];
static uint32_t _packetbufpos = 0;

static uint16_t _ic_rw_buffer[0x1000];

static bool _ic_is_running;

static const uint32_t _dbg_led_on_time = 50;
static volatile uint32_t _dbg_led_rx_off_tick = 0;
static volatile uint32_t _dbg_led_tx_off_tick = 0;

//DAC DMA
#define DAC_BUFFER_WORDS 124
static uint32_t          _dac_buffer[DAC_BUFFER_WORDS];
static bool              _dac_is_running;
static uint32_t          _dac_buffer_next;
static volatile bool     _dac_buffer0_full;
static volatile bool     _dac_buffer1_full;

extern DAC_HandleTypeDef  hdac;
extern TIM_HandleTypeDef  htim3;

static void DAC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma) {
//  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//  HAL_DAC_ConvHalfCpltCallbackCh1(hdac);
  _dac_buffer0_full = false;
}

static void DAC_DMAConvCplt(DMA_HandleTypeDef *hdma) {
//  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//  HAL_DAC_ConvCpltCallbackCh1(hdac); 
//  hdac->State= HAL_DAC_STATE_READY;
  _dac_buffer1_full = false;
}

static void DAC_DMAError(DMA_HandleTypeDef *hdma) {
  DAC_HandleTypeDef* hdac = ( DAC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  hdac->ErrorCode |= HAL_DAC_ERROR_DMA;
  //handle error here
  hdac->State= HAL_DAC_STATE_READY;
}

void FPDKUSB_Init(void)
{
  _packetbufpos = 0;
}

void FPDKUSB_DeInit(void)
{
}

void FPDKUSB_USBSignalPortOpenClose(void)
{
  _packetbufpos = 0;
  memset(_ic_rw_buffer, 0xFF, sizeof(_ic_rw_buffer));

  if( _ic_is_running )
  {
    FPDKUART_DeInit();
    FPDK_SetVDD(0,0);
    FPDK_SetLed(FPDK_LED_IC,false);
    _ic_is_running = false;
  }

  if( _dac_is_running )
  {
    FPDK_SetLeds(0);

    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
    HAL_TIM_Base_Stop(&htim3);

    HAL_DAC_DeInit(&hdac);
    HAL_DAC_Init(&hdac);
    DAC_ChannelConfTypeDef sConfig = {0};
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
    HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);
    FPDK_SetVDD(0,0);
    _dac_is_running = false;
  }
}

bool FPDKUSB_IsConnected(void)
{
  return CDC_IsHostPortOpen();
}

bool FPDKUSB_USBHandleReceive(const uint8_t* dat, const uint32_t len)
{
  uint32_t cpylen;
  if( _packetbufpos+len < sizeof(_packetbuf) )
    cpylen = len;
  else
    cpylen = sizeof(_packetbuf)-_packetbufpos;

  memcpy( &_packetbuf[_packetbufpos], dat, cpylen );
  _packetbufpos += cpylen;

  return true;
}

static void _FPDKUSB_TransmitBuffer(const uint8_t* dat, const uint16_t len)
{
  uint32_t tickstimeout = HAL_GetTick()+500;
  while( USBD_BUSY == CDC_Transmit_FS( (uint8_t*)dat, len ) ) 
  {
    if( HAL_GetTick()>tickstimeout )
      break;
  }
}

void _FPDKUSB_SendResponse(const FPDKPROTO_RSP rtype, const uint8_t* dat, const uint32_t len )
{
  uint8_t tmp[] = { rtype, len&0xFF, (len>>8)&0xFF };
  _FPDKUSB_TransmitBuffer(tmp, sizeof(tmp));
  if( len )
    _FPDKUSB_TransmitBuffer(dat, len);
}

void _FPDKUSB_SendError(const uint8_t* dat, const uint32_t len)
{
  _FPDKUSB_SendResponse( FPDKPROTO_RSP_ERROR, dat, len );
}

void _FPDKUSB_Ack(const uint8_t* dat, const uint32_t len)
{
  _FPDKUSB_SendResponse( FPDKPROTO_RSP_ACK, dat, len );
}

void FPDKUSB_SendDebug(const uint8_t* dat, const uint32_t len)
{
  FPDK_SetLed(FPDK_LED_UART_RX, true);
  _dbg_led_rx_off_tick = HAL_GetTick() + _dbg_led_on_time;
  _FPDKUSB_SendResponse( FPDKPROTO_RSP_DBGDAT, dat, len );
}

bool _FPDKUSB_HandleCmd(const FPDKPROTO_CMD cmd, const uint8_t* dat, const uint32_t len)
{
  switch( cmd )
  {
    case FPDKPROTO_CMD_GETVERINFO:
      _FPDKUSB_Ack( (const uint8_t*)FPDKVER, strlen(FPDKVER) );
      break;

    case FPDKPROTO_CMD_GETVERMSG:
      {
        char msg[128];
        snprintf(msg, sizeof(msg), FPDKMSG, FPDK_GetHwVariant(), FPDK_GetHwMod() );
        _FPDKUSB_Ack( (const uint8_t*)msg, strlen(msg) );
      }
      break;

    case FPDKPROTO_CMD_SETLED:
      {
        if( len<1 )
          return false;
        FPDK_SetLeds(dat[0]);
        _FPDKUSB_Ack(0, 0);
      }
      break;

    case FPDKPROTO_CMD_GETBUTTON:
      {
        uint8_t tmp = FPDK_IsButtonPressed();
        _FPDKUSB_Ack(&tmp, sizeof(tmp));
      }
      break;

    case  FPDKPROTO_CMD_SETVOLTOUT:
      {
        if( len<(2*sizeof(uint32_t)) )
          return false;
        uint32_t tmp;
        memcpy( &tmp, &dat[0], sizeof(uint32_t) );
        FPDK_SetVDD(tmp,0);
        memcpy( &tmp, &dat[4], sizeof(uint32_t) );
        FPDK_SetVPP(tmp,0);
        _FPDKUSB_Ack(0, 0);
        FPDK_SetLeds(0xF); //set all LEDs to on
      }
      break;

    case FPDKPROTO_CMD_GETVOLTAGES:
      {
        uint32_t tmp[] = { FPDK_GetAdcVref(), FPDK_GetAdcVdd(), FPDK_GetAdcVpp() };
        _FPDKUSB_Ack((uint8_t*)tmp, 3*sizeof(uint32_t));
      }
      break;

    case FPDKPROTO_CMD_SETBUF:
      {
        if( len<sizeof(uint16_t) )
          return false;
        uint16_t data_offs;
        memcpy( &data_offs, &dat[0], sizeof(uint16_t) );

        if( (data_offs>(sizeof(_ic_rw_buffer)*sizeof(uint16_t))) || ((data_offs+len-sizeof(uint16_t))>(sizeof(_ic_rw_buffer)*sizeof(uint16_t))) )
          return false;

        memcpy( ((uint8_t*)_ic_rw_buffer) + data_offs, &dat[2], len-sizeof(uint16_t) );
        _FPDKUSB_Ack(0, 0);
      }
      break;

    case FPDKPROTO_CMD_GETBUF:
      {
        if( len<(2*sizeof(uint16_t)) )
          return false;
        uint16_t data_offs;
        memcpy( &data_offs, &dat[0], sizeof(uint16_t) );
        uint16_t outlen;
        memcpy( &outlen, &dat[2], sizeof(uint16_t) );

        if( (data_offs>(sizeof(_ic_rw_buffer)*sizeof(uint16_t))) || ((data_offs+outlen)>(sizeof(_ic_rw_buffer)*sizeof(uint16_t))) )
          return false;

        _FPDKUSB_Ack( ((uint8_t*)_ic_rw_buffer) + data_offs, outlen);
      }
      break;

    case FPDKPROTO_CMD_PROBEIC:
      {
        FPDK_SetLed(FPDK_LED_IC,true);
        FPDKICTYPE type;
        uint32_t vpp, vdd;
        uint32_t ic_id = FPDK_ProbeIC(&type, &vpp, &vdd);
        FPDK_SetLed(FPDK_LED_IC,false);
        uint32_t r[] = {ic_id & 0x7FFFFFFF,vpp,vdd,type};
        _FPDKUSB_Ack((uint8_t*)&r, sizeof(r));
      }
      break;

    case FPDKPROTO_CMD_BLANKCKIC:
      {
        if( len<(2*sizeof(uint32_t)+4*sizeof(uint16_t)+7*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint8_t cmd = dat[3];
        uint8_t cmd_trailing_clocks = dat[4];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[5], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[9], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[13], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[17], sizeof(uint32_t) );
        uint8_t addr_bits = dat[21];
        uint8_t data_bits = dat[22];
        uint8_t ecc_bits = dat[23];
        uint16_t count;
        memcpy( &count, &dat[24], sizeof(uint16_t) );
        uint8_t addr_exclude_first_instr = dat[26];
        uint16_t addr_exclude_start;
        memcpy( &addr_exclude_start, &dat[27], sizeof(uint16_t) );
        uint16_t addr_exclude_end;
        memcpy( &addr_exclude_end, &dat[29], sizeof(uint16_t) );
 
        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_BlankCheckIC(ic_id, type, cmd, cmd_trailing_clocks,
                                  vpp_cmd, vdd_cmd, vpp_read, vdd_read, 
                                  addr_bits, data_bits, ecc_bits, count, 
                                  addr_exclude_first_instr, addr_exclude_start, addr_exclude_end );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_ERASEIC:
      {
        if( len<(4*sizeof(uint8_t)+2*sizeof(uint16_t)+4*sizeof(uint32_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint8_t cmd = dat[3];
        uint8_t cmd_trailing_clocks = dat[4];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[5], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[9], sizeof(uint32_t) );
        uint32_t vdd_erase;
        memcpy( &vdd_erase, &dat[13], sizeof(uint32_t) );
        uint32_t vpp_erase;
        memcpy( &vpp_erase, &dat[17], sizeof(uint32_t) );
        uint8_t erase_clocks = dat[21];
        uint16_t erase_clock_hcycles;
        memcpy( &erase_clock_hcycles, &dat[22], sizeof(uint16_t) );
 
        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_EraseIC(ic_id, type, cmd, cmd_trailing_clocks,
                             vpp_cmd, vdd_cmd, vpp_erase, vdd_erase, 
                             erase_clocks, erase_clock_hcycles);
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_READIC:
      {
        if( len<(2*sizeof(uint32_t)+4*sizeof(uint16_t)+6*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint8_t cmd = dat[3];
        uint8_t cmd_trailing_clocks = dat[4];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[5], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[9], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[13], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[17], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[21], sizeof(uint16_t) );
        uint8_t addr_bits = dat[23];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[24], sizeof(uint16_t) );
        uint8_t data_bits = dat[26];
        uint8_t ecc_bits = dat[27];
        uint16_t count;
        memcpy( &count, &dat[28], sizeof(uint16_t) );
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_ReadIC(ic_id, type, cmd, cmd_trailing_clocks,
                            vpp_cmd, vdd_cmd, vpp_read, vdd_read,
                            addr, addr_bits, 
                            &_ic_rw_buffer[data_offs], 
                            data_bits, ecc_bits, count );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_WRITEIC:
      {
        if( len<(4*sizeof(uint32_t)+5*sizeof(uint16_t)+13*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint8_t cmd = dat[3];
        uint8_t cmd_trailing_clocks = dat[4];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[5], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[9], sizeof(uint32_t) );
        uint32_t vdd_write;
        memcpy( &vdd_write, &dat[13], sizeof(uint32_t) );
        uint32_t vpp_write;
        memcpy( &vpp_write, &dat[17], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[21], sizeof(uint16_t) );
        uint8_t addr_bits = dat[23];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[24], sizeof(uint16_t) );
        uint8_t data_bits = dat[26];
        uint8_t ecc_bits = dat[27];
        uint16_t count;
        memcpy( &count, &dat[28], sizeof(uint16_t) );
        uint8_t write_block_address_first = dat[30];
        uint8_t write_block_size = dat[31];
        uint8_t write_block_limited = dat[32];
        uint8_t write_block_clock_groups = dat[33];
        uint8_t write_block_clock_group_lead_clocks = dat[34];
        uint8_t write_block_clock_group_slow_clocks = dat[35];
        uint16_t write_block_clock_group_slow_clock_hcycle;
        memcpy( &write_block_clock_group_slow_clock_hcycle, &dat[36], sizeof(uint16_t) );
        uint8_t write_block_clock_group_trail_clocks = dat[38];
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_WriteIC(ic_id, type, cmd, cmd_trailing_clocks,
                             vpp_cmd, vdd_cmd, vpp_write, vdd_write,
                             addr, addr_bits,
                             &_ic_rw_buffer[data_offs],
                             data_bits, ecc_bits,
                             count,
                             write_block_address_first,
                             write_block_size,
                             write_block_limited,
                             write_block_clock_groups,
                             write_block_clock_group_lead_clocks,
                             write_block_clock_group_slow_clocks,
                             write_block_clock_group_slow_clock_hcycle,
                             write_block_clock_group_trail_clocks);
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_VERIFYIC:
      {
        if( len<(2*sizeof(uint32_t)+6*sizeof(uint16_t)+7*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint8_t cmd = dat[3];
        uint8_t cmd_trailing_clocks = dat[4];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[5], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[9], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[13], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[17], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[21], sizeof(uint16_t) );
        uint8_t addr_bits = dat[23];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[24], sizeof(uint16_t) );
        uint8_t data_bits = dat[26];
        uint8_t ecc_bits = dat[27];
        uint16_t count;
        memcpy( &count, &dat[28], sizeof(uint16_t) );
        uint8_t addr_exclude_first_instr = dat[30];
        uint16_t addr_exclude_start;
        memcpy( &addr_exclude_start, &dat[31], sizeof(uint16_t) );
        uint16_t addr_exclude_end;
        memcpy( &addr_exclude_end, &dat[33], sizeof(uint16_t) );
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_VerifyIC(ic_id, type, cmd, cmd_trailing_clocks,
                              vpp_cmd, vdd_cmd, vpp_read, vdd_read,
                              addr, addr_bits,
                              &_ic_rw_buffer[data_offs],
                              data_bits, ecc_bits, count,
                              addr_exclude_first_instr, addr_exclude_start, addr_exclude_end );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_CALIBRATEIC:
      {
        if( len<(4*sizeof(uint32_t)) )
          return false;
        uint32_t type;
        memcpy( &type, &dat[0], sizeof(uint32_t) );
        uint32_t vdd;
        memcpy( &vdd, &dat[4], sizeof(uint32_t) );
        uint32_t freq;
        memcpy( &freq, &dat[8], sizeof(uint32_t) );
        uint32_t mult;
        memcpy( &mult, &dat[12], sizeof(uint32_t) );

        FPDK_SetLed(FPDK_LED_IC,true);

        uint8_t fcalval;
        uint32_t fcalfreq;
        if( !FPDK_Calibrate(type, vdd, freq, mult, &fcalval, &fcalfreq) )
        {
          FPDK_SetLed(FPDK_LED_IC,false);
          return false;
        }

        FPDK_SetLed(FPDK_LED_IC,false);

        uint32_t r[] = {fcalval, fcalfreq};
        _FPDKUSB_Ack((uint8_t*)&r, sizeof(r));
      }
      break;

    case FPDKPROTO_CMD_EXECUTEIC:
      {
        if( len<sizeof(uint32_t) )
          return false;
        uint32_t vdd;
        memcpy( &vdd, &dat[0], sizeof(uint32_t) );

        FPDKUART_DeInit();

        if( !FPDK_SetVDD(vdd, 0) )
          return false;

        FPDKUART_Init();

        FPDK_SetLed(FPDK_LED_IC,true);
        _ic_is_running = true;

        _FPDKUSB_Ack(0, 0);
      }
      break;

    case FPDKPROTO_CMD_STOPIC:
      FPDKUART_DeInit();
      FPDK_SetVDD(0, 0);
      FPDK_SetLed(FPDK_LED_IC,false);
      FPDK_SetLed(FPDK_LED_UART_RX, false);
      FPDK_SetLed(FPDK_LED_UART_TX, false);
      _FPDKUSB_Ack(0, 0);
      break;

    case FPDKPROTO_CMD_DBGDAT:
      {
        FPDK_SetLed(FPDK_LED_UART_TX, true);
        _dbg_led_tx_off_tick = HAL_GetTick() + _dbg_led_on_time;
        FPDKUART_SendData(dat, len);
        //no ACK here, since we could receive debug data in this moment
      }
      break;

    case FPDKPROTO_CMD_DACOUT:
      {
        if( sizeof(uint16_t) != len )
          return false;
        uint16_t timerval;
        memcpy( &timerval, &dat[0], sizeof(uint16_t) );

        if( 0 != timerval ) {
          _dac_is_running = true;
          HAL_DAC_DeInit(&hdac);
          HAL_DAC_Init(&hdac);
          DAC_ChannelConfTypeDef sConfig = {0};
          sConfig.DAC_Trigger = DAC_TRIGGER_T3_TRGO;
          sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
          HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
          HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);

          //setup DUAL DAC DMA manually since HAL does not support it
          __HAL_LOCK(&hdac);
          hdac.State = HAL_DAC_STATE_BUSY;
          hdac.DMA_Handle2->XferCpltCallback = DAC_DMAConvCplt;
          hdac.DMA_Handle2->XferHalfCpltCallback = DAC_DMAHalfConvCplt;
          hdac.DMA_Handle2->XferErrorCallback = DAC_DMAError;
          SET_BIT(hdac.Instance->CR, DAC_CR_DMAEN2);
          __HAL_DAC_ENABLE_IT(&hdac, DAC_IT_DMAUDR2);
          HAL_DMA_Start_IT(hdac.DMA_Handle2, (uint32_t)_dac_buffer, (uint32_t)&hdac.Instance->DHR12LD, DAC_BUFFER_WORDS); //12bit LEFT aligned, DUAL, DAC_BUFFER_WORDS WORDS (WORD = 2ch*16bit)
          __HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_1);
          __HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_2);
          __HAL_UNLOCK(&hdac);

          TIM3->ARR = timerval;
          HAL_TIM_Base_Start(&htim3);

          FPDK_SetLeds(0xFF);
        }
        else{
          if( _dac_is_running )
          {
            FPDK_SetLeds(0);

            HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
            HAL_TIM_Base_Stop(&htim3);

            HAL_DAC_DeInit(&hdac);
            HAL_DAC_Init(&hdac);
            DAC_ChannelConfTypeDef sConfig = {0};
            sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
            HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
            HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2);
            _dac_is_running = false;
            FPDK_SetVDD(0,0);
          }
          _dac_buffer_next = 0;
          _dac_buffer0_full = false;
          _dac_buffer1_full = false;
        }

        _FPDKUSB_Ack(0, 0);
      }
      break;

    case FPDKPROTO_CMD_DACBUF:
      {
        if( ((DAC_BUFFER_WORDS/2*2)*sizeof(uint16_t)) != len )
          return false;
        
        //wait for free buffer (max 500msec, then error)
        uint32_t tickstimeout = HAL_GetTick()+500;
        if( 0 == _dac_buffer_next ) {
          while( _dac_buffer0_full ) {
            if( HAL_GetTick()>tickstimeout )
              return false;
          }
          memcpy( ((uint8_t*)&_dac_buffer[0]), dat, len );
          _dac_buffer0_full = true;
          _dac_buffer_next = 1;
        }
        else {
          while( _dac_buffer1_full ) {
            if( HAL_GetTick()>tickstimeout )
              return false;
          }
          memcpy( ((uint8_t*)&_dac_buffer[DAC_BUFFER_WORDS/2]), dat, len );
          _dac_buffer1_full = true;
          _dac_buffer_next = 0;
        }

        _FPDKUSB_Ack(0, 0);
      }
      break;

    default:
      return false;
  }
  return true;
}

void FPDKUSB_HandleCommands(void)
{
  if( _dbg_led_rx_off_tick && (HAL_GetTick()>_dbg_led_rx_off_tick) )
  {
    FPDK_SetLed(FPDK_LED_UART_RX, false);
    _dbg_led_rx_off_tick = 0;
  }

  if( _dbg_led_tx_off_tick && (HAL_GetTick()>_dbg_led_tx_off_tick) )
  {
    FPDK_SetLed(FPDK_LED_UART_TX, false);
    _dbg_led_tx_off_tick = 0;
  }

  if( _packetbufpos<2 )
    return;

  uint32_t cmd_length = _packetbuf[1];

  if( _packetbufpos < (2+cmd_length) )
    return;

  if( !_FPDKUSB_HandleCmd(_packetbuf[0], &_packetbuf[2], cmd_length) )
    _FPDKUSB_SendError(0, 0);

  __disable_irq();
  if( _packetbufpos )
  {
    uint32_t cpylen = _packetbufpos-(2+cmd_length);
    memmove( _packetbuf, &_packetbuf[2+cmd_length], cpylen );
    _packetbufpos = cpylen;
  }
  __enable_irq();
}
