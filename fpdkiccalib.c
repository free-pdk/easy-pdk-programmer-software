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

#include "fpdkiccalib.h"

#define FPDKCALIBCP_MAXWORDS 32

#define CO_REPL        0x8000
#define CO_IGNR        0x4000

#define CO_BC_FIXUP    0x1000  //Before Calibration FIXUP                   (add insert position to opcode)

#define CO_AC_SETIMM   0x0200  //After Calibration SET IMMediate from val 2 (set lower part to value supplied to remove function, e.g. the calibration result)
#define CO_AC_NOP      0x0100  //After Calibration NOP                      (replace the calibration opcode with a NOP after calibration is done)

#define CO_VAL_FREQ_00 0x0001
#define CO_VAL_FREQ_08 0x0002
#define CO_VAL_FREQ_16 0x0003
#define CO_VAL_FREQ_24 0x0004

#define CO_VAL_MVOL_00 0x0010
#define CO_VAL_MVOL_08 0x0011

typedef struct FPDKCALIBCP
{
  uint16_t sopc; //search opcode
  uint16_t smsk; //serach mask
  uint16_t copc; //calibration opcode
  uint16_t cocf; //calibartion opocde flags

} FPDKCALIBCP;


typedef struct FPDKCALIBALGO
{
  FPDKCALIBTYPE type;
  uint8_t codebits;
  uint32_t loopcycles;
  FPDKCALIBCP algo[FPDKCALIBCP_MAXWORDS];

} FPDKCALIBALGO;


