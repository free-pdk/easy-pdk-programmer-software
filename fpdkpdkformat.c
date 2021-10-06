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
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "fpdkpdkformat.h"

int32_t pdkhdrlen(uint8_t* datain, uint32_t datainlen)
{
  if( datainlen<0x100 )
    return -1;

  uint8_t* hdrdata = datain;

  uint32_t version = *((uint32_t*)&hdrdata[0x08]);

  uint32_t marker  = *((uint32_t*)&hdrdata[0x00]);

  if( 0xFFAA5512 != marker )
    return -2;

  uint32_t extrahdr = *((uint16_t*)&hdrdata[0x26]) +
                      *((uint16_t*)&hdrdata[0x28]) +
                      *((uint16_t*)&hdrdata[0x2C]) +
                      *((uint16_t*)&hdrdata[0xD0]);

  //package extra info (0 terminated ASCII string of pin names at end of header extra data)
  if( version>=0x1c )
    extrahdr += hdrdata[0x49]; //length of the 0 terminated string (including terminating 0)

  return(0x100 + extrahdr);
}

uint32_t pdkchecksum( uint8_t* datain, uint32_t datainlen, uint32_t initval)
{
  uint16_t* data = (uint16_t*)datain;

  uint32_t  len = (datainlen >> 1) - 1;
  uint32_t  csum = initval ^ data[len];

  for(;len>0;)
  {
    len--;

    uint32_t v6 = *data++;
    uint32_t v5 = (v6 + len + (csum>>16))&0xFFFF;

    switch ( v5&7 )
    {
      case 0: csum += v5 + (v6 << 8) + 1; break;
      case 1: csum ^= (v6 << 8) + v5 + 3; break;
      case 2: csum += 4 * v5; break;
      case 3: csum ^= 16 * v5; break;
      case 4: csum += 32 * v5; break;
      case 5: csum ^= (v6 >> 3) + (v5 << 6); break;
      case 6: csum += (v5 << 7) - (v6 >> 3); break;
      case 7: csum ^= v6 + (v5 << 8); break;
    }
  }
  return csum;
}



