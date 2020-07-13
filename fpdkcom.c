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
#include "fpdkcom.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "serialcom.h"
#include "fpdkproto.h"
#include "fpdkutil.h"

static const char FPDK_VERSCAN[] = "FREE-PDK EASY PROG - HW:%u.%u SW:%u.%u PROTO:%u.%u\n";

#define FPDKCOM_CMDRSP_TIMEOUT              50
#define FPDKCOM_CMDRSP_PROBEIC_TIMEOUT      3500
#define FPDKCOM_CMDRSP_READIC_TIMEOUT       1000
#define FPDKCOM_CMDRSP_ERASE_TIMEOUT        2000
#define FPDKCOM_CMDRSP_WRITE_TIMEOUT        2000
#define FPDKCOM_CMDRSP_CALIBRATEIC_TIMEOUT  10000

static bool _FPDKCOM_SendCommand(const int fd, const FPDKPROTO_CMD cmd, const uint8_t* dat, const uint8_t len)
{
  uint8_t scmd[2+256] = {cmd,len};
  if( len )
    memcpy(&scmd[2], dat, len);

  return( (2+len) == serialcom_write(fd, scmd, 2+len) );
}

static int _FPDKCOM_ReceiveResponse(const int fd, uint8_t* rsp, const uint32_t len, const uint32_t timeout)
{
  unsigned long timeouttick = fpdkutil_getTickCount() + timeout;
  uint32_t rcvlen = 0;
  for( ;rcvlen<len; )
  {
    if( sizeof(uint8_t) == serialcom_read(fd, &rsp[rcvlen], sizeof(uint8_t)) )
    {
      rcvlen++;
      if( rcvlen>=3 )
      {
        uint32_t plen = rsp[1] | (((uint32_t)rsp[2])<<8);
        if( rcvlen>=(3+plen) )
          return rcvlen;
      }
    }
    if( fpdkutil_getTickCount()>timeouttick )
      break;
  }
  return -1;
}

static int _FPDKCOM_SendReceiveCommandWithTimeout(const int fd,
                                                  const FPDKPROTO_CMD cmd, const uint8_t* datin, const uint8_t lenin,
                                                  uint8_t* datout, const uint16_t lenout,
                                                  const uint32_t timeout
                                                 )
{
  if( !_FPDKCOM_SendCommand(fd, cmd, datin, lenin) )
    return -1;

  uint8_t buf[3];
  uint8_t* resp = buf;
  uint16_t rlen = sizeof(buf);

  if( datout && lenout)
  {
    resp = datout;
    rlen = lenout;
  }

  int resplen = _FPDKCOM_ReceiveResponse(fd, resp, rlen, timeout);
  if( resplen < 3 )
    return -2;

  if( FPDKPROTO_RSP_ACK != resp[0] )
    return -3;

  return resplen;
}

static int _FPDKCOM_SendReceiveCommand(const int fd,
                                       const FPDKPROTO_CMD cmd, const uint8_t* datin, const uint8_t lenin,
                                       uint8_t* datout, const uint16_t lenout
                                      )
{
  return _FPDKCOM_SendReceiveCommandWithTimeout(fd, cmd, datin, lenin, datout, lenout, FPDKCOM_CMDRSP_TIMEOUT);
}

int FPDKCOM_OpenAuto(char portpath[64])
{
  for( int i=0; i<999; i++ )
  {
#if defined(__unix__) || defined(__unix)
  #if defined(__MSYS__)
    sprintf( portpath, "/dev/ttyS%d", i );
  #else
    sprintf( portpath, "/dev/ttyACM%d", i );
  #endif
#elif defined(__APPLE__) && defined(__MACH__)
    if( 0==i )
      sprintf( portpath, "/dev/tty.usbmodem1234567855AA" );
    else
      sprintf( portpath, "/dev/tty.usbmodem1234567855AA%d", i );
#elif defined(_WIN32)
    if( (i+1)<10 )
      sprintf( portpath, "COM%d", i+1 );
    else
      sprintf( portpath, "\\\\.\\COM%d", i+1 );
#else
#error Unknown architecture (not unix, apple, windows mingw, windows msys2)
#endif
    int fd = FPDKCOM_Open( portpath );
    if( (fd>=0) || (-3==fd) )
      return fd;
  }
  return -1;
}

