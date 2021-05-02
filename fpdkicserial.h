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
#ifndef __FPDKICSERIAL_H__
#define __FPDKICSERIAL_H__

#include <stdint.h>
#include <stdbool.h>

#include "fpdkicdata.h"

int FPDKSERIAL_InsertSerial(const FPDKICDATA* icdata, uint8_t* code, const uint16_t len, uint64_t serial);

#endif //__FPDKICSERIAL_H__