int32_t depdk(uint8_t* datain, uint32_t datainlen, uint8_t* dataout, uint32_t dataoutlen)
{
  if( datainlen<0x100 )
    return -1;

  uint8_t* hdrdata = datain;

  uint32_t version = *((uint32_t*)&hdrdata[0x08]);
  uint32_t datalen = *((uint32_t*)&hdrdata[0x20]);

  uint32_t extrahdr = *((uint16_t*)&hdrdata[0x26]) +
                      *((uint16_t*)&hdrdata[0x28]) +
                      *((uint16_t*)&hdrdata[0x2C]) +
                      *((uint16_t*)&hdrdata[0xD0]);

  if( version>=0x1c )
    extrahdr += hdrdata[0x49];

  uint16_t key[0x10];
  memcpy( key, &hdrdata[0xE0], sizeof(key) );

  uint16_t kxorw7C92 = key[3]^key[14];
  uint16_t kxorw7E8A = key[7]^key[15];

  if( (version>=0x15) && (version<=0x17) ) {
    kxorw7C92 ^= 0x1234; kxorw7E8A ^= 0x5678;
  }
  if( version==0x1D ) {
    kxorw7C92 = key[4]^key[15]^0x1234;
    kxorw7E8A = key[8]^key[10]^0x57AE;
  }

  if( dataoutlen<datalen )
    return -2;

  uint16_t keyindex = 0;

  for( uint32_t fullpos=0; fullpos<datalen; fullpos+=32 ) {
    uint16_t data[32];
    memcpy( data, datain + 0x100 + extrahdr + fullpos*2, sizeof(data) );

    uint32_t dataptr = 0;
    for( uint32_t j=0; j<4; j++ ) {
      uint16_t tmp_xor_key1;
      uint16_t tmp_xor_key2;

      keyindex += kxorw7C92;
      tmp_xor_key1 = data[dataptr];
      data[dataptr] += key[keyindex&0xF];
      dataptr++;
      key[fullpos&0xF] ^= tmp_xor_key1;

      keyindex += tmp_xor_key1;
      tmp_xor_key2 = data[dataptr];
      data[dataptr] = (data[dataptr] + kxorw7E8A ) ^ key[keyindex&0xF];
      dataptr++;
      key[kxorw7E8A&0xF] ^= kxorw7C92;

      keyindex ^= tmp_xor_key2;
      kxorw7C92 = data[dataptr];
      data[dataptr] += key[keyindex&0xF]; 
      dataptr++;
      key[kxorw7E8A&0xF] ^= 0x55AA;

      keyindex = (keyindex + kxorw7C92)>>1;
      kxorw7E8A = data[dataptr];
      data[dataptr] = (data[dataptr] - tmp_xor_key1) ^ key[keyindex&0xF];
      dataptr++;

      keyindex += kxorw7E8A;
      tmp_xor_key1 = data[dataptr];
      data[dataptr] ^= key[keyindex&0xF]; 
      dataptr++;
      key[kxorw7C92&0xF] += kxorw7E8A;

      keyindex ^= tmp_xor_key1;
      tmp_xor_key2 = data[dataptr];
      data[dataptr] = (data[dataptr] ^ kxorw7E8A) ^ key[keyindex&0xF];
      dataptr++;
      key[tmp_xor_key2&0xF] ^= tmp_xor_key1;

      keyindex ^= tmp_xor_key2;
      kxorw7C92 = data[dataptr];
      data[dataptr] += key[keyindex&0xF];
      dataptr++;
      key[tmp_xor_key1&0xF] += tmp_xor_key2;

      keyindex += kxorw7C92;
      kxorw7E8A = data[dataptr];
      data[dataptr] = (data[dataptr] + tmp_xor_key1) ^ key[keyindex&0xF];
      dataptr++;

      key[2] ^= tmp_xor_key1;
      key[4] += tmp_xor_key2;
      key[6] += kxorw7E8A;
      key[8] -= kxorw7C92;
      key[1] ^= key[15];
      key[3] ^= key[14];
      key[5] ^= key[13];
      keyindex += j;
    }

    memcpy( dataout + fullpos*2, data, sizeof(data) );
  }

  return datalen*2;
}