int FPDKCOM_Open(const char* devname)
{
  int fd = serialcom_open(devname);
  if( fd<0 )
    return -1;

  uint8_t dummy[1024];
  while( serialcom_read(fd,dummy,sizeof(dummy))>0 ) {;}

  unsigned int hw_major, hw_minor, sw_major, sw_minor, proto_major, proto_minor;
  if( !FPDKCOM_GetVersion(fd, &hw_major, &hw_minor, &sw_major, &sw_minor, &proto_major, &proto_minor, NULL, 0) )
  {
    serialcom_close(fd);
    return -2;
  }
  if( (FPDKPROTO_MAJOR != proto_major) || (FPDKPROTO_MINOR != proto_minor) )
  {
    serialcom_close(fd);
    return -3;
  }

  return fd;
}

int FPDKCOM_Close(const int fd)
{
  return serialcom_close(fd);
}

////
////////
////

bool FPDKCOM_GetVersion(const int fd, 
                        unsigned int *hw_major, unsigned int *hw_minor, 
                        unsigned int *sw_major, unsigned int *sw_minor, 
                        unsigned int *proto_major, unsigned int *proto_minor,
                        char* fwstr, const size_t fwstrlen )
{
  uint8_t resp[3+128+1];
  memset(resp, 0, sizeof(resp));

  if( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_GETVERINFO, 0, 0, resp, sizeof(resp)-1) < 0 )
    return false;

  if( fwstr )
    strncpy( fwstr, (char*)&resp[3], fwstrlen );

  if( 6 != sscanf( (char*)&resp[3], FPDK_VERSCAN, hw_major, hw_minor, sw_major, sw_minor, proto_major, proto_minor ) )
    return false;

  return true;
}

bool FPDKCOM_SetLed(const int fd, const uint8_t ledbits)
{
  return( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_SETLED, &ledbits, sizeof(ledbits), 0, 0) > 0 );
}

bool FPDKCOM_GetButtonState(const int fd, bool* buttonstate)
{
  uint8_t resp[3+1];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_GETBUTTON, 0,0, resp, sizeof(resp)) )
    return false;

  *buttonstate = resp[3];
  return true;
}

bool FPDKCOM_SetOutputVoltages(const int fd, const float vdd, const float vpp)
{
  uint32_t vdd_u = vdd*1000;
  uint32_t vpp_u = vpp*1000;
  uint8_t dat[] = {vdd_u,vdd_u>>8,vdd_u>>16,vdd_u>>24, vpp_u,vpp_u>>8,vpp_u>>16,vpp_u>>24};
  return( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_SETVOLTOUT, (uint8_t*)dat, sizeof(dat), 0, 0) > 0 );
}

bool FPDKCOM_MeasureOutputVoltages(const int fd, float* vdd, float* vpp, float* vref)
{
  uint8_t resp[3+3*sizeof(uint32_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_GETVOLTAGES, 0,0, resp, sizeof(resp)) )
    return false;

  if( vref )
    *vref = ((float)(resp[ 3] | (((uint32_t)resp[ 4])<<8) | (((uint32_t)resp[ 5])<<16) | (((uint32_t)resp[ 6])<<24))) / 1000;

  if( vdd )
    *vdd  = ((float)(resp[ 7] | (((uint32_t)resp[ 8])<<8) | (((uint32_t)resp[ 9])<<16) | (((uint32_t)resp[10])<<24))) / 1000;

  if( vpp )
    *vpp  = ((float)(resp[11] | (((uint32_t)resp[12])<<8) | (((uint32_t)resp[13])<<16) | (((uint32_t)resp[14])<<24))) / 1000;

  return true;
}

bool FPDKCOM_SetBuffer(const int fd, const uint16_t woffset, const uint8_t* dat, const uint16_t len)
{
  for( uint16_t p=0; p<len; )
  {
    uint8_t cdata[254] = { (p+woffset)&0xFF, (p+woffset)>>8 };

    uint16_t slen = len;
    if( slen>(sizeof(cdata)-sizeof(uint16_t)) )
      slen = sizeof(cdata)-sizeof(uint16_t);
   
    memcpy( &cdata[2], dat+p, slen );
    
    if( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_SETBUF, cdata, sizeof(uint16_t)+slen, 0, 0) < 0 )
      return false;

    p+=slen;
  }
  return true;
}

