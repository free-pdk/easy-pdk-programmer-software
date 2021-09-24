/*
Simple streaming dac output on VDD and VPP (can be used to replay analog captured data)

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
#include <stdlib.h>
#include <unistd.h>

#include "fpdkutil.h"
#include "fpdkcom.h"

int main( int argc, const char * argv [] )
{
  //immediate output on stdout (no buffering)
  setvbuf(stdout,0,_IONBF,0);

  printf("%s v1.3 (streamed DAC output on VDD and VPP, input data: 2 channel, 16 bit signed)\n",argv[0]);
  
  if( argc!=3 ) {
    printf("Usage: %s datafile.raw frequency\n",argv[0]);
    return 0;
  }

  FILE *fin = fopen( argv[1], "rb");
  if( !fin ) {
    printf("ERROR: Could not open %s\n", argv[1]);
    return -1;
  }
    
  long freq = atol(argv[2]);
  long timerval = 48000000ULL / freq;
  
  if( (timerval<10) || (timerval>65535) ){
    printf("ERROR: Invalid timer value: %ld\n", timerval);
    return -1;
  }

  //open programmer
  int comfd = -1;
  printf("Searching programmer...");
  char compath[64];
  comfd = FPDKCOM_OpenAuto(compath);
  if( comfd<0 )
  {
    if( -3 == comfd )
    {
      printf(" found: %s\n", compath);
      printf("Error: PROTOCOL MISMATCH ERROR");
    }
    else
    {
      printf (" (tried serial ports up to %s) ", compath);
      fprintf(stderr, "No programmer found\n");
    }
    return -1;
  }
  else
    printf(" found: %s\n", compath);

  unsigned int hw_major, hw_minor, sw_major, sw_minor, proto_major, proto_minor;
  char fwstr[130];
  if( !FPDKCOM_GetVersion(comfd, &hw_major, &hw_minor, &sw_major, &sw_minor, &proto_major, &proto_minor, fwstr, sizeof(fwstr)) )
    return -1;

  printf("%s",fwstr);

  char vermsg[130];
  if( (proto_major>=1) && (proto_minor>=4) && FPDKCOM_GetVerMessage(comfd, vermsg, sizeof(vermsg) ) )
    printf("%s",vermsg);

  if( !FPDKCOM_DACOut(comfd, 0) ) {
    fprintf(stderr, "DAC reset failed\n");
    return -1;
  }

  uint16_t dacbuf[128];
  for( uint32_t t=0;;t++ ) {
    if( 124 != fread(dacbuf, sizeof(uint16_t), 124, fin) )
      break;

    for( uint32_t c=0; c<124; c++ )
      dacbuf[c]+=32768;

    if( !FPDKCOM_DACBuffer(comfd, dacbuf) ) {
      fprintf(stderr, "DAC buffer failed #%u\n",t);
      break;
    }

    if( 1==t ) {
      if( !FPDKCOM_DACOut(comfd, timerval) ) {
        fprintf(stderr, "DAC setup failed\n");
        break;
      }
      printf("sending DAC data...");
    }
  }
  fclose( fin );

  printf(" done.\n");

  FPDKCOM_DACOut(comfd, 0);
  FPDKCOM_Close(comfd);

  return 0;
}
