/*
Simple hardware selftest for the easy pdk programmer hardware

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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "fpdkutil.h"
#include "fpdkcom.h"

int main( int argc, const char * argv [] )
{
  //immediate output on stdout (no buffering)
  setvbuf(stdout,0,_IONBF,0);

  char compath[64];
  int comfd = FPDKCOM_OpenAuto(compath);
  if( comfd<0 )
  {
    printf("no programmer found\n\n");
    return -1;
  }

  //be carefully, remove IC from programmer!
  FPDKCOM_SetOutputVoltages(comfd, 5.0, 5.0);

  for(;;)
  {
    bool btn;
    FPDKCOM_GetButtonState(comfd, &btn);
    FPDKCOM_SetLed(comfd, btn?0xFF:0x00);

    float vdd,vpp,vref;
    if( FPDKCOM_MeasureOutputVoltages(comfd, &vdd, &vpp, &vref) )
      printf("vdd: %.2f   vpp: %.2f    (vref= %.2f)          \r", vdd, vpp, vref);

    fpdkutil_waitfdorkeypress(comfd, 20);
  }

  FPDKCOM_Close(comfd);

  return 0;
}
