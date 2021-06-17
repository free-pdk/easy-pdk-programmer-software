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
  FPDK_IC_FLASH    = 0xF0,
} FPDKICTYPE;

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
  uint8_t    eccbits;
  uint16_t   codewords;
  uint16_t   ramsize;
  bool       exclude_code_first_instr;
  uint16_t   exclude_code_start;
  uint16_t   exclude_code_end;
  uint16_t   scramble_code_block_length;
  uint16_t   scramble_code_block_repeat;
  char       scramble_code_shuffle_bits[16];
  uint8_t    command_trailing_clocks;
  int16_t    calibration_loop_cycle_correction;
  uint8_t    cmd_read;
  float      vdd_cmd_read;
  float      vpp_cmd_read;
  float      vdd_read_hv;
  float      vpp_read_hv;
  float      vdd_cmd_write;
  float      vpp_cmd_write;
  uint8_t    cmd_write;
  float      vdd_write_hv;
  float      vpp_write_hv;
  uint8_t    write_block_address_first;
  uint8_t    write_block_size;
  uint8_t    write_block_limited;
  uint8_t    write_block_clock_groups;
  uint8_t    write_block_clock_group_lead_clocks;
  uint8_t    write_block_clock_group_slow_clocks;
  uint16_t   write_block_clock_group_slow_clock_hcycle;
  uint8_t    write_block_clock_group_trail_clocks;
  uint8_t    cmd_erase;
  float      vdd_cmd_erase;
  float      vpp_cmd_erase;
  float      vdd_erase_hv;
  float      vpp_erase_hv;
  uint8_t    erase_clocks;
  uint16_t   erase_clock_hcycle;

} FPDKICDATA;

FPDKICDATA* FPDKICDATA_GetICDataById12Bit(const uint16_t id12bit);
FPDKICDATA* FPDKICDATA_GetICDataByCmdResponse(const FPDKICTYPE type, const uint32_t cmdrsp);
FPDKICDATA* FPDKICDATA_GetICDataByName(const char* name);

#endif //__FPDKICDATA_H_
