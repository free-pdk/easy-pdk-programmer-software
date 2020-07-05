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
#ifndef __FPDKIHEX8_H_
#define __FPDKIHEX8_H_

#include <stdint.h>

int FPDKIHEX8_ReadFile(const char* filename, uint16_t* datout, const uint16_t datlen);
int FPDKIHEX8_WriteFile(const char* filename, const uint8_t* datin, const uint16_t datlen);

#endif //__FPDKIHEX8_H_