static const FPDKCALIBALGO fpdk_calib_algos[] = { 
 {
  .type=FPDKCALIB_IHRC,
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0x1448, .smsk=0xFFFF, .copc=0x0F71, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'H'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x1438, .smsk=0xFFFF, .copc=0x17FF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '8'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0F70, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x008B, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x0B), A  (IHRCR)   /   after-calib: stay same
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0C90, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1001, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0E70, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1802, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x002       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_ILRC,
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0x144C, .smsk=0xFFFF, .copc=0x0F71, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'L'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x1438, .smsk=0xFFFF, .copc=0x17FF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '8'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0F70, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x009F, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x1F), A  (ILRCR)   /   after-calib: stay same
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0C90, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1001, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0E70, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1802, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x002       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_BG,
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0x1442, .smsk=0xFFFF, .copc=0x1709, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x1431, .smsk=0xFFFF, .copc=0x0091, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '1'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x1439, .smsk=0xFFFF, .copc=0x1784, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '9'            /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x009A, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x1A), A  (GPCC)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x17A3, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x009E, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x1E), A  (GPCS)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x17FF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, 0              /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0F70, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0099, .cocf=CO_REPL},                                          //search: AND A, 0              /   calib: MOV IO(0x19), A  (BGTR)    /   after-calib: stay
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0C90, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x1001, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0E70, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x1807, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x007       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_IHRC,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C48, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'H'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C38, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '8'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x018B, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x0B), A  (IHRCR)   /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x3002, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x002       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_IHRC0,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C48, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'H'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C31, .smsk=0xFFFF, .copc=0x1F0D, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '1'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x2C30, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '0'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x018B, .cocf=CO_REPL|CO_VAL_FREQ_16},                           //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x0B), A  (IHRCR)   /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_08)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x3004, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x004       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_ILRC,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C4C, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'L'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C38, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '8'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x01B9, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x39), A  (ILRCR)   /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x3002, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x002       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_ILRC,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C6C, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'l'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C38, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, '8'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x01BB, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x3B), A  (ILRCR)   /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x3002, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x002       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_ILRC,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C4C, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'L'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C33, .smsk=0xFFFF, .copc=0x1F0D, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '3'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x2C42, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, 'B'            /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x01BB, .cocf=CO_REPL|CO_VAL_FREQ_08},                           //search: AND A, *   (FREQ_08)  /   calib: MOV IO(0x3B), A  (ILRCR)   /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_16},                 //search: AND A, *   (FREQ_16)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x3003, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP|CO_VAL_MVOL_08},     //search: AND A, *   (MVOL_08)  /   calib: GOTO 0x003       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_BG,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C42, .smsk=0xFFFF, .copc=0x2F09, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x2C31, .smsk=0xFFFF, .copc=0x0191, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '1'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x2C41, .smsk=0xFFFF, .copc=0x2F84, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'A'            /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x0198, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x18), A  (GPCC)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2FA3, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x0199, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x19), A  (GPCS)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, 0              /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x019A, .cocf=CO_REPL},                                          //search: AND A, 0              /   calib: MOV IO(0x1A), A  (BGTR)    /   after-calib: stay
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x3007, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x007       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_BG,
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C42, .smsk=0xFFFF, .copc=0x2F09, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x2C33, .smsk=0xFFFF, .copc=0x0191, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '3'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x2C43, .smsk=0xFFFF, .copc=0x2F84, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'C'            /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x01AB, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x2B), A  (GPCC)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2FA3, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x01AC, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x2C), A  (GPCS)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2FFF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, 0              /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1ED0, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x01BC, .cocf=CO_REPL},                                          //search: AND A, 0              /   calib: MOV IO(0x3C), A  (BGTR)    /   after-calib: stay
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1910, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2801, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1CD0, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x3007, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x007       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_IHRC,
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0x5448, .smsk=0xFFFF, .copc=0x3D91, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'H'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x5439, .smsk=0xFFFF, .copc=0x3E0D, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '9'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x57FF, .cocf=CO_REPL|CO_AC_SETIMM|CO_VAL_FREQ_00},              //search: AND A, *   (FREQ_00)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3D90, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x010B, .cocf=CO_REPL|CO_VAL_FREQ_16},                           //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x0B), A  (IHRCR)   /   after-calib: stay
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3210, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x5001, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3990, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_08)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x6003, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x003       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_ILRC,
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0x544C, .smsk=0xFFFF, .copc=0x3D91, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'L'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x5439, .smsk=0xFFFF, .copc=0x3E0D, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '9'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x57FF, .cocf=CO_REPL|CO_AC_SETIMM|CO_VAL_FREQ_00},              //search: AND A, *   (FREQ_00)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3D90, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x0162, .cocf=CO_REPL|CO_VAL_FREQ_16},                           //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x62), A  (ILRCR)   /   after-calib: stay
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3210, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x5001, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3990, .cocf=CO_REPL|CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_08)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x6003, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x003       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .type=FPDKCALIB_BG,
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0x5442, .smsk=0xFFFF, .copc=0x5709, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x5436, .smsk=0xFFFF, .copc=0x0111, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '6'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x5433, .smsk=0xFFFF, .copc=0x3E0D, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, '3'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x5784, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x012B, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x2B), A  (GPCC)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x57A3, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x012C, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: MOV IO(0x2C), A  (GPCS)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x57FF, .cocf=CO_REPL|CO_AC_SETIMM},                             //search: AND A, 0              /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3D90, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x0163, .cocf=CO_REPL},                                          //search: AND A, 0              /   calib: MOV IO(0x63), A  (BGTR)    /   after-calib: stay
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3210, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x5001, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3990, .cocf=CO_REPL|CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x6008, .cocf=CO_REPL|CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x008       <FIXUP>   /   after-calib: NOP
  }
 },

};

static uint16_t _FPDKCALIB_GetAlgoLength(const FPDKCALIBCP* algo)
{
  uint16_t algowords;
  for( algowords=0; algowords<FPDKCALIBCP_MAXWORDS; algowords++ )
  {
    if( 0 == algo[algowords].cocf )
      break;
  }
  return algowords;
}

