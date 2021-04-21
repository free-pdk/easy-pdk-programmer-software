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

#include "fpdkicscramble.h"

#include <string.h>
#include <stdlib.h>

static uint16_t _scramble_instruction(const uint16_t in, const char* scrambler)
{
  uint16_t out = 0;
  for( unsigned int p=0; p<16; p++ )
  {
    char stridx[2] = { scrambler[p], 0 };
    uint8_t idx = strtol( stridx, NULL, 16);
    out |= (in & (1<<(15-idx)))?1<<(15-p):0;
  }
  return out;
}

static uint16_t _descramble_instruction(const uint16_t in, const char* scrambler)
{
  uint16_t out = 0;
  for( unsigned int p=0; p<16; p++ )
  {
    char stridx[2] = { scrambler[p], 0 };
    uint8_t idx = strtol( stridx, NULL, 16);
    out |= (in & (1<<(15-p)))?1<<(15-idx):0;
  }
  return out;
}

void FPDKSCRAMBLE_Scramble(const FPDKICDATA* icdata, uint8_t* code, const uint16_t len, const bool descramble)
{
  uint16_t* code16 = (uint16_t*)code;
  uint16_t  len16 = len/2;

  uint16_t scramble = 0;
  uint16_t repeat = 0;

  for( uint16_t p=0; p<len16; p++ )
  {
    if( 0 == repeat )
    {
      scramble = icdata->scramble_code_block_length;
      repeat = icdata->scramble_code_block_repeat;
    }

    if( scramble>0 )
    {
      if( descramble )
        code16[p] = _descramble_instruction(code16[p], icdata->scramble_code_shuffle_bits);
      else
        code16[p] = _scramble_instruction(code16[p], icdata->scramble_code_shuffle_bits);
      scramble--;
    }
    repeat--;
  }
}