int FPDKCOM_GetBuffer(const int fd, const uint16_t roffset, uint8_t* dat, const uint16_t len)
{
  uint16_t p;
  for( p=0; p<len; )
  {
    uint16_t rlen = len;
#if defined(_WIN32)
    if( rlen>63 ) rlen = 63; //work around for windows serial receive problems
#endif
    uint8_t cdata[] = { (p+roffset)&0xFF, (p+roffset)>>8, rlen&0xFF, rlen>>8 };
    uint8_t resp[3 + 0x1000*sizeof(uint16_t)];
    if( rlen>(sizeof(resp)-3) )
      return -1;
    if( rlen != (_FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_GETBUF, cdata, sizeof(cdata), resp, 3+rlen)-3) )
      return -2;
    memcpy( dat+p, &resp[3], rlen );
    p += rlen;
  }
  return(len);
}

int FPDKCOM_IC_Probe(const int fd, float* vpp_found, float* vdd_found, FPDKICTYPE* type)
{
  uint8_t resp[3+4*sizeof(uint32_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_PROBEIC, 0,0, resp, sizeof(resp), FPDKCOM_CMDRSP_PROBEIC_TIMEOUT) )
    return -1;

  uint32_t icid = resp[3] | (((uint32_t)resp[4])<<8) | (((uint32_t)resp[5])<<16) | (((uint32_t)resp[6])<<24);
  if( icid )
  {
    uint32_t vpp = resp[ 7] | (((uint32_t)resp[ 8])<<8) | (((uint32_t)resp[ 9])<<16) | (((uint32_t)resp[10])<<24);
    *vpp_found = vpp/1000.0;
    uint32_t vdd = resp[11] | (((uint32_t)resp[12])<<8) | (((uint32_t)resp[13])<<16) | (((uint32_t)resp[14])<<24);
    *vdd_found = vdd/1000.0;
    *type = resp[15];
  }
  return( icid );
}

int FPDKCOM_IC_BlankCheck(const int fd,
                          const uint16_t icid, const FPDKICTYPE type,
                          const float vdd_cmd, const float vpp_cmd,
                          const float vdd_read, const float vpp_read,
                          const uint8_t addr_bits, const uint8_t data_bits,
                          const uint16_t count,
                          const bool exclude_first_instruction, const uint16_t exclude_start, const uint16_t exclude_end)
{
  uint32_t vdd_cmd_u  = vdd_cmd*1000;
  uint32_t vpp_cmd_u  = vpp_cmd*1000;
  uint32_t vdd_read_u = vdd_read*1000;
  uint32_t vpp_read_u = vpp_read*1000;

  uint8_t dat[] = { icid,icid>>8, type,
                    vdd_cmd_u,vdd_cmd_u>>8,vdd_cmd_u>>16,vdd_cmd_u>>24, vpp_cmd_u,vpp_cmd_u>>8, vpp_cmd_u>>16,vpp_cmd_u>>24,
                    vdd_read_u,vdd_cmd_u>>8,vdd_read_u>>16,vdd_read_u>>24, vpp_read_u,vpp_read_u>>8, vpp_read_u>>16,vpp_read_u>>24,
                    addr_bits, data_bits, count,count>>8, 
                    exclude_first_instruction, exclude_start,exclude_start>>8, exclude_end,exclude_end>>8 };

  uint8_t resp[3+sizeof(uint16_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_BLANKCKIC, dat,sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_READIC_TIMEOUT) )
    return -1;

  return( resp[3] | (((int)resp[4])<<8) );
}

