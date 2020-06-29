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

#ifndef __FPDK_H_
#define __FPDK_H_

#include <stdbool.h>
#include <stdint.h>

#define __FPDKSW__ "1.3"
#define __FPDKHW__ "1.2"

typedef enum FPDKICTYPE
{
  FPDK_IC_OTP1_2   = 0x12,
  FPDK_IC_OTP2_1   = 0x21,
  FPDK_IC_OTP2_2   = 0x22,
  FPDK_IC_OTP3_1   = 0x31,
  FPDK_IC_FLASH    = 0xF0,
  FPDK_IC_FLASH_1  = 0xF1,
  FPDK_IC_FLASH_2  = 0xF2,
} FPDKICTYPE;

#define  FPDK_IS_FLASH_TYPE(type) (0xF0==(type&0xF0))

void     FPDK_Init(void);
void     FPDK_DeInit(void);

void     FPDK_SetLeds(uint32_t val);
void     FPDK_SetLed(uint32_t led, bool enable);
bool     FPDK_IsButtonPressed(void);

bool     FPDK_SetVDD(uint32_t mV, uint32_t stabelizeDelayUS);
bool     FPDK_SetVPP(uint32_t mV, uint32_t stabelizeDelayUS);
uint32_t FPDK_GetAdcVref(void);
uint32_t FPDK_GetAdcVdd(void);
uint32_t FPDK_GetAdcVpp(void);

uint32_t FPDK_ProbeIC(FPDKICTYPE* type, uint32_t* vpp_cmd, uint32_t* vdd_cmd);

uint16_t FPDK_ReadIC(const uint16_t ic_id,
                     const FPDKICTYPE type,
                     const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                     const uint32_t vpp_read, const uint32_t vdd_read,
                     const uint32_t addr, const uint8_t addr_bits,
                     uint16_t* data, const uint8_t data_bits,
                     const uint32_t count);

uint16_t FPDK_VerifyIC(const uint16_t ic_id,
                       const FPDKICTYPE type,
                       const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                       const uint32_t vpp_read, const uint32_t vdd_read,
                       const uint32_t addr, const uint8_t addr_bits,
                       const uint16_t* data, const uint8_t data_bits,
                       const uint32_t count,
                       const bool addr_exclude_first_instr, const uint32_t addr_exclude_start, const uint32_t addr_exclude_end);

uint16_t FPDK_BlankCheckIC(const uint16_t ic_id,
                           const FPDKICTYPE type,
                           const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                           const uint32_t vpp_read, const uint32_t vdd_read,
                           const uint8_t addr_bits,
                           const uint8_t data_bits,
                           const uint32_t count,
                           const bool addr_exclude_first_instr, const uint32_t addr_exclude_start, const uint32_t addr_exclude_end);

uint16_t FPDK_EraseIC(const uint16_t ic_id,
                      const FPDKICTYPE type,
                      const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                      const uint32_t vpp_erase, const uint32_t vdd_erase,
                      const uint8_t erase_clocks);

uint16_t FPDK_WriteIC(const uint16_t ic_id,
                      const FPDKICTYPE type,
                      const uint32_t vpp_cmd, const uint32_t vdd_cmd,
                      const uint32_t vpp_write, const uint32_t vdd_write,
                      const uint32_t addr, const uint8_t addr_bits,
                      const uint16_t* data, const uint8_t data_bits,
                      const uint32_t count,
                      const uint8_t write_block_size, const uint8_t write_block_clock_groups, const uint8_t write_block_clocks_per_group);

bool FPDK_Calibrate(const uint32_t type, const uint32_t vdd,
                    const uint32_t frequency, const uint32_t multiplier,
                    uint8_t* fcalval1, uint32_t* freq1_tuned);

#endif //__FPDK_H_
