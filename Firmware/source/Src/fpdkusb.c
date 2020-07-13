/*
Copyright (C) 2019-2020  freepdk  https://free-pdk.github.io

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

static const uint8_t FPDKVER[] = "FREE-PDK EASY PROG - HW:" __FPDKHW__ " SW:" __FPDKSW__ " PROTO:" __FPDKPROTO__ " " __FPDKSUB__ "\n";

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
      _FPDKUSB_Ack( FPDKVER, strlen((char*)FPDKVER) );
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
        if( len<(2*sizeof(uint32_t)+4*sizeof(uint16_t)+4*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[3], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[7], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[11], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[15], sizeof(uint32_t) );
        uint8_t addr_bits = dat[19];
        uint8_t data_bits = dat[20];
        uint16_t count;
        memcpy( &count, &dat[21], sizeof(uint16_t) );
        uint8_t addr_exclude_first_instr = dat[23];
        uint16_t addr_exclude_start;
        memcpy( &addr_exclude_start, &dat[24], sizeof(uint16_t) );
        uint16_t addr_exclude_end;
        memcpy( &addr_exclude_end, &dat[26], sizeof(uint16_t) );
 
        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_BlankCheckIC(ic_id, type, vpp_cmd, vdd_cmd, vpp_read, vdd_read, addr_bits, data_bits, count, addr_exclude_first_instr, addr_exclude_start, addr_exclude_end );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_ERASEIC:
      {
        if( len<(2*sizeof(uint8_t)+sizeof(uint16_t)+4*sizeof(uint32_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[3], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[7], sizeof(uint32_t) );
        uint32_t vdd_erase;
        memcpy( &vdd_erase, &dat[11], sizeof(uint32_t) );
        uint32_t vpp_erase;
        memcpy( &vpp_erase, &dat[15], sizeof(uint32_t) );
        uint8_t erase_clocks = dat[19];
 
        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_EraseIC(ic_id, type, vpp_cmd, vdd_cmd, vpp_erase, vdd_erase, erase_clocks);
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_READIC:
      {
        if( len<(2*sizeof(uint32_t)+4*sizeof(uint16_t)+3*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[3], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[7], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[11], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[15], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[19], sizeof(uint16_t) );
        uint8_t addr_bits = dat[21];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[22], sizeof(uint16_t) );
        uint8_t data_bits = dat[24];
        uint16_t count;
        memcpy( &count, &dat[25], sizeof(uint16_t) );
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_ReadIC(ic_id, type, vpp_cmd, vdd_cmd, vpp_read, vdd_read, addr, addr_bits, &_ic_rw_buffer[data_offs], data_bits, count );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_WRITEIC:
      {
        if( len<(4*sizeof(uint32_t)+4*sizeof(uint16_t)+6*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[3], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[7], sizeof(uint32_t) );
        uint32_t vdd_write;
        memcpy( &vdd_write, &dat[11], sizeof(uint32_t) );
        uint32_t vpp_write;
        memcpy( &vpp_write, &dat[15], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[19], sizeof(uint16_t) );
        uint8_t addr_bits = dat[21];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[22], sizeof(uint16_t) );
        uint8_t data_bits = dat[24];
        uint16_t count;
        memcpy( &count, &dat[25], sizeof(uint16_t) );
        uint8_t write_block_size = dat[27];
        uint8_t write_block_clock_groups = dat[28];
        uint8_t write_block_clocks_per_group = dat[29];
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_WriteIC(ic_id, type, vpp_cmd, vdd_cmd, vpp_write, vdd_write, addr, addr_bits, &_ic_rw_buffer[data_offs], data_bits, count, write_block_size, write_block_clock_groups, write_block_clocks_per_group );
        FPDK_SetLed(FPDK_LED_IC,false);

        _FPDKUSB_Ack((uint8_t*)&ic_id, sizeof(ic_id));
      }
      break;

    case FPDKPROTO_CMD_VERIFYIC:
      {
        if( len<(2*sizeof(uint32_t)+6*sizeof(uint16_t)+4*sizeof(uint8_t)) )
          return false;

        uint16_t ic_id;
        memcpy( &ic_id, &dat[0], sizeof(uint16_t) );
        uint8_t type = dat[2];
        uint32_t vdd_cmd;
        memcpy( &vdd_cmd, &dat[3], sizeof(uint32_t) );
        uint32_t vpp_cmd;
        memcpy( &vpp_cmd, &dat[7], sizeof(uint32_t) );
        uint32_t vdd_read;
        memcpy( &vdd_read, &dat[11], sizeof(uint32_t) );
        uint32_t vpp_read;
        memcpy( &vpp_read, &dat[15], sizeof(uint32_t) );
        uint16_t addr;
        memcpy( &addr, &dat[19], sizeof(uint16_t) );
        uint8_t addr_bits = dat[21];
        uint16_t data_offs;
        memcpy( &data_offs, &dat[22], sizeof(uint16_t) );
        uint8_t data_bits = dat[24];
        uint16_t count;
        memcpy( &count, &dat[25], sizeof(uint16_t) );
        uint8_t addr_exclude_first_instr = dat[27];
        uint16_t addr_exclude_start;
        memcpy( &addr_exclude_start, &dat[28], sizeof(uint16_t) );
        uint16_t addr_exclude_end;
        memcpy( &addr_exclude_end, &dat[30], sizeof(uint16_t) );
 
        if( (data_offs>sizeof(_ic_rw_buffer)) || ((data_offs+len)>sizeof(_ic_rw_buffer)) )
          return false;

        FPDK_SetLed(FPDK_LED_IC,true);
        ic_id = FPDK_VerifyIC(ic_id, type, vpp_cmd, vdd_cmd, vpp_read, vdd_read, addr, addr_bits, &_ic_rw_buffer[data_offs], data_bits, count, addr_exclude_first_instr, addr_exclude_start, addr_exclude_end );
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
      break;;

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
