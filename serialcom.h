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
#ifndef __SERIAL_COM_H
#define __SERIAL_COM_H

#include <stdint.h>
#include <stdlib.h>

int serialcom_open(const char* devpath);
int serialcom_close(const int fd);
int serialcom_write(const int fd, uint8_t* buf, const size_t len);
int serialcom_read(const int fd, uint8_t* buf, const size_t len);

#endif // __SERIAL_COM_H
