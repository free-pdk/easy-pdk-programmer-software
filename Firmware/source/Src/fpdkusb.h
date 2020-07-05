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

#ifndef __FPDKUSB_H_
#define __FPDKUSB_H_

#include <stdbool.h>
#include <stdint.h>

#include "fpdk.h"
#include "fpdkproto.h"

void FPDKUSB_Init(void);
void FPDKUSB_DeInit(void);

bool FPDKUSB_IsConnected(void);

void FPDKUSB_USBSignalPortOpenClose(void);
bool FPDKUSB_USBHandleReceive(const uint8_t* dat, const uint32_t len);

void FPDKUSB_HandleCommands(void);

void FPDKUSB_SendDebug(const uint8_t* dat, const uint32_t len);

#endif //__FPDKUSB_H_