int32_t enpdk(uint8_t* hdrin, uint32_t hdrinlen, uint8_t* datain, uint32_t datainlen, uint8_t* dataout, uint32_t dataoutlen)
{
  if( hdrinlen<0x100 )
    return -1;

  uint8_t* hdrdata = datain;

  uint32_t version = *((uint32_t*)&hdrdata[0x08]);
  uint32_t datalen = *((uint32_t*)&hdrdata[0x20]);

  uint32_t extrahdr = *((uint16_t*)&hdrdata[0x26]) +
                      *((uint16_t*)&hdrdata[0x28]) +
                      *((uint16_t*)&hdrdata[0x2C]) +
                      *((uint16_t*)&hdrdata[0xD0]);

  if( version>=0x1c )
    extrahdr += hdrdata[0x49];

  uint16_t key[0x10];
  memcpy( key, &hdrdata[0xE0], sizeof(key) );

  uint16_t kxorw7C92 = key[3]^key[14];
  uint16_t kxorw7E8A = key[7]^key[15];

  if( (version>=0x15) && (version<=0x17) ) {
    kxorw7C92 ^= 0x1234; kxorw7E8A ^= 0x5678;
  }

  if( dataoutlen < (hdrinlen+datalen) )
    return -2;

  if( hdrinlen != (0x100 + extrahdr) )
    return -3;

  memcpy( dataout, hdrdata, hdrinlen );

  uint16_t keyindex = 0;

  for( uint32_t fullpos=0; fullpos<datalen; fullpos+=32 ) {
    uint16_t data[32];
    memcpy( data, datain + fullpos*2, sizeof(data) );

    uint32_t dataptr = 0;
    for( uint32_t j=0; j<4; j++ ) {
      keyindex += kxorw7C92;
      uint16_t tmp_xor_key1 = data[dataptr] - key[keyindex&0xF];
      data[dataptr] = tmp_xor_key1;
      dataptr++;
      key[fullpos&0xF] ^= tmp_xor_key1;

      keyindex += tmp_xor_key1;
      uint16_t tmp_xor_key2 = (data[dataptr] ^ key[keyindex&0xF]) - kxorw7E8A;
      data[dataptr] = tmp_xor_key2;
      dataptr++;
      key[kxorw7E8A&0xF] ^= kxorw7C92;

      keyindex ^= tmp_xor_key2;
      kxorw7C92 = data[dataptr] - key[keyindex&0xF]; 
      data[dataptr] = kxorw7C92;
      dataptr++;
      key[kxorw7E8A&0xF] ^= 0x55AA;

      keyindex = (keyindex + kxorw7C92)>>1;
      kxorw7E8A = (data[dataptr] ^ key[keyindex&0xF]) + tmp_xor_key1;
      data[dataptr] = kxorw7E8A;
      dataptr++;

      keyindex += kxorw7E8A;
      tmp_xor_key1 = data[dataptr] ^ key[keyindex&0xF];
      data[dataptr] = tmp_xor_key1;
      dataptr++;
      key[kxorw7C92&0xF] += kxorw7E8A;

      keyindex ^= tmp_xor_key1;
      tmp_xor_key2 = (data[dataptr] ^ key[keyindex&0xF]) ^ kxorw7E8A;
      data[dataptr] = tmp_xor_key2;
      dataptr++;
      key[tmp_xor_key2&0xF] ^= tmp_xor_key1;

      keyindex ^= tmp_xor_key2;
      kxorw7C92 = data[dataptr] - key[keyindex&0xF];
      data[dataptr] = kxorw7C92;  
      dataptr++;
      key[tmp_xor_key1&0xF] += tmp_xor_key2;

      keyindex += kxorw7C92;
      kxorw7E8A = (data[dataptr] ^ key[keyindex&0xF]) - tmp_xor_key1;
      data[dataptr] = kxorw7E8A;
      dataptr++;

      key[2] ^= tmp_xor_key1;
      key[4] += tmp_xor_key2;
      key[6] += kxorw7E8A;
      key[8] -= kxorw7C92;
      key[1] ^= key[15];
      key[3] ^= key[14];
      key[5] ^= key[13];
      keyindex += j;
    }

    memcpy( dataout + hdrinlen + fullpos*2, data, sizeof(data) );
  }

  return hdrinlen + datalen*2;
}

int FPDKPDK_ReadFile(const char* filename, uint8_t* data, const uint16_t datalen, FPDKPDKHDR* pdkhdr)
{
  uint8_t datain[0x3000];

  FILE* fin = fopen( filename, "rb");
  if( !fin )
    return -1;

  int32_t datainlen = fread( datain, 1, sizeof(datain), fin );
  if( datainlen<=0 )
    return -2;
  fclose(fin);

  int32_t dataoutlen = depdk( datain, datainlen, data, datalen );
  if( dataoutlen<=0 )
    return -3;

  int32_t hdrlen = pdkhdrlen(datain,datainlen);

  memcpy( pdkhdr, datain, sizeof(FPDKPDKHDR) );

  uint32_t fcsum;
  memcpy( &fcsum, &datain[0x0C], sizeof(fcsum) );
  memset( &datain[0x0C], 0, 4 );

  uint32_t csum1 = pdkchecksum( datain, hdrlen, 0 );
  uint32_t csum2 = pdkchecksum( data, dataoutlen, 0 );
  uint32_t csumh = csum1+csum2;

  if( csumh!=fcsum )
    return -4;
  
  if( !pdkhdr->tuning_freq_half )
    pdkhdr->tuning_freq_half = 8000000;

  if( !pdkhdr->tuning_voltage_mv )
    pdkhdr->tuning_voltage_mv = 5000;

  return dataoutlen;
}
