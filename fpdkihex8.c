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
#include "fpdkihex8.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bool _X2UI8(const char* str, uint8_t* out)
{
  char tmp[] = {str[0],str[1],0};
  unsigned int r;
  if( 1 != sscanf(tmp, "%x", &r) )
    return false;
  *out = r;
  return true;
}

static bool _FPDKIHEX8_ParseLine(const char* line, uint8_t* type, uint16_t* address, uint8_t* data, uint8_t* datcount)
{
  uint8_t addrl, addrh;
  if( (strlen(line)<9) || (':' != line[0]) ||
      !_X2UI8(&line[1], datcount) || !_X2UI8(&line[3], &addrh) || !_X2UI8(&line[5], &addrl) || !_X2UI8(&line[7], type) )
    return false;
  *address = (((uint16_t)addrh)<<8) | addrl;

  uint8_t check;
  if( (strlen(line)<(9+2*(*datcount))) || !_X2UI8(&line[9+2*(*datcount)], &check) )
    return false;
  check += (*datcount) + addrl + addrh + (*type);

  for( uint8_t p=0; p<(*datcount); p++ )
  {
    if( !_X2UI8(&line[9+2*p], &data[p]) )
      return false;
    check += data[p];
  }
  return( 0 == check );
}

static int _FPDKIHEX8_CreateLine(const uint8_t type, const uint16_t address, const uint8_t* data, const uint8_t datcount, char lineout[600])
{
  sprintf(lineout, ":%02X%04X%02X", datcount, address, type);
  uint8_t check = datcount + address + (address>>8) + type;
  char tmp[4];
  for( uint8_t p=0; p<datcount; p++ )
  {
    sprintf(tmp, "%02X", data[p]);
    strcat(lineout, tmp);
    check += data[p];
  }
  check = -check;
  sprintf(tmp,"%02X\n", check);
  strcat(lineout, tmp);
  return strlen(lineout);
}

int FPDKIHEX8_ReadFile(const char* filename, uint16_t* datout, const uint16_t datcount)
{
  memset(datout, 0, sizeof(uint16_t)*datcount);

  FILE *fin = fopen(filename, "r");
  if( !fin )
    return -1;

  bool berr = false;
//  char *line = 0;
//  size_t len = 0;
//  while( getline(&line, &len, fin)>0 )
  char line[600];
  while( NULL != fgets(line, sizeof(line), fin) )
  {
    uint8_t tmp[256];
    uint8_t type;
    uint16_t address;
    uint8_t count;
    if( !_FPDKIHEX8_ParseLine(line, &type, &address, tmp, &count) || (type>1) )
    {
      berr = true;
      break;
    }

    if( 1 == type )
      break;

    if( (address+count) > datcount )
    {
      berr = true;
      break;
    }

    for( uint8_t p=0; p<count; p++ )
      datout[address+p] = 0x100 + tmp[p];
  }

//  if( line )
//    free(line);

  fclose(fin);

  if( berr )
    return -2;

  return 0;
}

int FPDKIHEX8_WriteFile(const char* filename, const uint8_t* datin, const uint16_t datlen)
{
  FILE *fout = fopen(filename, "w");
  if( !fout )
    return -1;

  bool berr = false;
  char line[600];
  uint8_t data[256];
  uint16_t lineitems = 0;
  uint16_t lineaddr = 0;

  for( uint16_t p=0; p<datlen; p++ )
  {
    data[lineitems] = datin[p];
    lineitems++;

    if( lineitems>=32 )
    {
      int linelength = _FPDKIHEX8_CreateLine(0, lineaddr, data, lineitems, line);
      if( linelength != fwrite(line, 1, linelength, fout) )
      {
        berr = true;
        break;
      }
      lineaddr += lineitems;
      lineitems = 0;
    }
  }
  
  int linelength = _FPDKIHEX8_CreateLine(1, 0, NULL, 0, line);
  if( linelength != fwrite(line, 1, linelength, fout) )
    berr = true;

  fclose(fout);

  if( berr )
    return -2;

  return 0;
}
