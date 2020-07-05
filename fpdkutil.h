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
#ifndef __FPDKUTIL_H_
#define __FPDKUTIL_H_

#define _POSIX_C_SOURCE 200809L

#include <stdint.h>

extern const char FPDK_ERR_MSG[16][64];

void verbose_set(int v);
int  verbose_printf(char *format, ...);

void          fpdkutil_waitfdorkeypress(const int fd, const int timeout);
int           fpdkutil_getchar(void);
unsigned long fpdkutil_getTickCount(void);

//DEL void          fpdkutil_usleep(int64_t usec);

#endif //__FPDKUTIL_H_
