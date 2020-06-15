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
#ifndef __FPDKICCALIB_H__
#define __FPDKICCALIB_H__

#include <stdint.h>
#include <stdbool.h>

#include "fpdkicdata.h"

typedef enum FPDKCALIBTYPE
{
  FPDKCALIB_IHRC  = 1,
  FPDKCALIB_ILRC  = 2,
  FPDKCALIB_BG    = 3,
  FPDKCALIB_IHRC0 = 4,

} FPDKCALIBTYPE;

typedef struct FPDKCALIBDATA
{
  FPDKCALIBTYPE type;
  uint8_t       algo;
  uint32_t      loopcycles;
  uint16_t      pos;
  uint32_t      frequency;
  uint32_t      millivolt;

} FPDKCALIBDATA;

bool FPDKCALIB_InsertCalibration(const FPDKICDATA* icdata, uint8_t* code, const uint16_t len, FPDKCALIBDATA* calibdata);
bool FPDKCALIB_RemoveCalibration(const FPDKCALIBDATA* calibdata, uint8_t* code, const uint8_t val);

#endif //__FPDKICCALIB_H__