int FPDKCOM_IC_Erase(const int fd,
                     const uint16_t icid, const FPDKICTYPE type,
                     const float vdd_cmd, const float vpp_cmd,
                     const float vdd_erase, const float vpp_erase,
                     const uint8_t erase_clocks)
{
  uint32_t vdd_cmd_u = vdd_cmd*1000;
  uint32_t vpp_cmd_u = vpp_cmd*1000;
  uint32_t vdd_erase_u = vdd_erase*1000;
  uint32_t vpp_erase_u = vpp_erase*1000;

  uint8_t dat[] = { icid,icid>>8, type,
                    vdd_cmd_u,vdd_cmd_u>>8,vdd_cmd_u>>16,vdd_cmd_u>>24, vpp_cmd_u,vpp_cmd_u>>8, vpp_cmd_u>>16,vpp_cmd_u>>24,
                    vdd_erase_u,vdd_erase_u>>8,vdd_erase_u>>16,vdd_erase_u>>24, vpp_erase_u,vpp_erase_u>>8, vpp_erase_u>>16,vpp_erase_u>>24,
                    erase_clocks };

  uint8_t resp[3+sizeof(uint16_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_ERASEIC, dat,sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_ERASE_TIMEOUT) )
    return -1;

  return( resp[3] | (((int)resp[4])<<8) );
}

int FPDKCOM_IC_Read(const int fd,
                    const uint16_t icid, const FPDKICTYPE type,
                    const float vdd_cmd, const float vpp_cmd,
                    const float vdd_read, const float vpp_read,
                    const uint16_t addr, const uint8_t addr_bits,
                    const uint16_t data_offs, const uint8_t data_bits,
                    const uint16_t count)
{
  uint32_t vdd_cmd_u  = vdd_cmd*1000;
  uint32_t vpp_cmd_u  = vpp_cmd*1000;
  uint32_t vdd_read_u = vdd_read*1000;
  uint32_t vpp_read_u = vpp_read*1000;

  uint8_t dat[] = { icid,icid>>8, type,
                    vdd_cmd_u,vdd_cmd_u>>8,vdd_cmd_u>>16,vdd_cmd_u>>24, vpp_cmd_u,vpp_cmd_u>>8, vpp_cmd_u>>16,vpp_cmd_u>>24,
                    vdd_read_u,vdd_cmd_u>>8,vdd_read_u>>16,vdd_read_u>>24, vpp_read_u,vpp_read_u>>8, vpp_read_u>>16,vpp_read_u>>24,
                    addr,addr>>8, addr_bits,
                    data_offs,data_offs>>8, data_bits,
                    count,count>>8 };

  uint8_t resp[3+sizeof(uint16_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_READIC, dat,sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_READIC_TIMEOUT) )
    return -1;

  return( resp[3] | (((int)resp[4])<<8) );
}

int FPDKCOM_IC_Write(const int fd,
                     const uint16_t icid, const FPDKICTYPE type,
                     const float vdd_cmd, const float vpp_cmd,
                     const float vdd_write, const float vpp_write,
                     const uint16_t addr, const uint8_t addr_bits,
                     const uint16_t data_offs, const uint8_t data_bits,
                     const uint16_t count, 
                     const uint8_t write_block_size, const uint8_t write_block_clock_groups, const uint8_t write_block_clocks_per_group)
{
  uint32_t vdd_cmd_u = vdd_cmd*1000;
  uint32_t vpp_cmd_u = vpp_cmd*1000;
  uint32_t vdd_write_u = vdd_write*1000;
  uint32_t vpp_write_u = vpp_write*1000;

  uint8_t dat[] = { icid,icid>>8, type,
                    vdd_cmd_u,vdd_cmd_u>>8,vdd_cmd_u>>16,vdd_cmd_u>>24, vpp_cmd_u,vpp_cmd_u>>8, vpp_cmd_u>>16,vpp_cmd_u>>24,
                    vdd_write_u,vdd_write_u>>8,vdd_write_u>>16,vdd_write_u>>24, vpp_write_u,vpp_write_u>>8, vpp_write_u>>16,vpp_write_u>>24,
                    addr,addr>>8, addr_bits,
                    data_offs,data_offs>>8, data_bits,
                    count,count>>8, write_block_size, write_block_clock_groups, write_block_clocks_per_group };

  uint8_t resp[3+sizeof(uint16_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_WRITEIC, dat,sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_READIC_TIMEOUT) )
    return -1;

  return( resp[3] | (((int)resp[4])<<8) );
}