static uint16_t _FPDKCALIB_FindPattern(const uint16_t* code, const uint16_t codewords, const FPDKCALIBCP* algo)
{
  uint16_t algowords = _FPDKCALIB_GetAlgoLength(algo);
  for( uint16_t p=0; p<=codewords-(algowords); p++ )
  {
    bool match = true;
    for( uint16_t m=0; m<algowords; m++ )
    {
      if( (code[p+m] & algo[m].smsk) != algo[m].sopc )
      {
        match = false;
        break;
      }
    }
    if( match )
      return p;
  }
  return 0xFFFF;
}

bool FPDKCALIB_InsertCalibration(const FPDKICDATA* icdata, uint8_t* code, const uint16_t len, FPDKCALIBDATA* calibdata)
{
  uint16_t* code16 = (uint16_t*)code;
  uint16_t  len16 = len/2;

  calibdata->pos = 0xFFFF;

  //search pattern
  for( uint8_t p=0; p<sizeof(fpdk_calib_algos)/sizeof(FPDKCALIBALGO); p++ )
  {
    if( fpdk_calib_algos[p].codebits == icdata->codebits )
    {
      calibdata->pos = _FPDKCALIB_FindPattern(code16, len16, fpdk_calib_algos[p].algo);
      if( 0xFFFF != calibdata->pos )
      {
        calibdata->type = fpdk_calib_algos[p].type;
        calibdata->loopcycles = fpdk_calib_algos[p].loopcycles;
        calibdata->algo = p;
        break;
      }
    }
  }
  if( 0xFFFF == calibdata->pos )
    return false;

  uint16_t algowords = _FPDKCALIB_GetAlgoLength(fpdk_calib_algos[calibdata->algo].algo);

  calibdata->frequency = 0;
  calibdata->millivolt = 0;

  if( FPDKCALIB_BG == calibdata->type )
    calibdata->millivolt = 3950; //BG tuning uses case 3 / n=3 ==> VDD<4.0V

  //extract mvolt and freq
  for( uint16_t x=0; x<algowords; x++ )
  {
    switch(fpdk_calib_algos[calibdata->algo].algo[x].cocf & 0xFF)
    {
      case CO_VAL_FREQ_00: calibdata->frequency |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<< 0; break;
      case CO_VAL_FREQ_08: calibdata->frequency |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<< 8; break;
      case CO_VAL_FREQ_16: calibdata->frequency |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<<16; break;
      case CO_VAL_FREQ_24: calibdata->frequency |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<<24; break;
      case CO_VAL_MVOL_00: calibdata->millivolt |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<< 0; break;
      case CO_VAL_MVOL_08: calibdata->millivolt |= ((uint32_t)(code16[calibdata->pos+x]&0xFF))<< 8; break;
      default:
        break;
    }
  }

  //insert calibration code + fixup
  for( uint16_t i=0; i<algowords; i++ )
  {
    code16[calibdata->pos+i] = fpdk_calib_algos[calibdata->algo].algo[i].copc;
    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_FIXUP )
      code16[calibdata->pos+i] += calibdata->pos;
  }

  return true;
}

bool FPDKCALIB_RemoveCalibration(const FPDKCALIBDATA* calibdata, uint8_t* code, const uint8_t val)
{
  uint16_t* code16 = (uint16_t*)code;

  uint16_t algowords = _FPDKCALIB_GetAlgoLength(fpdk_calib_algos[calibdata->algo].algo);
  for( uint16_t p=0; p<algowords; p++ )
  {
    if( fpdk_calib_algos[calibdata->algo].algo[p].cocf & CO_AC_SETIMM )   //set val ?
      code16[calibdata->pos+p] = (code16[calibdata->pos+p]&0xFF00) | val; //place val in immediate value of opcode

    if( fpdk_calib_algos[calibdata->algo].algo[p].cocf & CO_AC_NOP )      //replace with NOP?
      code16[calibdata->pos+p] = 0;                                       //replace opcode with NOP
  }

  return true;
}

