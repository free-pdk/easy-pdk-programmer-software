/*
Copyright (C) 2019  freepdk  https://free-pdk.github.io

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

#include "fpdk.h"
#include "fpdkproto.h"

#include "main.h"
#include <string.h>
#include <stdlib.h>

//handles to peripherals (defined from cubemx in main.c)
extern ADC_HandleTypeDef  hadc;
extern DMA_HandleTypeDef  hdma_adc;
extern DAC_HandleTypeDef  hdac;
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim2;
extern TIM_HandleTypeDef  htim15;
extern SPI_HandleTypeDef  hspi1;
extern DMA_HandleTypeDef  hdma_spi1_tx;
extern DMA_HandleTypeDef  hdma_spi1_rx;
extern UART_HandleTypeDef huart1;

//board specific defines for programing IO
#define _FPDK_CLK_UP()       HAL_GPIO_WritePin( IC_IO_PA3_CLK_GPIO_Port, IC_IO_PA3_CLK_Pin, GPIO_PIN_SET )
#define _FPDK_CLK_DOWN()     HAL_GPIO_WritePin( IC_IO_PA3_CLK_GPIO_Port, IC_IO_PA3_CLK_Pin, GPIO_PIN_RESET )
#define _FPDK_SET_DAT_O(bit) HAL_GPIO_WritePin( IC_IO_PA4_GPIO_Port,     IC_IO_PA4_Pin,     bit?GPIO_PIN_SET:GPIO_PIN_RESET )
#define _FPDK_SET_DAT_F(bit) HAL_GPIO_WritePin( IC_IO_PA6_DAT_GPIO_Port, IC_IO_PA6_DAT_Pin, bit?GPIO_PIN_SET:GPIO_PIN_RESET )
#define _FPDK_GET_DAT()      HAL_GPIO_ReadPin(  IC_IO_PA6_DAT_GPIO_Port, IC_IO_PA6_DAT_Pin )

//general macros for programing IO
#define _FPDK_DelayUS(us)    { asm volatile ("MOV R0,%[loops]\n1:\nSUB R0,#1\nCMP R0,#0\nBNE 1b"::[loops]"r"(10*us):"memory"); }
#define _FPDK_Clock()        { _FPDK_CLK_UP(); _FPDK_CLK_DOWN(); }
#define _FPDK_SendBitO(bit)  { _FPDK_SET_DAT_O(bit); _FPDK_Clock(); }
#define _FPDK_SendBitF(bit)  { _FPDK_SET_DAT_F(bit); _FPDK_Clock(); }
#define _FPDK_RecvBit()      ({ _FPDK_CLK_UP(); uint32_t bit=_FPDK_GET_DAT(); _FPDK_CLK_DOWN(); bit; })

//board specific max values (DAC max => mV max after opamp output / -30 mV DAC DC offset)
#define FPDK_VDD_DAC_MAX_MV ( 6290 - 30)
#define FPDK_VPP_DAC_MAX_MV (13300 - 30)

//STM32F072 chip specific factory calibration values in rom
#define TEMP030_CAL ((uint32_t)*((uint16_t*)0x1FFFF7B8))
#define TEMP110_CAL ((uint32_t)*((uint16_t*)0x1FFFF7C2))
#define VREFINT_CAL ((uint32_t)*((uint16_t*)0x1FFFF7BA))

//PDK command timings
#define FPDK_VPP_CMD_STABELIZE_DELAYUS  100
#define FPDK_VDD_CMD_STABELIZE_DELAYUS  500
#define FPDK_VPP_EW_STABELIZE_DELAYUS   10000
#define FPDK_VDD_EW_STABELIZE_DELAYUS   10000
#define FPDK_VDD_STOP_DELAYUS           0 //250
#define FPDK_VPP_STOP_DELAYUS           0 //100
#define FPDK_LEAVEPROGMODE_DELAYUS      10000  //IMPORTANT: wait a bit after leaving program mode, before executing next command
#define FPDK_VDD_CAL_STARTUP_DELAYUS    1000


//current dac output values, we need to store them so we can set channels seperate
static uint32_t _dac_vdd;
static uint32_t _dac_vpp;

//DMA double buffer = 2 * (3*16 bit=>2*32bit) // each adc value always takes 16 bit
static uint32_t _adcDMABuffer[(2*(8*3)+3)/sizeof(uint16_t)];

//averaged ADC conversions converted to mV
static volatile uint32_t _adc_vref;
static volatile uint32_t _adc_vdd;
static volatile uint32_t _adc_vpp;

static void _FPDK_ADC_HandleData(const uint16_t* adcdata)
{
  uint32_t avref=1;
  uint32_t avdd=0;
  uint32_t avpp=0;

  for( uint32_t p=0; p<8; p++ )
  {
    avpp+= adcdata[p*3+0];
    avdd+= adcdata[p*3+1];
    avref+=adcdata[p*3+2];
  }

  _adc_vref = (3*_adc_vref + ((8 * VDD_VALUE * VREFINT_CAL)) / avref) / 4;                         //average vref also over last measurements
  _adc_vdd = (_adc_vref*avdd*6)>>15;                                                               //factor 6 by voltage divider resistors, >>15 = /4096 / 8
  _adc_vpp = (_adc_vref*avpp*6)>>15;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle) 
{
  _FPDK_ADC_HandleData((uint16_t*)_adcDMABuffer);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) 
{
  _FPDK_ADC_HandleData(((uint16_t*)_adcDMABuffer)+(8*3));
}

static void _FPDK_SetClkOutgoing(void)
{
  HAL_GPIO_WritePin( IC_IO_PA3_CLK_GPIO_Port, IC_IO_PA3_CLK_Pin, GPIO_PIN_RESET );
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA3_CLK_Pin, .Mode=GPIO_MODE_OUTPUT_PP, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA3_CLK_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SetClkIncoming(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA3_CLK_Pin, .Mode=GPIO_MODE_INPUT, .Pull=GPIO_PULLDOWN, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA3_CLK_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SetDatOutgoing(void)
{
  HAL_GPIO_WritePin( IC_IO_PA6_DAT_GPIO_Port, IC_IO_PA6_DAT_Pin, GPIO_PIN_RESET );
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA6_DAT_Pin, .Mode=GPIO_MODE_OUTPUT_PP, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA6_DAT_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SetDatIncoming(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA6_DAT_Pin, .Mode=GPIO_MODE_INPUT, .Pull=GPIO_PULLDOWN, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA6_DAT_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SetPA4Outgoing(void)
{
  HAL_GPIO_WritePin( IC_IO_PA4_GPIO_Port, IC_IO_PA4_Pin, GPIO_PIN_RESET );
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA4_Pin, .Mode=GPIO_MODE_OUTPUT_PP, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA4_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SetPA4Incoming(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { .Pin=IC_IO_PA4_Pin, .Mode=GPIO_MODE_INPUT, .Speed=GPIO_SPEED_FREQ_HIGH };
  HAL_GPIO_Init(IC_IO_PA4_GPIO_Port, &GPIO_InitStruct);
}

static void _FPDK_SendBits32O(const uint32_t data, const uint8_t bits)
{
  uint32_t bitdat = data<<(32-bits);
  for( uint32_t p=0; p<bits; p++ )
  {
    _FPDK_SendBitO( bitdat&0x80000000 );
    bitdat<<=1;
  }
  _FPDK_SET_DAT_O(0);
}

static void _FPDK_SendBits32F(const uint32_t data, const uint8_t bits)
{
  uint32_t bitdat = data<<(32-bits);
  for( uint32_t p=0; p<bits; p++ )
  {
    _FPDK_SendBitF( bitdat&0x80000000 );
    bitdat<<=1;
  }
}

static uint32_t _FPDK_RecvBits32(const uint8_t bits)
{
  uint32_t bitdat = 0;
  for( uint32_t p=0; p<bits; p++ )
    bitdat = (bitdat<<1) | (_FPDK_RecvBit()?1:0);
  return bitdat;
}

static int _FPDK_EnterProgramingmMode(const FPDKICTYPE type, const uint32_t VPP_mV, const uint32_t VDD_mV)
{
  _FPDK_SetClkOutgoing();

  if( !FPDK_SetVPP(VPP_mV, FPDK_VPP_CMD_STABELIZE_DELAYUS) )                                       //set VPP
  {
    FPDK_SetVPP(0,0);
    return -1;
  }

  if( !FPDK_SetVDD(VDD_mV, FPDK_VDD_CMD_STABELIZE_DELAYUS) )                                       //set VDD
  {
    FPDK_SetVPP(0,0);
    FPDK_SetVDD(0,0);
    return -2;
  }

  if( FPDK_IC_FLASH == type )
    _FPDK_SetDatOutgoing();                                                                        //set DAT outgoing
  else
  {
    _FPDK_SetDatIncoming();                                                                        //set DAT incoming
    _FPDK_SetPA4Outgoing();                                                                        //set PA4 outgoing
  }
  return 0;
}

static void _FPDK_LeaveProgramingMode(const FPDKICTYPE type, const uint32_t extrawaitus)
{
  _FPDK_SetDatIncoming();
  _FPDK_SetPA4Incoming();
  FPDK_SetVDD(0, FPDK_VDD_STOP_DELAYUS);                                                           //disable VDD
  FPDK_SetVPP(0, FPDK_VPP_STOP_DELAYUS);                                                           //disable VPP
  _FPDK_DelayUS(FPDK_LEAVEPROGMODE_DELAYUS);
  _FPDK_DelayUS(extrawaitus+1);
  _FPDK_SetClkIncoming();
}

static uint16_t _FPDK_SendCommand(const FPDKICTYPE type, const uint8_t command)
{
  uint32_t ack = 0;
  if( FPDK_IC_FLASH == type )
  {
    _FPDK_SendBits32F(0xA5A5A5A0 | command, 32);                                                   //preamble+command
    _FPDK_SetDatIncoming();                                                                        //set DAT incoming
    ack = _FPDK_RecvBits32(16);                                                                    //receive ack
    _FPDK_SetDatOutgoing();                                                                        //set DAT outgoing
    _FPDK_Clock();                                                                                 //1 extra clock
  }
  else
    _FPDK_SendBits32O(0xA5A5A5A0 | command, 32);                                                   //preamble+command
  return ack;                                                                                      //return ack (ic_id)
}

static uint32_t _FPDK_ReadAddr(const FPDKICTYPE type, const uint32_t addr, const uint8_t addr_bits, const uint8_t data_bits)
{
  uint32_t dat;
  if( FPDK_IC_FLASH == type )
  {
    _FPDK_SendBits32F(addr,addr_bits);                                                             //send address to read from 
    _FPDK_SetDatIncoming();                                                                        //set DAT incoming
    dat = _FPDK_RecvBits32(data_bits);                                                             //receive data
    _FPDK_SetDatOutgoing();                                                                        //set DAT outgoing
    _FPDK_Clock();                                                                                 //1 extra clock
  }
  else
  {
    _FPDK_SendBits32O(addr,addr_bits);                                                             //send address to read from 
    dat = _FPDK_RecvBits32(data_bits);                                                             //receive data
  }
  return dat;
}

static void _FPDK_WriteAddr(const FPDKICTYPE type, const uint32_t addr, const uint8_t addr_bits, const uint16_t* data, const uint8_t data_bits, 
                            const uint32_t count, const uint8_t write_block_clock_groups, const uint8_t write_block_clocks_per_group)
{
  if( FPDK_IC_FLASH == type )
  {
    for( uint32_t p=0; p<count; p++ )
      _FPDK_SendBits32F(data[p],data_bits);                                                        //write 1 word

    _FPDK_SendBits32F(addr,addr_bits);                                                             //send address to write to
    _FPDK_SetDatIncoming();                                                                        //set DAT incoming

    _FPDK_DelayUS(4);

    for( uint32_t l=0; l<write_block_clock_groups; l++ )
    {
      for( uint32_t w=0; w<write_block_clocks_per_group; w++ )
      {
        _FPDK_CLK_UP();
        _FPDK_DelayUS(15);
        _FPDK_CLK_DOWN();
        _FPDK_DelayUS(15);
      }

      _FPDK_Clock();                                                                               //1 extra clock
      _FPDK_DelayUS(4);
    }
    _FPDK_SetDatOutgoing();                                                                        //set DAT outgoing
    _FPDK_DelayUS(25);
  }
  else
  {
    for( uint32_t p=0; p<count; p++ )
      _FPDK_SendBits32O(data[p],data_bits);                                                        //write 1 word

    _FPDK_SendBits32O(addr,addr_bits);                                                             //send address to write to

    _FPDK_Clock();                                                                                 //1 extra clock 
    _FPDK_DelayUS(4);

    for( uint32_t l=0; l<write_block_clock_groups; l++ )
    {
      _FPDK_CLK_UP();
      for( uint32_t w=0; w<write_block_clocks_per_group; w++ )
      {
        _FPDK_SET_DAT_O(1);
        _FPDK_DelayUS(30);
        _FPDK_SET_DAT_O(0);
        _FPDK_DelayUS(30);
      }
      _FPDK_CLK_DOWN();
      _FPDK_DelayUS(4);

      _FPDK_Clock();                                                                               //1 extra clock
      _FPDK_DelayUS(4);
    }
    _FPDK_DelayUS(25);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FPDK_Init(void)
{
  HAL_SPI_DeInit(&hspi1);
  HAL_UART_DeInit(&huart1);

  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);                                                       //start PWM output to generate -4.8V (50kHz, 50% duty)

  HAL_DAC_Start( &hdac, DAC_CHANNEL_1 );                                                           //enable DAC output channel 1
  HAL_DAC_Start( &hdac, DAC_CHANNEL_2 );                                                           //enable DAC output channel 2

  _dac_vdd = 0;
  _dac_vpp = 0;
  HAL_DACEx_DualSetValue( &hdac, DAC_ALIGN_12B_R, _dac_vpp, _dac_vdd );                            //set 0 volt output for both channels

  HAL_GPIO_WritePin( DCDC15VOLT_ENABLE_OUT_GPIO_Port, DCDC15VOLT_ENABLE_OUT_Pin, GPIO_PIN_SET );   //enable DCDC 15V booster

  _adc_vref = 0;
  _adc_vdd = 0;
  _adc_vpp = 0;
  HAL_ADCEx_Calibration_Start(&hadc);                                                              //calibrate ADC
  HAL_ADC_Start_DMA(&hadc, (uint32_t*)_adcDMABuffer, 2*(8*3) );                                    //start ADC (double buffer DMA with completion callbacks)
  HAL_TIM_Base_Start(&htim1);                                                                      //start tim1 to trigger ADC conversions

  HAL_TIM_Base_Start(&htim2);                                                                      //start tim2 for frequency measurement

  _FPDK_SetClkIncoming();
  _FPDK_SetDatIncoming();
  _FPDK_SetPA4Incoming();
}

void FPDK_DeInit(void)
{
  HAL_DACEx_DualSetValue( &hdac, DAC_ALIGN_12B_R, 0, 0 );                                          //set 0 volt output for both channels
  HAL_GPIO_WritePin( DCDC15VOLT_ENABLE_OUT_GPIO_Port, DCDC15VOLT_ENABLE_OUT_Pin, GPIO_PIN_RESET ); //disable DCDC 15V booster
  HAL_ADC_Stop( &hadc );                                                                           //stop ADC
  _FPDK_SetClkIncoming();
  _FPDK_SetDatIncoming();
  _FPDK_SetPA4Incoming();
}

void FPDK_SetLeds(uint32_t val)
{
  HAL_GPIO_WritePin( LED1_OUT_GPIO_Port, LED1_OUT_Pin, (val&1)?GPIO_PIN_RESET:GPIO_PIN_SET );
  HAL_GPIO_WritePin( LED2_OUT_GPIO_Port, LED2_OUT_Pin, (val&2)?GPIO_PIN_RESET:GPIO_PIN_SET );
  HAL_GPIO_WritePin( LED3_OUT_GPIO_Port, LED3_OUT_Pin, (val&4)?GPIO_PIN_RESET:GPIO_PIN_SET );
}

void FPDK_SetLed(uint32_t led, bool enable)
{
  switch( led )
  {
    case 1: HAL_GPIO_WritePin( LED1_OUT_GPIO_Port, LED1_OUT_Pin, enable?GPIO_PIN_RESET:GPIO_PIN_SET );
      break;
    case 2: HAL_GPIO_WritePin( LED2_OUT_GPIO_Port, LED2_OUT_Pin, enable?GPIO_PIN_RESET:GPIO_PIN_SET );
      break;
    case 3: HAL_GPIO_WritePin( LED3_OUT_GPIO_Port, LED3_OUT_Pin, enable?GPIO_PIN_RESET:GPIO_PIN_SET );
      break;

    default:
      break;
  }
}

bool FPDK_IsButtonPressed(void)
{
  static uint32_t press_count = 0;
  static uint32_t tick_press_checked = 0;

  if( HAL_GetTick() > tick_press_checked )
  {
    tick_press_checked = HAL_GetTick()+1;
    if( HAL_GPIO_ReadPin( USER_BTN_IN_GPIO_Port, USER_BTN_IN_Pin ) )
      press_count++;
    else
      press_count = 0;
  }
  return(press_count>10);
}

uint32_t FPDK_GetAdcVref(void) { 
  return _adc_vref; 
}

uint32_t FPDK_GetAdcVdd(void) {
  return _adc_vdd;
}

uint32_t FPDK_GetAdcVpp(void) {
  return _adc_vpp;
}

bool FPDK_SetVDD(uint32_t mV, uint32_t stabelizeDelayUS)
{
  _dac_vdd = (mV*4095) / FPDK_VDD_DAC_MAX_MV;

  if( _dac_vdd>4095 )
    _dac_vdd = 4095;

  HAL_DACEx_DualSetValue( &hdac, DAC_ALIGN_12B_R, _dac_vpp, _dac_vdd );                            //set VDD

  if( stabelizeDelayUS )
    _FPDK_DelayUS(stabelizeDelayUS);

  return true;
}

bool FPDK_SetVPP(uint32_t mV, uint32_t stabelizeDelayUS)
{
  _dac_vpp = (mV*4095) / FPDK_VPP_DAC_MAX_MV;

  if( _dac_vpp>4095 )
    _dac_vpp = 4095;

  HAL_DACEx_DualSetValue( &hdac, DAC_ALIGN_12B_R, _dac_vpp, _dac_vdd );                            //set VPP

  if( stabelizeDelayUS )
    _FPDK_DelayUS(stabelizeDelayUS);

  return true;
}

static uint32_t _FPDK_GetIDIC(const FPDKICTYPE type, const uint32_t vpp_cmd, const uint32_t vdd_cmd, const uint8_t data_bits)
{
  uint32_t ic_id = 0;
  switch( type )
  {
    case FPDK_IC_FLASH:
      {
        if( _FPDK_EnterProgramingmMode(FPDK_IC_FLASH,vpp_cmd,vdd_cmd)<0 )
          return FPDK_ERR_VPPVDD;
  
        ic_id = _FPDK_SendCommand(FPDK_IC_FLASH, 0x6);                                             //use read command for probing
  
        _FPDK_LeaveProgramingMode(FPDK_IC_FLASH, 0);
      }
      break;

    case FPDK_IC_OTP1:
      {
        if( _FPDK_EnterProgramingmMode(FPDK_IC_OTP1,vpp_cmd,vdd_cmd)<0 )
          return FPDK_ERR_VPPVDD;

        _FPDK_SendCommand(FPDK_IC_OTP1, 0x7);                                                      //use write command for probing

        ic_id = _FPDK_RecvBits32(data_bits+data_bits+12);                                          //data#1, data#2, address => 12 bit response

        _FPDK_LeaveProgramingMode(FPDK_IC_OTP1, 0);                                                //abort write before it is executed
      }
      break;

    case FPDK_IC_OTP2:
      break;
  }
  return ic_id;
}

uint32_t FPDK_ProbeIC(FPDKICTYPE* type, uint32_t* vpp_cmd, uint32_t* vdd_cmd)
{
  uint32_t vpp_probe;
  uint32_t vdd_probe;
  uint32_t ic_id;

  //try flash first
  vpp_probe = 4500; vdd_probe = 2000;
  ic_id = _FPDK_GetIDIC(FPDK_IC_FLASH, vpp_probe, vdd_probe, 0);
  if( ic_id )
  {
    *type = FPDK_IC_FLASH;
    *vpp_cmd = vpp_probe;
    *vdd_cmd = vdd_probe;
    return ic_id;
  }

  //try OTP1
  vpp_probe = 4500; vdd_probe = 2000;
  ic_id = _FPDK_GetIDIC(FPDK_IC_OTP1, vpp_probe, vdd_probe, 16);                                   //try with 16 codebits (need to shift right for smaller devices)
  if( ic_id )
  {
    *type = FPDK_IC_OTP1;
    *vpp_cmd = vpp_probe;
    *vdd_cmd = vdd_probe;
    return ic_id;
  }

  //TODO: try OTP2

  return 0;
}

uint16_t FPDK_ReadIC(const uint16_t ic_id, const FPDKICTYPE type, const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                     const uint32_t addr, const uint8_t addr_bits, uint16_t* data, const uint8_t data_bits, const uint32_t count)
{
  if( (FPDK_IC_FLASH != type) && (ic_id != (_FPDK_GetIDIC( type, vpp_cmd, vdd_cmd, data_bits )&0xFFF)) )
    return FPDK_ERR_CMDRSP;

  if( _FPDK_EnterProgramingmMode(type,vpp_cmd,vdd_cmd) < 0 )                                       //enter programing mode using VPP and VDD
    return FPDK_ERR_VPPVDD;

  uint16_t resp = _FPDK_SendCommand(type,0x6);                                                     //send READ command
  if( (FPDK_IC_FLASH == type) && (ic_id != (resp&0xFFF)) )
  {
    _FPDK_LeaveProgramingMode(type, 0);
    return FPDK_ERR_CMDRSP;
  }

  for( uint32_t p=0; p<count; p++ )
    data[p] = _FPDK_ReadAddr( type, addr+p, addr_bits, data_bits );

  _FPDK_LeaveProgramingMode(type, 0);
  return ic_id;
}

uint16_t FPDK_VerifyIC(const uint16_t ic_id, const FPDKICTYPE type, const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                       const uint32_t addr, const uint8_t addr_bits, const uint16_t* data, const uint8_t data_bits, const uint32_t count,
                       const bool addr_exclude_first_instr, const uint32_t addr_exclude_start, const uint32_t addr_exclude_end)
{
  if( (FPDK_IC_FLASH != type) && (ic_id != (_FPDK_GetIDIC( type, vpp_cmd, vdd_cmd, data_bits )&0xFFF)) )
    return FPDK_ERR_CMDRSP;

  if( _FPDK_EnterProgramingmMode(type,vpp_cmd,vdd_cmd) < 0 )                                       //enter programing mode using VPP and VDD
    return FPDK_ERR_VPPVDD;

  uint16_t resp = _FPDK_SendCommand(type,0x6);                                                     //send READ command
  if( (FPDK_IC_FLASH == type) && (ic_id != (resp&0xFFF)) )
  {
    _FPDK_LeaveProgramingMode(type, 0);
    return FPDK_ERR_CMDRSP;
  }

  uint16_t ret = ic_id;

  uint32_t blank_value = (1<<data_bits)-1;

  for( uint32_t p=0; p<count; p++ )
  {
    if( addr_exclude_first_instr && (0 == addr+p) )
      continue;

    if( (p<addr_exclude_start) || (p>addr_exclude_end) )
    {
      uint32_t dat = _FPDK_ReadAddr( type, addr+p, addr_bits, data_bits );
      if( (data[p]&blank_value) != (dat&blank_value) )
      {
        //special exception if data was blank value this was a "joker" to skip write / ignore original value
        if( (data[p]&blank_value) != blank_value )
        {
          ret = FPDK_ERR_VERIFY;
          break;
        }
      }
    }
  }

  _FPDK_LeaveProgramingMode(type, 0);
  return ret;
}

uint16_t FPDK_BlankCheckIC(const uint16_t ic_id, const FPDKICTYPE type, const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                           const uint8_t addr_bits, const uint8_t data_bits, const uint32_t count, 
                           const bool addr_exclude_first_instr, const uint32_t addr_exclude_start, const uint32_t addr_exclude_end)
{
  if( (FPDK_IC_FLASH != type) && (ic_id != (_FPDK_GetIDIC( type, vpp_cmd, vdd_cmd, data_bits )&0xFFF)) )
    return FPDK_ERR_CMDRSP;

  if( _FPDK_EnterProgramingmMode(type,vpp_cmd,vdd_cmd) < 0 )                                       //enter programing mode using VPP and VDD
    return FPDK_ERR_VPPVDD;

  uint16_t resp = _FPDK_SendCommand(type,0x6);                                                     //send READ command
  if( (FPDK_IC_FLASH == type) && (ic_id != (resp&0xFFF)) )
  {
    _FPDK_LeaveProgramingMode(type, 0);
    return FPDK_ERR_CMDRSP;
  }

  uint32_t blank_value = (1<<data_bits)-1;

  uint16_t ret = ic_id;
  
  for( uint32_t p=0; p<count; p++ )
  {
    if( addr_exclude_first_instr && (0 == p) )
      continue;

    if( (p<addr_exclude_start) || (p>addr_exclude_end) )
    {
      uint32_t dat = _FPDK_ReadAddr( type, p, addr_bits, data_bits );
      if( blank_value != dat )
      {
        ret = FPDK_ERR_NOTBLANK;
        break;
      }
    }
  }

  _FPDK_LeaveProgramingMode(type, 0);
  return ret;
}

uint16_t FPDK_EraseIC(const uint16_t ic_id, const FPDKICTYPE type, 
                      const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                      const uint32_t vpp_erase, const uint32_t vdd_erase,
                      const uint8_t erase_clocks)
{
  if( _FPDK_EnterProgramingmMode(type,vpp_cmd,vdd_cmd) < 0 )                                       //enter programing mode using VPP and VDD
    return FPDK_ERR_VPPVDD;

  if( FPDK_IC_FLASH != type )
    return FPDK_ERR_UKNOWN;

  if( ic_id != (_FPDK_SendCommand(type,0x3)&0xFFF) )                                               //send ERASE command
  {
    _FPDK_LeaveProgramingMode(type, 0);
    return FPDK_ERR_CMDRSP;
  }

  if( !FPDK_SetVPP(vpp_erase, FPDK_VPP_EW_STABELIZE_DELAYUS) ||                                    //set erase VPP and VDD
      !FPDK_SetVDD(vdd_erase, FPDK_VDD_EW_STABELIZE_DELAYUS)   )
    return FPDK_ERR_HVPPHVDD;

  for( uint32_t e=0; e<erase_clocks; e++ )
  {
    _FPDK_CLK_UP();
    _FPDK_DelayUS(5000);
    _FPDK_CLK_DOWN();
    _FPDK_DelayUS(1);
    _FPDK_CLK_UP();
    _FPDK_DelayUS(1);
    _FPDK_CLK_DOWN();
    _FPDK_DelayUS(4);
  }

  _FPDK_Clock();                                                                                   //1 extra clock
  _FPDK_LeaveProgramingMode(type, 100000);

  return ic_id;
}

uint16_t FPDK_WriteIC(const uint16_t ic_id, const FPDKICTYPE type, 
                      const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                      const uint32_t vpp_write, const uint32_t vdd_write,
                      const uint32_t addr, const uint8_t addr_bits, 
                      const uint16_t* data, const uint8_t data_bits, 
                      const uint32_t count, 
                      const uint8_t write_block_size, const uint8_t write_block_clock_groups, const uint8_t write_block_clocks_per_group)
{
  if( !write_block_size || (write_block_size>8) )
    return FPDK_ERR_UKNOWN;

  if( (FPDK_IC_FLASH != type) && (ic_id != (_FPDK_GetIDIC( type, vpp_cmd, vdd_cmd, data_bits )&0xFFF)) )
    return FPDK_ERR_CMDRSP;

  if( _FPDK_EnterProgramingmMode(type,vpp_cmd,vdd_cmd) < 0 )                                       //enter programing mode using VPP and VDD
    return FPDK_ERR_VPPVDD;

  uint16_t resp = _FPDK_SendCommand(type,0x7);                                                     //send WRITE command
  if( (FPDK_IC_FLASH == type) && (ic_id != (resp&0xFFF)) )
  {
    _FPDK_LeaveProgramingMode(type, 0);
    return FPDK_ERR_CMDRSP;
  }

  if( !FPDK_SetVPP(vpp_write, FPDK_VPP_EW_STABELIZE_DELAYUS) ||                                    //set write VPP and VDD
      !FPDK_SetVDD(vdd_write, FPDK_VDD_EW_STABELIZE_DELAYUS)   )
    return FPDK_ERR_HVPPHVDD;

  uint32_t blank_value = (1<<data_bits)-1;

  for( uint32_t p=0; p<count; p+=write_block_size )
  {
    uint16_t write_buf[8];
    memset( write_buf, 0xFF, sizeof(write_buf) );                                                  //initialize empty write buffer (all bits '1')

    uint32_t write_count = (count>(p+write_block_size-1))?write_block_size:(count-p);
    memcpy( &write_buf[addr % write_block_size], &data[p], write_count*sizeof(uint16_t) );         //place data to write in write buffer (aligned to block size)

    bool block_is_empty = true;
    for( uint32_t c=0; c<write_block_size; c++ )                                                   //check of complete block is empty (all bits '1')
    {
      if( blank_value != (write_buf[c] & blank_value) )
      {
        block_is_empty = false;
        break;
      }
    }

    if( !block_is_empty )                                                                          //skip empty blocks
    {
      uint32_t write_addr_aligned = ((addr+p) / write_block_size) * write_block_size;
      _FPDK_WriteAddr(type, 
                      write_addr_aligned, addr_bits, write_buf, data_bits, 
                      write_block_size, write_block_clock_groups, write_block_clocks_per_group);
    }
  }

  _FPDK_LeaveProgramingMode(type, 100000);

  return ic_id;
}

////////////////////////////

#define SPI_BLOCK_SIZE 16
#define SPI_BLOCKS_MEASURE 10
#define SPI_BLOCKS_IGNORE  1
static uint8_t _spiDMARxBuffer[SPI_BLOCK_SIZE];
static uint8_t _spiDMATxBuffer[SPI_BLOCK_SIZE];

static volatile bool     _spiSendPulseStartMeasure;
static volatile int32_t  _spiFreqMeasureCounter;
static volatile uint32_t _spiFrequency;

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
  if( _spiSendPulseStartMeasure )
  {
    _spiSendPulseStartMeasure = false;
    _spiFreqMeasureCounter = -SPI_BLOCKS_IGNORE;
    _spiDMATxBuffer[0] = 1; //set single one bit
  }
  else
    _spiDMATxBuffer[0] = 0; //reset single one bit
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if( !_spiFreqMeasureCounter )
    __HAL_TIM_SET_COUNTER(&htim2,0);

  if( _spiFreqMeasureCounter <= SPI_BLOCKS_MEASURE )
    _spiFreqMeasureCounter++;

  if( SPI_BLOCKS_MEASURE == _spiFreqMeasureCounter )
  {
    uint32_t c = __HAL_TIM_GET_COUNTER(&htim2);
    _spiFrequency = (8*SPI_BLOCK_SIZE*(SPI_BLOCKS_MEASURE-1)*48000000ULL)/c;
  }
}

static uint32_t _FPDK_CalibrateGetNextFreqeuncy()
{
  _spiFrequency = 0;
  _spiSendPulseStartMeasure = true;

  uint32_t timeoutTick = HAL_GetTick() + 1000;
  for( ; HAL_GetTick()<timeoutTick; )
  {
    if( _spiFrequency )
      return _spiFrequency;
  }

  return 0;
}

static uint8_t _FPDK_CalibrateSingleFrequency(const uint32_t tune_frequency, const uint32_t multiplier, uint32_t* actual_frequency)
{
  int32_t bestDistance = 100000000; //100MHz, can not be reached
  uint8_t bestMatch = 0;
  for( uint16_t t=0; t<0xA0; t++ ) //0x9F seems maximum for IHRCR, upper bits unknown
  {
    uint32_t measured_frequency = multiplier * _FPDK_CalibrateGetNextFreqeuncy(); 

    int32_t distance = abs((int32_t)measured_frequency - (int32_t)tune_frequency);
    if( distance < bestDistance )
    {
      bestDistance = distance;
      bestMatch = t;
      *actual_frequency = measured_frequency;
    }
  }
  return bestMatch;
}

bool FPDK_Calibrate(const uint32_t type, const uint32_t vdd, 
                    const uint32_t frequency, const uint32_t multiplier,
                    uint8_t* fcalval, uint32_t* freq_tuned, 
                    uint8_t* bgcalval)
{
  bool ret = false;

  for( ;; )
  {
    //setup SPI + always running DMA for TX/RX
    memset(_spiDMATxBuffer, 0, sizeof(_spiDMATxBuffer));
    if( HAL_OK != HAL_SPI_Init(&hspi1) )
      break;;
    if( HAL_OK != HAL_SPI_TransmitReceive_DMA(&hspi1, _spiDMATxBuffer, _spiDMARxBuffer, SPI_BLOCK_SIZE) )
      break;

    //start IC
    if( !FPDK_SetVDD(vdd, FPDK_VDD_CAL_STARTUP_DELAYUS) )
      return false;

    //TODO: ADD BG TYPE

    *fcalval = _FPDK_CalibrateSingleFrequency( frequency, multiplier, freq_tuned );

    //found valid tuning (max 10% drift) ?
    if( abs( *freq_tuned - frequency ) < (frequency/10) )
      ret = true;

    break;
  }

  FPDK_SetVDD(0,0);
  HAL_SPI_Abort(&hspi1);
  HAL_SPI_DeInit(&hspi1);

  _FPDK_DelayUS(50000);

  return ret;
}

