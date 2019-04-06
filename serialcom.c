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
#include "serialcom.h"

#if defined(__unix__) || defined(__unix) || defined(__APPLE__) && defined(__MACH__)
#define _DARWIN_C_SOURCE
#include <unistd.h>
#ifndef __USE_MISC
#define __USE_MISC
#endif
#include <termios.h>
#include <fcntl.h>
#include <string.h>

int serialcom_open(const char* devpath)
{
  int fd = open(devpath, O_RDWR | O_NOCTTY | O_NDELAY);
  if( fd<0 )
    return fd;

  struct termios config;
  memset(&config, 0, sizeof(config));
  tcgetattr(fd, &config);
  config.c_iflag &= ~(INLCR | ICRNL);
  config.c_iflag |= IGNPAR | IGNBRK;
  config.c_oflag &= ~(OPOST | ONLCR | OCRNL);
  config.c_cflag &= ~(PARENB | PARODD | CSTOPB | CSIZE | CRTSCTS);
  config.c_cflag |= CLOCAL | CREAD | CS8;
  config.c_lflag &= ~(ICANON | ISIG | ECHO);
  int baud = B115200;
  cfsetospeed(&config, baud);
  cfsetispeed(&config, baud);
  config.c_cc[VTIME] = 1;
  config.c_cc[VMIN]  = 0;
  if( tcsetattr(fd, TCSANOW, &config)<0 )
  {
    close(fd);
    return -1;
  }
  return fd;
}

int serialcom_close(const int fd)
{
  tcdrain(fd);
  return close(fd);
}

int serialcom_write(const int fd, uint8_t* buf, const size_t len)
{
  return write( fd, buf, len );
}

int serialcom_read(const int fd, uint8_t* buf, const size_t len)
{
  return read( fd, buf, len );
}

#elif defined(_WIN32)
#include <windows.h>

int serialcom_open(const char* devpath)
{
  DCB config;
  COMMTIMEOUTS timeouts;

  HANDLE fd = CreateFileA(devpath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
  if( INVALID_HANDLE_VALUE == fd )
    return -1;

  SetupComm(fd, 3+65536, 2+256);
  timeouts.ReadIntervalTimeout = MAXDWORD;
  timeouts.ReadTotalTimeoutConstant = 0;
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant = 0;
  SetCommTimeouts(fd, &timeouts);

  GetCommState(fd, &config);
  config.BaudRate = 115200;
  config.fBinary = TRUE;
  config.fParity = 0;
  config.fErrorChar = 0;
  config.fNull = 0;
  config.fAbortOnError = 0;
  config.ByteSize = 8;
  config.Parity = 0;
  config.StopBits = 0;
  config.EvtChar = '\n';

  if( SetCommState(fd, &config) == 0 ) {
    CloseHandle(fd);
    return -1;
  }

  EscapeCommFunction(fd, SETDTR);

  return (int)fd;
}

int serialcom_close(const int fd)
{
  return CloseHandle((HANDLE)fd);
}

int serialcom_write(const int fd, uint8_t* buf, const size_t len)
{
  DWORD written = 0;
  if( !WriteFile((HANDLE)fd, buf, len, &written, NULL) )
    return -1;
  return written;
}

int serialcom_read(const int fd, uint8_t* buf, const size_t len)
{
  DWORD readbytes = 0;
  if( !ReadFile((HANDLE)fd, buf, len, &readbytes, NULL) )
    return -1;
  return readbytes;
}

#else
#error Unknown OS (not Unix or Windows)
#endif
