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
#include "fpdkutil.h"
#include "fpdkproto.h"

#include <stdarg.h>
#include <stdio.h>

const char FPDK_ERR_MSG[16][64] = {
  "?", //0xFFF0
  "?", //0xFFF1
  "?", //0xFFF2
  "?", //0xFFF3
  "?", //0xFFF4
  "?", //0xFFF5
  "?", //0xFFF6
  "?", //0xFFF7
  "?", //0xFFF8
  "?", //0xFFF9
  "chip is not blank",                //0xFFFA
  "verify failed",                    //0xFFFB
  "command ack failed / wrong icid",  //0xFFFC
  "setup high vpp/high vdd failed",   //0xFFFD
  "setup vpp/vdd failed",             //0xFFFE
  "unknown"                           //0xFFFF
};

static int _verbose = 0;

void verbose_set(int v)
{
  _verbose = v;
}

int verbose_printf(char *format, ...)
{
  if( !_verbose )
    return 0;
  va_list args;
  va_start(args, format);
  int r = vprintf(format, args);
  va_end(args);
  return r;
}

#if defined(__unix__) || defined(__unix) || defined(__APPLE__) && defined(__MACH__)
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int xmode=0;
struct termios orig_termios;
int orig_f;

void fpdkutil_stdin_rawnb_disable(void)
{
  if( xmode )
  {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    fcntl(STDIN_FILENO, F_SETFL, orig_f);
  }
  xmode = 0;
}

void fpdkutil_stdin_rawnb_enable(void)
{
  tcgetattr(STDIN_FILENO, &orig_termios);
  orig_f = fcntl(STDIN_FILENO, F_GETFL, 0);

  atexit(fpdkutil_stdin_rawnb_disable);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ECHONL | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  fcntl(STDIN_FILENO, F_SETFL, orig_f | O_NONBLOCK);
  xmode = 1;
}

int fpdkutil_getchar(void)
{
  if( !xmode )
    fpdkutil_stdin_rawnb_enable();
  int ch = getchar();
  if( (27 == ch) && ( -1 != getchar()) ) //filter arrow and function keys
  {
    getchar();
    ch = -1;
  }
  return ch;
}

unsigned long fpdkutil_getTickCount(void)
{
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  return( (spec.tv_sec*1000) + (spec.tv_nsec / 1000000) );
}

void fpdkutil_waitfdorkeypress(const int fd, const int timeout)
{
  struct pollfd fds[2] = {{.fd=fd, .events=POLLIN}, {.fd=0, .events=POLLIN} };
  poll(fds, 2, timeout);
}

#elif defined(_WIN32)
#include <windows.h>
#include <conio.h>
int fpdkutil_getchar(void)
{
/*
  if( WAIT_OBJECT_0 == WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), 0) )
  {
    INPUT_RECORD ir;
    DWORD inpread;
    ReadConsoleInput( GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &inpread );
    if( (KEY_EVENT == ir.EventType) && (ir.Event.KeyEvent.bKeyDown) )
      return ir.Event.KeyEvent.uChar.AsciiChar;
  }
*/
  if( _kbhit() )
     return _getch(); //!!!

  return -1;
}

unsigned long fpdkutil_getTickCount(void)
{
  return GetTickCount();
}

void fpdkutil_waitfdorkeypress(const int fd, const int timeout)
{
  DWORD dwEndTick = GetTickCount() + timeout;

  for( ;GetTickCount()<dwEndTick; )
  {
    DWORD dwError;
    COMSTAT stat;
    ClearCommError( (HANDLE)fd, &dwError, &stat );
    if( stat.cbInQue )
      return;

    if( _kbhit() )
      return;

    Sleep(1);
  }
}

#else
#error unknown OS (not unix or windows)
#endif
