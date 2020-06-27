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
#ifndef __FPDKICDATA_H_
#define __FPDKICDATA_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum FPDKICTYPE
{
  FPDK_IC_OTP1_2  = 0x12,
  FPDK_IC_OTP2_1  = 0x21,
  FPDK_IC_OTP2_2  = 0x22,
  FPDK_IC_OTP3_1  = 0x31,
//  FPDK_IC_FLASH    = 0xF0,
  FPDK_IC_FLASH_1  = 0xF1,
  FPDK_IC_FLASH_2  = 0xF2,
} FPDKICTYPE;

#define FPDK_IS_FLASH_TYPE(type) (0xF0==(type&0xF0))

typedef struct FPDKICDATA
{
  char       name[32];
  char       name_variant_1[32];
  char       name_variant_2[32];
  uint16_t   id12bit;
  uint16_t   otpid;
  FPDKICTYPE type;
  uint8_t    addressbits;
  uint8_t    codebits;
  uint16_t   codewords;
  uint16_t   ramsize;
  bool       exclude_code_first_instr;
  uint16_t   exclude_code_start;
  uint16_t   exclude_code_end;
  float      vdd_cmd_read;
  float      vpp_cmd_read;
  float      vdd_read_hv;
  float      vpp_read_hv;
  float      vdd_cmd_write;
  float      vpp_cmd_write;
  float      vdd_write_hv;
  float      vpp_write_hv;
  uint8_t    write_block_size;
  uint8_t    write_block_clock_groups;
  uint8_t    write_block_clocks_per_group;
  float      vdd_cmd_erase;
  float      vpp_cmd_erase;
  float      vdd_erase_hv;
  float      vpp_erase_hv;
  uint8_t    erase_clocks;

} FPDKICDATA;

FPDKICDATA* FPDKICDATA_GetICDataById12Bit(const uint16_t id12bit);
FPDKICDATA* FPDKICDATA_GetICDataByCmdResponse(const FPDKICTYPE type, const uint32_t cmdrsp);
FPDKICDATA* FPDKICDATA_GetICDataByName(const char* name);

#endif //__FPDKICDATA_H_
