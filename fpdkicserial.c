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

#include "fpdkicserial.h"

#include <string.h>

//TODO:
#include <stdio.h>

static const uint16_t _serial_pattern [][8] = 
{
  { 0x0146, 0x0150, 0x0153, 0x0145, 0x0152, 0x0149, 0x0141, 0x014C },  //13 bit    (ret 'F', ret 'P', ret 'S', ret 'E', ret 'R', ret 'I', ret 'A', ret 'L')
  { 0x0246, 0x0250, 0x0253, 0x0245, 0x0252, 0x0249, 0x0241, 0x024C },  //14+15 bit (ret 'F', ret 'P', ret 'S', ret 'E', ret 'R', ret 'I', ret 'A', ret 'L')
  { 0x0F46, 0x0F50, 0x0F53, 0x0F45, 0x0F52, 0x0F49, 0x0F41, 0x0F4C },  //16 bit    (ret 'F', ret 'P', ret 'S', ret 'E', ret 'R', ret 'I', ret 'A', ret 'L')
};

int FPDKSERIAL_InsertSerial(const FPDKICDATA* icdata, uint8_t* code, const uint16_t len, uint64_t serial)
{
  int inserts = 0;
  uint16_t* code16 = (uint16_t*)code;
  uint16_t  len16 = len/2;

  uint8_t idx;
  switch( icdata->codebits )
  {
    case 13: idx=0; break;
    case 14: idx=1; break;
    case 15: idx=1; break;
    case 16: idx=2; break;
    default:
      return 0;
  }
  for( uint16_t p=0; p<=len16-8; p++ )
  {
    //search pattern
    if( 0 == memcmp( &code16[p], &_serial_pattern[idx][0], 8*2 ) )
    {
      //insert serial number
      code16[p+0] = (code16[p+0]&0xFF00) | ((serial>> 0)&0xFF);
      code16[p+1] = (code16[p+1]&0xFF00) | ((serial>> 8)&0xFF);
      code16[p+2] = (code16[p+2]&0xFF00) | ((serial>>16)&0xFF);
      code16[p+3] = (code16[p+3]&0xFF00) | ((serial>>24)&0xFF);
      code16[p+4] = (code16[p+4]&0xFF00) | ((serial>>32)&0xFF);
      code16[p+5] = (code16[p+5]&0xFF00) | ((serial>>40)&0xFF);
      code16[p+6] = (code16[p+6]&0xFF00) | ((serial>>48)&0xFF);
      code16[p+7] = (code16[p+7]&0xFF00) | ((serial>>56)&0xFF);
      inserts++;
    }
  }
  return inserts;
}