int FPDKCOM_IC_Verify(const int fd,
                      const uint16_t icid, const FPDKICTYPE type,
                      const float vdd_cmd, const float vpp_cmd,
                      const float vdd_read, const float vpp_read,
                      const uint16_t addr, const uint8_t addr_bits,
                      const uint16_t data_offs, const uint8_t data_bits,
                      const uint16_t count,
                      const bool exclude_first_instruction, const uint16_t exclude_start, const uint16_t exclude_end)
{
  uint32_t vdd_cmd_u  = vdd_cmd*1000;
  uint32_t vpp_cmd_u  = vpp_cmd*1000;
  uint32_t vdd_read_u = vdd_read*1000;
  uint32_t vpp_read_u = vpp_read*1000;

  uint8_t dat[] = { icid,icid>>8, type,
                    vdd_cmd_u,vdd_cmd_u>>8,vdd_cmd_u>>16,vdd_cmd_u>>24, vpp_cmd_u,vpp_cmd_u>>8, vpp_cmd_u>>16,vpp_cmd_u>>24,
                    vdd_read_u,vdd_cmd_u>>8,vdd_read_u>>16,vdd_read_u>>24, vpp_read_u,vpp_read_u>>8, vpp_read_u>>16,vpp_read_u>>24,
                    addr,addr>>8, addr_bits,
                    data_offs,data_offs>>8, data_bits,
                    count,count>>8,
                    exclude_first_instruction, exclude_start,exclude_start>>8, exclude_end,exclude_end>>8 };

  uint8_t resp[3+sizeof(uint16_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_VERIFYIC, dat,sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_READIC_TIMEOUT) )
    return -1;

  return( resp[3] | (((int)resp[4])<<8) );
}

bool FPDKCOM_IC_Calibrate(const int fd, const uint32_t type, const uint32_t vdd, const uint32_t freq, const uint32_t mult,
                          uint8_t* fcalval, uint32_t* fcalfreq)
{
  uint8_t dat[] = {type,type>>8,type>>16,type>>24, vdd,vdd>>8,vdd>>16,vdd>>24, 
                   freq,freq>>8,freq>>16,freq>>24, mult,mult>>8,mult>>16,mult>>24};

  uint8_t resp[3+2*sizeof(uint32_t)];
  if( sizeof(resp) != _FPDKCOM_SendReceiveCommandWithTimeout(fd, FPDKPROTO_CMD_CALIBRATEIC, (uint8_t*)dat, sizeof(dat), resp, sizeof(resp), FPDKCOM_CMDRSP_CALIBRATEIC_TIMEOUT) )
    return false;

  *fcalval = resp[3];
  *fcalfreq = resp[ 7] | (((uint32_t)resp[ 8])<<8) | (((uint32_t)resp[ 9])<<16) | (((uint32_t)resp[10])<<24);

  return true;
}

bool FPDKCOM_IC_StartExecution(const int fd, const float vdd)
{
  uint32_t vdd_u = vdd*1000;
  uint8_t dat[] = {vdd_u,vdd_u>>8,vdd_u>>16,vdd_u>>24};
  return( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_EXECUTEIC, (uint8_t*)dat, sizeof(dat), 0, 0) > 0);
}

bool FPDKCOM_IC_StopExecution(const int fd)
{
  return( _FPDKCOM_SendReceiveCommand(fd, FPDKPROTO_CMD_STOPIC, 0, 0, 0, 0) > 0 );
}

int FPDKCOM_IC_ReceiveDebugData(const int fd, uint8_t* dat, const uint8_t len)
{
  uint8_t resp[3+256];
  int resplen = _FPDKCOM_ReceiveResponse(fd, resp, 3+((uint32_t)len), 1);

  if( 0 == resplen )
    return 0;

  if( resplen < 3 )
    return -1;

  if( FPDKPROTO_RSP_DBGDAT != resp[0] )
    return -2;

  memcpy( dat, &resp[3], resplen-3 );

  return resplen-3;
}

bool FPDKCOM_IC_SendDebugData(const int fd, const uint8_t* dat, const uint8_t len)
{
  return _FPDKCOM_SendCommand(fd, FPDKPROTO_CMD_DBGDAT, dat, len);
}
