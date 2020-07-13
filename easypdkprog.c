/*
Copyright (C) 2019-2020  freepdk  https://free-pdk.github.io

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
#include <inttypes.h>
#include <unistd.h>

#include "fpdkutil.h"
#include "fpdkcom.h"
#include "fpdkicdata.h"
#include "fpdkiccalib.h"
#include "fpdkicserial.h"
#include "fpdkihex8.h"
#include "argp.h"

#ifndef EPDKVER
#define EPDKVER "unknown version"
#endif

const char *argp_program_version                = "easypdkprog " EPDKVER;
static const char easypdkprog_doc[]             = "easypdkprog -- read, write and execute programs on PADAUK microcontroller\nhttps://free-pdk.github.io";
static const char easypdkprog_args_doc[]        = "list|probe|read|write|erase|start [FILE]";

static struct argp_option easypdkprog_options[] = {
  {"verbose",      'v', 0,        0,  "Verbose output" },
  {"port",         'p', "PORT",   0,  "COM port of programmer. Default: Auto search" },
  {"bin",          'b', 0,        0,  "Binary file output. Default: ihex8" },
  {"noerase",     555,  0,        0,  "Skip erase before write" },
  {"noblankchk",  666,  0,        0,  "Skip blank check before write" },
  {"securefill",  777,  0,        0,  "Fill unused space with 0 (NOP) to prevent readout" },
  {"noverify",    888,  0,        0,  "Skip verify after write" },
  {"nocalibrate" ,999,  0,        0,  "Skip calibration after write." },
  {"fuse",         'f', "FUSE",   0,  "FUSE value, e.g. 0x31FD"},
  {"allowsecfuse",444,  0,        0,  "Allow setting the security fuse."},
  {"runvdd",       'r', "VDD",    0,  "Voltage for running the IC. Default: 5.0" },
  {"icname",       'n', "NAME",   0,  "IC name, e.g. PFS154" },
  {"icid",         'i', "ID",     0,  "IC ID 12 bit, e.g. 0xAA1" },
  {"serial",       's', "SERIAL", 0,  "SERIAL value (64bit), e.g. 0x123456789ABCDEF0"},
  { 0 }
};

struct easypdkprog_args {
  char     command;
  int      verbose;
  char     *port;
  int      binout;
  char     *inoutfile;
  int      securefill;
  int      nocalibrate;
  int      noerase;
  int      noblankcheck;
  int      noverify;
  int      allowsecfuse;
  uint16_t fuse;
  uint64_t serial;
  float    runvdd;
  char     *ic;
  uint16_t icid;
};

static error_t easypdkprog_parse_opt(int key, char *arg, struct argp_state *state)
{
  struct easypdkprog_args *arguments = state->input;
  switch (key)
  {
    case 'v': arguments->verbose = 1; break;
    case 'p': arguments->port = arg; break;
    case 'b': arguments->binout = 1; break;
    case 555: arguments->noerase = 1; break;
    case 666: arguments->noblankcheck = 1; break;
    case 777: arguments->securefill = 1; break;
    case 888: arguments->noverify = 1; break;
    case 999: arguments->nocalibrate = 1; break;
    case 'f': if(arg) arguments->fuse = strtol(arg,NULL,16); break;
    case 'n': arguments->ic = arg; break;
    case 'i': if(arg) arguments->icid = strtol(arg,NULL,16); break;
    case 'r': if(arg) sscanf(arg,"%f",&arguments->runvdd); break;
    case 's': if(arg) arguments->serial = strtoull(arg,NULL,16); break;
    case 444: arguments->allowsecfuse = 1; break;

    case ARGP_KEY_ARG:
      if(0 == state->arg_num)
      {
        if( !strcmp(arg,"list") && 
            !strcmp(arg,"probe") && 
            !strcmp(arg,"read") && 
            !strcmp(arg,"write") && 
            !strcmp(arg,"erase") && 
            !strcmp(arg,"start") )
        {
          argp_usage(state);
        }
        arguments->command = arg[0];
      }
      else if(1 == state->arg_num)
      {
        arguments->inoutfile = arg;
      }
      else
        argp_usage(state);
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage(state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { easypdkprog_options, easypdkprog_parse_opt, easypdkprog_args_doc, easypdkprog_doc };

#define PROTOCOL_MISMATCH_ERROR_STRING "Programmer protocol mismatch - Please update programmer firmware"

static int qscmp(const void *p1, const void *p2)
{
  return strcmp(p1, p2);
}

int main( int argc, const char * argv [] )
{
  //immediate output on stdout (no buffering)
  setvbuf(stdout,0,_IONBF,0);
  
  struct easypdkprog_args arguments = { .runvdd=5.0, .fuse=0xFFFF, .serial=0x4C41495245535046ULL };
  argp_parse(&argp, argc, (char**)argv, 0, 0, &arguments);

  verbose_set(arguments.verbose);

  if( 'l'==arguments.command )
  {
    printf("Supported ICs:\n");

    char mculist[100][128];
    unsigned int idx=0;

    for( uint16_t id=1; id<0xFFF; id++ )
    {
      FPDKICDATA* icdata = FPDKICDATA_GetICDataById12Bit(id);
      if( icdata )
      {
        sprintf( mculist[idx++], " %-8s (0x%03X): %s: %d (%d bit), RAM: %3d bytes %s", icdata->name, icdata->id12bit, FPDK_IS_FLASH_TYPE(icdata->type)?"FLASH":"OTP  ", icdata->codewords, icdata->codebits, icdata->ramsize, icdata->vdd_cmd_write?"":"(RO)");
        if( icdata->name_variant_1[0] )
          sprintf( mculist[idx++], " %-8s (0x%03X): %s: %d (%d bit), RAM: %3d bytes %s", icdata->name_variant_1, icdata->id12bit, FPDK_IS_FLASH_TYPE(icdata->type)?"FLASH":"OTP  ", icdata->codewords, icdata->codebits, icdata->ramsize, icdata->vdd_cmd_write?"":"(RO)");
        if( icdata->name_variant_2[0] )
          sprintf( mculist[idx++], " %-8s (0x%03X): %s: %d (%d bit), RAM: %3d bytes %s", icdata->name_variant_2, icdata->id12bit, FPDK_IS_FLASH_TYPE(icdata->type)?"FLASH":"OTP  ", icdata->codewords, icdata->codebits, icdata->ramsize, icdata->vdd_cmd_write?"":"(RO)");
      }
    }

    qsort( mculist, idx, 128, qscmp );

    for( unsigned int i=0; i<idx; i++ )
      printf( "%s\n", mculist[i] );

    return 0;
  }

  //pre checks
  FPDKICDATA* icdata = NULL;
  if( ('r'==arguments.command) || ('w'==arguments.command) || ('e'==arguments.command) )
  {
    if( !arguments.icid && !arguments.ic)
    {
      fprintf(stderr, "ERROR: IC NAME and IC ID unspecified. Use -n or -i option.\n");
      return -2;
    }
    else
    {
      icdata = FPDKICDATA_GetICDataById12Bit(arguments.icid);
      if( !icdata )
        icdata = FPDKICDATA_GetICDataByName(arguments.ic);

      if( !icdata )
      {
        fprintf(stderr, "ERROR: Unknown IC NAME / IC ID.\n");
        return -2;
      }
    }
  }

  if( ('w'==arguments.command) && !arguments.inoutfile )
  {
    fprintf(stderr, "ERROR: Write requires an input file.\n");
    return -3;
  }

  //open programmer
  int comfd = -1;
  if( !arguments.port )
  {
    verbose_printf("Searching programmer...");
    char compath[64];
    comfd = FPDKCOM_OpenAuto(compath);
    if( comfd<0 )
    {
      if( -3 == comfd )
      {
        verbose_printf(" found: %s\n", compath);
        printf("Error: " PROTOCOL_MISMATCH_ERROR_STRING ".\n");
      }
      else
      {
        verbose_printf (" (tried serial ports up to %s) ", compath);
        fprintf(stderr, "No programmer found\n");
      }
      return -1;
    }
    else
      verbose_printf(" found: %s\n", compath);
  }
  else
  {
    comfd = FPDKCOM_Open(arguments.port);
    if( comfd<0 )
    {
      if ( -3 == comfd )
        fprintf(stderr, "Error %d (" PROTOCOL_MISMATCH_ERROR_STRING ") connecting to programmer on port: %s\n\n", comfd, arguments.port);
      else
        fprintf(stderr, "Error %d connecting to programmer on port: %s\n\n", comfd, arguments.port);
      return -1;
    }
  }

  unsigned int hw_major, hw_minor, sw_major, sw_minor, proto_major, proto_minor;
  char fwstr[130];
  if( !FPDKCOM_GetVersion(comfd, &hw_major, &hw_minor, &sw_major, &sw_minor, &proto_major, &proto_minor, fwstr, sizeof(fwstr)) )
    return -1;

  verbose_printf("%s",fwstr);

  switch( arguments.command )
  {
    case 'p': //probe
    {
      printf("Probing IC... ");
      FPDKICTYPE type;
      float vpp,vdd;
      int icid = FPDKCOM_IC_Probe(comfd,&vpp,&vdd,&type);
      if( icid<=0 )
        printf("Nothing found.\n");      
      else
      if( (icid>=FPDK_ERR_ERROR) && (icid<=0xFFFF) )
      {
        fprintf(stderr, "ERROR: %s\n",FPDK_ERR_MSG[icid&0x000F]);
        return -4;
      }
      else
      {
        printf("found.\nTYPE:%s RSP:0x%X VPP=%.2f VDD=%.2f\n",FPDK_IS_FLASH_TYPE(type)?"FLASH":"OTP",icid,vpp,vdd);
        icdata = FPDKICDATA_GetICDataByCmdResponse(type,icid);
        if( icdata )
        {
          printf("IC is supported: %s", icdata->name);
          if( icdata->name_variant_1[0] )
            printf(" / %s", icdata->name_variant_1);
          if( icdata->name_variant_2[0] )
            printf(" / %s", icdata->name_variant_2);

          printf(" ICID:0x%03X", icdata->id12bit);
        }
        else
          printf("Unsupported IC");

        printf("\n");
      }
    }
    break;

    case 'r': //read
    {
      if( !icdata->vdd_cmd_read  || !icdata->vpp_cmd_read )
      {
        printf("Read for this IC not implemented yet.\n");
        return -20;
      }
      printf("Reading IC (%" PRIu32 " words)...", icdata->codewords);
      int r = FPDKCOM_IC_Read(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_read, icdata->vpp_cmd_read, icdata->vdd_read_hv, icdata->vpp_read_hv, 0, icdata->addressbits, 0, icdata->codebits, icdata->codewords);
      if( r>=FPDK_ERR_ERROR )
      {
        fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
        return -4;
      }
      else
      if( r != icdata->id12bit )
      {
        fprintf(stderr, "ERROR: Read failed.\n");
        return -5;
      }
      else
      {
        printf("done.\n");
        if( arguments.inoutfile )
        {
          uint8_t buf[0x1000*2];
          if( FPDKCOM_GetBuffer(comfd, 0, buf, icdata->codewords*sizeof(uint16_t))>0 )
          {
            if( arguments.binout )
            {
              FILE *f = fopen(arguments.inoutfile,"wb");
              if(f)
              {
                fwrite(buf, 1, icdata->codewords*sizeof(uint16_t), f);
                fclose(f);
              }
              else
              {
                fprintf(stderr, "ERROR: Could not write file: %s\n", arguments.inoutfile);
                return -6;
              }
            }
            else
            {
              if( FPDKIHEX8_WriteFile(arguments.inoutfile, buf, icdata->codewords*sizeof(uint16_t)) < 0 )
              {
                fprintf(stderr, "ERROR: Could not write file: %s\n", arguments.inoutfile);
                return -6;
              }
            }
          }
          else
          {
            fprintf(stderr, "ERROR: Could not read data from programmer\n");
            return -7;
          }
        }
      }
    }
    break;

    case 'w': //write
    {
      if( !icdata->vdd_cmd_write  || !icdata->vpp_cmd_write || !icdata->vdd_write_hv )
      {
        fprintf(stderr, "Write for this IC not implemented yet.\n");
        return -20;
      }
      uint16_t write_data[0x1000*2];
      if( FPDKIHEX8_ReadFile(arguments.inoutfile, write_data, 0x1000*2) < 0 )
      {
        fprintf(stderr, "ERROR: Invalid input file / not ihex8 format.\n");
        return -8;
      }

      uint8_t data[0x1000*2];
      memset(data, arguments.securefill?0x00:0xFF, sizeof(data));
      uint32_t len = 0;
      for( uint32_t p=0; p<sizeof(data); p++)
      {
        if( write_data[p] & 0xFF00 )
        {
          data[p] = write_data[p]&0xFF;
          len = p + 1;
        }
      }

      if( arguments.securefill )
      {
        uint16_t fillend = icdata->codewords - 8;
        if( icdata->exclude_code_start && (icdata->exclude_code_start < fillend) )
          fillend = icdata->exclude_code_start;

        len = fillend*sizeof(uint16_t);
      }

      if( 0 == len )
      {
        printf("Nothing to write\n");
        break;
      }

      if( (0xFFFF == arguments.fuse) && (icdata->codewords*2 == len) )
      {
        arguments.fuse = (((uint16_t)data[len-1])<<8) | data[len-2];
        data[len-1]=0xFF; data[len-2]=0xFF; len -= 2;
      }

      #define MAX_CALIBRATIONS 16
      FPDKCALIBDATA  calibdata[MAX_CALIBRATIONS];
      memset(calibdata, 0, sizeof(calibdata));

      uint32_t calibrations = 0;

      if( !arguments.nocalibrate )
      {
        for( calibrations=0; calibrations<MAX_CALIBRATIONS; calibrations++ )
        {
          if( !FPDKCALIB_InsertCalibration(icdata, data, len, &calibdata[calibrations]) )
            break;

          if( FPDKCALIB_UNKNOWN == calibdata[calibrations].type )
          {
            fprintf(stderr, "ERROR: Unknown calibration found. Please check that you have the latest programer software and firmware or recompile your application to match this version.\n");
            return -17;
          }
        }
      }

      int inserts = FPDKSERIAL_InsertSerial(icdata, data, len, arguments.serial);

      if( arguments.serial != 0x4C41495245535046ULL )
      {
        printf("Setting serial: 0x%02X%02X%02X%02X%02X%02X%02X%02X (%d insertions)\n", 
               (uint8_t)(arguments.serial>>56)&0xFF, (uint8_t)(arguments.serial>>48)&0xFF, (uint8_t)(arguments.serial>>40)&0xFF, (uint8_t)(arguments.serial>>32)&0xFF, 
               (uint8_t)(arguments.serial>>24)&0xFF, (uint8_t)(arguments.serial>>16)&0xFF, (uint8_t)(arguments.serial>>8)&0xFF, (uint8_t)(arguments.serial>>0)&0xFF,
               inserts );
      }
      else
      {
        if( inserts>0 )
        {
          fprintf(stderr, "ERROR: Firmware uses serial feature, but no serial specified. Use --serial option.\n");
          return -22;
        }
      }

      if( FPDK_IS_FLASH_TYPE(icdata->type) && !arguments.noerase )
      {
        printf("Erasing IC... ");

        int r = FPDKCOM_IC_Erase(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_erase, icdata->vpp_cmd_erase, icdata->vdd_erase_hv, icdata->vpp_erase_hv, icdata->erase_clocks );
        if( r>=FPDK_ERR_ERROR )
        {
          fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
          return -4;
        }
        else
        if( r != icdata->id12bit )
        {
          fprintf(stderr, "ERROR: Erase failed.\n");
          return -9;
        }
        else
          printf("done.\n");
      }

      if( !arguments.noblankcheck )
      {
        verbose_printf("Blank check IC... ");
        int r = FPDKCOM_IC_BlankCheck(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_read, icdata->vpp_cmd_read, icdata->vdd_read_hv, icdata->vpp_read_hv, icdata->addressbits, icdata->codebits, icdata->codewords, icdata->exclude_code_first_instr, icdata->exclude_code_start, icdata->exclude_code_end);
        if( r>=FPDK_ERR_ERROR )
        {
          fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
          return -4;
        }
        if( r != icdata->id12bit )
        {
          fprintf(stderr, "ERROR: Blank check failed.\n");
          return -10;
        }
        verbose_printf("done.\n");
      }

      uint32_t codewords = (len+1)/2;

      printf("Writing IC (%" PRIu32 " words)... ", codewords);

      if( !FPDKCOM_SetBuffer(comfd, 0, data, len) )
      {
        fprintf(stderr, "ERROR: Could not send data to programmer\n");
        return -11;
      }

      int r = FPDKCOM_IC_Write(comfd, icdata->id12bit, icdata->type,
                               icdata->vdd_cmd_write, icdata->vpp_cmd_write, icdata->vdd_write_hv, icdata->vpp_write_hv,
                               0, icdata->addressbits, 0, icdata->codebits, codewords,
                               icdata->write_block_size, icdata->write_block_clock_groups, icdata->write_block_clocks_per_group);
      if( r>=FPDK_ERR_ERROR )
      {
        fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
        return -4;
      }
      if( r != icdata->id12bit )
      {
        fprintf(stderr, "ERROR: Write failed.\n");
        return -12;
      }
      printf("done.\n");

      if( !arguments.noverify )
      {
        verbose_printf("Verifiying IC... ");
        int r = FPDKCOM_IC_Verify(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_read, icdata->vpp_cmd_read, icdata->vdd_read_hv, icdata->vpp_read_hv, 0, icdata->addressbits, 0, icdata->codebits, codewords, icdata->exclude_code_first_instr, icdata->exclude_code_start, icdata->exclude_code_end);
        if( r>=FPDK_ERR_ERROR )
        {
          printf("FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
          return -4;
        }
        if( r != icdata->id12bit )
        {
          fprintf(stderr, "ERROR: Verify failed.\n");
          return -13;
        }
        verbose_printf("done.\n");
      }

      if( calibrations>0 )
      {
        printf("Calibrating IC\n");
        for( uint32_t c=0; c<calibrations; c++ )
        {
          uint32_t posmin = 0xFFFF;
          uint32_t calib = 0xFFFF;
          for( uint32_t s=0; s<calibrations; s++ )
          {
            if( (calibdata[s].type) && (calibdata[s].pos<posmin) )
            {
              calib = s;
              posmin = calibdata[s].pos;
            }
          }
          if( 0xFFFF == calib )
            break;
          switch( calibdata[calib].type )
          {
            case FPDKCALIB_IHRC:         printf("* IHRC SYSCLK=%dHz @ %.2fV ", calibdata[calib].frequency, (float)calibdata[calib].millivolt/1000.0); break;
            case FPDKCALIB_ILRC:         printf("* ILRC SYSCLK=%dHz @ %.2fV ", calibdata[calib].frequency, (float)calibdata[calib].millivolt/1000.0); break;
            case FPDKCALIB_BG:           printf("* BandGap "); break;
            default:
              fprintf(stderr, "ERROR: Unknown calibration\n");
              return -17;
          }
          printf("... ");

          uint8_t fcalval;
          uint32_t fcalfreq;

          if( !FPDKCOM_IC_Calibrate(comfd, calibdata[calib].type, calibdata[calib].millivolt, calibdata[calib].frequency, calibdata[calib].loopcycles, &fcalval, &fcalfreq) )
          {
            printf("failed.\n");
            fprintf(stderr, "ERROR: Calibration failed\n");
            return -17;
          }

          printf("calibration result: ");
          switch( calibdata[calib].type )
          {
            case FPDKCALIB_IHRC:
            case FPDKCALIB_ILRC:
              {
                printf("%dHz ", fcalfreq);
                //found valid tuning (max 10% drift) ?
                if( abs( (int32_t)fcalfreq - (int32_t)calibdata[calib].frequency ) > (calibdata[calib].frequency/10) )
                {
                  printf("(0x%02X)  ", fcalval);
                  printf("out of range.\n");
                  fprintf(stderr, "ERROR: Calibration failed\n");
                  return -18;
                }
              }
              break;

            default:
              break;
          }
          printf("(0x%02X)  ", fcalval);

          if( FPDKCALIB_RemoveCalibration( &calibdata[calib], data, fcalval) )
          {
            if( !FPDKCOM_SetBuffer(comfd, 0, data, len) )
            {
              fprintf(stderr, "ERROR: Could not send data to programmer\n");
              return -16;
            }

            uint32_t codewords = (len+1)/2;

            int r = FPDKCOM_IC_Write(comfd, icdata->id12bit, icdata->type,
                                     icdata->vdd_cmd_write, icdata->vpp_cmd_write, icdata->vdd_write_hv, icdata->vpp_write_hv,
                                     0, icdata->addressbits, 0, icdata->codebits, codewords,
                                     icdata->write_block_size, icdata->write_block_clock_groups, icdata->write_block_clocks_per_group);
            if( r>=FPDK_ERR_ERROR )
            {
              fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
              return -4;
            }
            if( r != icdata->id12bit )
            {
              fprintf(stderr, "ERROR: Write calibration failed.\n");
              return -16;
            }

            calibdata[calib].type = 0;
          }
          else
          {
            fprintf(stderr, "ERROR: Removing calibration function.\n");
            return -17;
          }
          printf("done.\n");
        }
      }

      if( 0xFFFF != arguments.fuse )
      {
        if( (0==(arguments.fuse&1)) && !arguments.allowsecfuse )
        {
          printf("Setting of security fuse disabled. Use '--allowsecfuse' to set security fuse.\n");
          arguments.fuse |= 1;
        }

        printf("Writing IC Fuse... (0x%04X) ", arguments.fuse);

        uint8_t fusedata[] = {arguments.fuse, arguments.fuse>>8};

        uint16_t fuseaddr = icdata->codewords-1;

        if( !FPDKCOM_SetBuffer(comfd, fuseaddr*2, fusedata, sizeof(fusedata)) )
        {
          fprintf(stderr, "ERROR: Could not send data to programmer\n");
          return -14;
        }

        int r = FPDKCOM_IC_Write(comfd, icdata->id12bit, icdata->type, 
                                 icdata->vdd_cmd_write, icdata->vpp_cmd_write, icdata->vdd_write_hv, icdata->vpp_write_hv,
                                 fuseaddr, icdata->addressbits, fuseaddr, icdata->codebits, 1, 
                                 icdata->write_block_size, icdata->write_block_clock_groups, icdata->write_block_clocks_per_group);
        if( r>=FPDK_ERR_ERROR )
        {
          fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
          return -4;
        }
        if( r != icdata->id12bit )
        {
          fprintf(stderr, "ERROR: Write fuse failed.\n");
          return -15;
        }
        printf("done.\n");
      }

    }
    break;

    case 'e': //erase
    {
      if( !FPDK_IS_FLASH_TYPE(icdata->type) )
      {
        fprintf(stderr, "ERROR: Only FLASH type IC can get erased\n");
        return -18;
      }
      if( !icdata->vdd_cmd_erase  || !icdata->vpp_cmd_erase || !icdata->vdd_erase_hv )
      {
        fprintf(stderr, "Erase for this IC not implemented yet.\n");
        return -20;
      }

      printf("Erasing IC... ");
      int r = FPDKCOM_IC_Erase(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_erase, icdata->vpp_cmd_erase, icdata->vdd_erase_hv, icdata->vpp_erase_hv, icdata->erase_clocks );
      if( r>=FPDK_ERR_ERROR )
      {
        fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
        return -4;
      }
      else
      if( r != icdata->id12bit )
      {
        fprintf(stderr, "ERROR: Erasing IC failed.\n");
        return -19;
      }
      else
      {
        printf("done.\n");

        if( !arguments.noblankcheck )
        {
          verbose_printf("Blank check IC... ");
          int r = FPDKCOM_IC_BlankCheck(comfd, icdata->id12bit, icdata->type, icdata->vdd_cmd_read, icdata->vpp_cmd_read, icdata->vpp_cmd_read, icdata->vdd_read_hv, icdata->addressbits, icdata->codebits, icdata->codewords, icdata->exclude_code_first_instr, icdata->exclude_code_start, icdata->exclude_code_end);
          if( r>=FPDK_ERR_ERROR )
          {
            fprintf(stderr, "FPDK_ERROR: %s\n",FPDK_ERR_MSG[r&0x000F]);
            return -4;
          }
          if( r != icdata->id12bit )
          {
            fprintf(stderr, "ERROR: Blank check IC failed.\n");
            return -21;
          }
          verbose_printf("done.\n");
        }
      }
    }
    break;

    case 's':
    {
      printf("Running IC (%.2fV)... ", arguments.runvdd);
      if( !FPDKCOM_IC_StartExecution(comfd, arguments.runvdd) )
      {
        fprintf(stderr, "ERROR: Could not start IC.\n");
        return -22;
      }
      printf("IC started, press [Esc] to stop.\n");

      for( ;; )
      {
        fpdkutil_waitfdorkeypress(comfd, 1000);
        unsigned char dbgmsg[256] = {0};
        int dbgd = FPDKCOM_IC_ReceiveDebugData(comfd, dbgmsg, 255);
        if( dbgd>0 )
        {
          dbgmsg[dbgd]=0;
          printf("%s", dbgmsg);
        }
        int c = fpdkutil_getchar();
        if( 27 == c )
          break;
        if( -1 != c )
          FPDKCOM_IC_SendDebugData(comfd, (uint8_t*)&c, 1);
      }

      if( FPDKCOM_IC_StopExecution(comfd) )
        printf("\nIC stopped\n");
    }
    break;

  }

  return 0;
}
