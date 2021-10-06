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

#include "fpdkiccalib.h"

#define FPDKCALIBCP_MAXWORDS 16

#define CO_BC_FIXUP    0x1000  //Before Calibration FIXUP                   (add insert position to opcode)
#define CO_BC_SETREG0  0x2000  //Before Calibration SET REGISTER0           (sets the register0 used for calibration)
#define CO_BC_SETREG1  0x4000  //Before Calibration SET REGISTER1           (sets the register1 used for calibration)
#define CO_BC_SETREG2  0x8000  //Before Calibration SET REGISTER2           (sets the register2 used for calibration)

#define CO_AC_SAME     0x0000  //After Calibration stay same                (intruction is not changed)
#define CO_AC_NOP      0x0100  //After Calibration NOP                      (replace the calibration opcode with a NOP after calibration is done)
#define CO_AC_SETIMM   0x0200  //After Calibration SET IMMediate from val 2 (set lower part to value supplied to remove function, e.g. the calibration result)

#define CO_VAL_FREQ_00  0x0001
#define CO_VAL_FREQ_08  0x0002
#define CO_VAL_FREQ_16  0x0003
#define CO_VAL_FREQ_24  0x0004

#define CO_VAL_MVOL_00  0x0010
#define CO_VAL_MVOL_08  0x0011

#define CO_VAL_CAL_REG0 0x0020
#define CO_VAL_CAL_REG1 0x0021
#define CO_VAL_CAL_REG2 0x0022

#define CO_VAL_CAL_TYP  0x0030

typedef struct FPDKCALIBCP
{
  uint16_t sopc; //search opcode
  uint16_t smsk; //serach mask
  uint16_t copc; //calibration opcode
  uint16_t cocf; //calibartion opocde flags

} FPDKCALIBCP;


typedef struct FPDKCALIBALGO
{
  bool unknown;
  uint8_t codebits;
  uint32_t loopcycles;
  FPDKCALIBCP algo[FPDKCALIBCP_MAXWORDS];

} FPDKCALIBALGO;

static const FPDKCALIBALGO fpdk_calib_algos[] = { 

 {
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0x1452, .smsk=0xFFFF, .copc=0x0F71, .cocf=CO_AC_NOP},                                //search: AND A, 'R'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x1443, .smsk=0xFFFF, .copc=0x0F8D, .cocf=CO_AC_NOP},                                //search: AND A, 'C'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x17FF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_TYP},              //search: AND A, *   (CAL_TYP)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1001, .cocf=CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0F70, .cocf=CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0080, .cocf=CO_BC_SETREG0|CO_VAL_FREQ_16},             //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x00), A  (SETREG)  /   after-calib: stay same
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0C90, .cocf=CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1001, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0E70, .cocf=CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_00)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1804, .cocf=CO_BC_FIXUP|CO_AC_NOP|CO_VAL_CAL_REG0},    //search: AND A, *   (CALREG0)  /   calib: GOTO 0x004       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0x1442, .smsk=0xFFFF, .copc=0x1709, .cocf=CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x1447, .smsk=0xFFFF, .copc=0x0091, .cocf=CO_AC_NOP},                                //search: AND A, 'G'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0F8D, .cocf=CO_AC_NOP|CO_VAL_CAL_TYP},                 //search: AND A, *   (CAL_TYP)  /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x1784, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0080, .cocf=CO_BC_SETREG1|CO_AC_NOP|CO_VAL_MVOL_08},   //search: AND A, *   (MVOL_08)  /   calib: MOV IO(0x00), A  (SETREG1) /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x17A3, .cocf=CO_AC_NOP|CO_VAL_CAL_REG0},                //search: AND A, *   (CALREG0)  /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x0080, .cocf=CO_BC_SETREG2|CO_AC_NOP|CO_VAL_CAL_REG1},  //search: AND A, *   (CALREG1)  /   calib: MOV IO(0x1E), A  (SETREG2) /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFF00, .copc=0x17FF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_REG2},             //search: AND A, *   (CALREG2)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0F70, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0080, .cocf=CO_BC_SETREG0},                            //search: AND A, 0              /   calib: MOV IO(0x00), A  (SETREG0) /   after-calib: stay
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0C90, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x1001, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x0E70, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x1400, .smsk=0xFFFF, .copc=0x1808, .cocf=CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x008       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C52, .smsk=0xFFFF, .copc=0x1ED1, .cocf=CO_AC_NOP},                                //search: AND A, 'R'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x2C43, .smsk=0xFFFF, .copc=0x1F0D, .cocf=CO_AC_NOP},                                //search: AND A, 'C'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2FFF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_TYP},              //search: AND A, *   (CAL_TYP)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1ED0, .cocf=CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x0180, .cocf=CO_BC_SETREG0|CO_VAL_FREQ_16},             //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x00), A  (SETREG)  /   after-calib: stay same
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1910, .cocf=CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2801, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1CD0, .cocf=CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_08)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x3004, .cocf=CO_BC_FIXUP|CO_AC_NOP|CO_VAL_CAL_REG0},    //search: AND A, *   (CALREG0)  /   calib: GOTO 0x004       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0x2C42, .smsk=0xFFFF, .copc=0x2F09, .cocf=CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x2C47, .smsk=0xFFFF, .copc=0x0191, .cocf=CO_AC_NOP},                                //search: AND A, 'G'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x1F0D, .cocf=CO_AC_NOP|CO_VAL_CAL_TYP},                 //search: AND A, *   (CAL_TYP)  /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2F84, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x0180, .cocf=CO_BC_SETREG1|CO_AC_NOP|CO_VAL_MVOL_08},   //search: AND A, *   (MVOL_08)  /   calib: MOV IO(0x00), A  (SETREG1) /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2FA3, .cocf=CO_AC_NOP|CO_VAL_CAL_REG0},                //search: AND A, *   (CALREG0)  /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x0180, .cocf=CO_BC_SETREG2|CO_AC_NOP|CO_VAL_CAL_REG1},  //search: AND A, *   (CALREG1)  /   calib: MOV IO(0x00), A  (SETREG2) /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFF00, .copc=0x2FFF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_REG2},             //search: AND A, *   (CALREG2)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1ED0, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x0180, .cocf=CO_BC_SETREG0},                            //search: AND A, 0              /   calib: MOV IO(0x00), A  (SETREG0) /   after-calib: stay
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1910, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x2801, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x1CD0, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x2C00, .smsk=0xFFFF, .copc=0x3008, .cocf=CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x008       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0x5452, .smsk=0xFFFF, .copc=0x3D91, .cocf=CO_AC_NOP},                                //search: AND A, 'R'            /   calib: SET1 IO(0x11).3  (PAC.3)   /   after-calib: NOP
    { .sopc=0x5443, .smsk=0xFFFF, .copc=0x3E0D, .cocf=CO_AC_NOP},                                //search: AND A, 'C'            /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x57FF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_TYP},              //search: AND A, *   (CAL_TYP)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x5001, .cocf=CO_AC_NOP|CO_VAL_FREQ_00},                 //search: AND A, *   (FREQ_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3D90, .cocf=CO_AC_NOP|CO_VAL_FREQ_08},                 //search: AND A, *   (FREQ_08)  /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x0100, .cocf=CO_BC_SETREG0|CO_VAL_FREQ_16},             //search: AND A, *   (FREQ_16)  /   calib: MOV IO(0x00), A  (SETREG)  /   after-calib: stay
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3210, .cocf=CO_AC_NOP|CO_VAL_FREQ_24},                 //search: AND A, *   (FREQ_24)  /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x5001, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3990, .cocf=CO_AC_NOP|CO_VAL_MVOL_08},                 //search: AND A, *   (MVOL_08)  /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x6004, .cocf=CO_BC_FIXUP|CO_AC_NOP|CO_VAL_CAL_REG0},    //search: AND A, 0   (CALREG0)  /   calib: GOTO 0x004       <FIXUP>   /   after-calib: NOP
  }
 },

 {
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0x5442, .smsk=0xFFFF, .copc=0x5709, .cocf=CO_AC_NOP},                                //search: AND A, 'B'            /   calib: MOV A, 0x09                /   after-calib: NOP
    { .sopc=0x5447, .smsk=0xFFFF, .copc=0x0111, .cocf=CO_AC_NOP},                                //search: AND A, 'G'            /   calib: MOV IO(0x11), A  (PAC)     /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x3E0D, .cocf=CO_AC_NOP|CO_VAL_CAL_TYP},                 //search: AND A, *   (CAL_TYP)  /   calib: SET1 IO(0x0D).4  (PADIER.4)/   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x5784, .cocf=CO_AC_NOP|CO_VAL_MVOL_00},                 //search: AND A, *   (MVOL_00)  /   calib: MOV A, 0x84                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x0100, .cocf=CO_BC_SETREG1|CO_AC_NOP|CO_VAL_MVOL_08},   //search: AND A, *   (MVOL_08)  /   calib: MOV IO(0x00), A  (SETREG1) /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x57A3, .cocf=CO_AC_NOP|CO_VAL_CAL_REG0},                //search: AND A, *   (CALREG0)  /   calib: MOV A, 0xA3                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x0100, .cocf=CO_BC_SETREG2|CO_AC_NOP|CO_VAL_CAL_REG1},  //search: AND A, *   (CALREG1)  /   calib: MOV IO(0x00), A  (SETREG2) /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFF00, .copc=0x57FF, .cocf=CO_AC_SETIMM|CO_VAL_CAL_REG2},             //search: AND A, *   (CALREG2)  /   calib: MOV A, 0xFF                /   after-calib: MOV A, <val>
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3D90, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET1 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x0100, .cocf=CO_BC_SETREG0},                            //search: AND A, 0              /   calib: MOV IO(0x00), A  (SETREG0) /   after-calib: stay
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3210, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: T0SN IO(0x10).4  (PA.4)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x5001, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: ADD A, 0x01                /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x3990, .cocf=CO_AC_NOP},                                //search: AND A, 0              /   calib: SET0 IO(0x10).3  (PA.3)    /   after-calib: NOP
    { .sopc=0x5400, .smsk=0xFFFF, .copc=0x6008, .cocf=CO_BC_FIXUP|CO_AC_NOP},                    //search: AND A, 0              /   calib: GOTO 0x008       <FIXUP>   /   after-calib: NOP
  }
 },

 //special algos inserted into PDK files
 {
  .codebits=13,
  .loopcycles=7,
  .algo={
    { .sopc=0xFFFF, .copc=0x1734, .cocf=CO_AC_NOP},              //calib: MOV A, 0x34      (IHRC/2)   /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0083, .cocf=CO_AC_NOP},              //calib: MOV IO(0x03),A   (CLKMD)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0F71, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x11).3  (PAC.3)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0F8D, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x0D).4  (PADIER.4) /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x17FF, .cocf=CO_AC_SETIMM},           //calib: MOV A, 0xFF                 /   after-calib: MOV A, <val>
    { .sopc=0xFFFF, .copc=0x1001, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0F70, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x008B, .cocf=CO_AC_NOP},              //calib: MOV IO(0x0B), A  (IHRCR)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0C90, .cocf=CO_AC_NOP},              //calib: T0SN IO(0x10).4  (PA.4)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1001, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0E70, .cocf=CO_AC_NOP},              //calib: SET0 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1805, .cocf=CO_BC_FIXUP|CO_AC_NOP},  //calib: GOTO 0x005       <FIXUP>    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x003A, .cocf=CO_AC_SAME},             //calib: RET                         /   after-calib: stay same
  }
 },

 {
  .codebits=14,
  .loopcycles=7,
  .algo={
    { .sopc=0xFFFF, .copc=0x2F34, .cocf=CO_AC_NOP},              //calib: MOV A, 0x34      (IHRC/2)   /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0183, .cocf=CO_AC_NOP},              //calib: MOV IO(0x03),A   (CLKMD)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1ED1, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x11).3  (PAC.3)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1F0D, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x0D).4  (PADIER.4) /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x2FFF, .cocf=CO_AC_SETIMM},           //calib: MOV A, 0xFF                 /   after-calib: MOV A, <val>
    { .sopc=0xFFFF, .copc=0x2801, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1ED0, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x018B, .cocf=CO_AC_NOP},              //calib: MOV IO(0x0B), A  (IHRCR)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1910, .cocf=CO_AC_NOP},              //calib: T0SN IO(0x10).4  (PA.4)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x2801, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x1CD0, .cocf=CO_AC_NOP},              //calib: SET0 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3005, .cocf=CO_BC_FIXUP|CO_AC_NOP},  //calib: GOTO 0x005       <FIXUP>    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x007A, .cocf=CO_AC_SAME},             //calib: RET                         /   after-calib: stay same
  }
 },

 {
  .codebits=15,
  .loopcycles=7,
  .algo={
    { .sopc=0xFFFF, .copc=0x2F34, .cocf=CO_AC_NOP},              //calib: MOV A, 0x34      (IHRC/2)   /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x0103, .cocf=CO_AC_NOP},              //calib: MOV IO(0x03),A   (CLKMD)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3D91, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x11).3  (PAC.3)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3E0D, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x0D).4  (PADIER.4) /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x57FF, .cocf=CO_AC_SETIMM},           //calib: MOV A, 0xFF                 /   after-calib: MOV A, <val>
    { .sopc=0xFFFF, .copc=0x5001, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3D90, .cocf=CO_AC_NOP},              //calib: SET1 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x010B, .cocf=CO_AC_NOP},              //calib: MOV IO(0x0B), A  (IHRCR)    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3210, .cocf=CO_AC_NOP},              //calib: T0SN IO(0x10).4  (PA.4)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x5001, .cocf=CO_AC_NOP},              //calib: ADD A, 0x01                 /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x3990, .cocf=CO_AC_NOP},              //calib: SET0 IO(0x10).3  (PA.3)     /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x6005, .cocf=CO_BC_FIXUP|CO_AC_NOP},  //calib: GOTO 0x005       <FIXUP>    /   after-calib: NOP
    { .sopc=0xFFFF, .copc=0x007A, .cocf=CO_AC_SAME},             //calib: RET                         /   after-calib: stay same
  }
 },

 //undefined algos must be defined at the end, used to catch all unknown magic blocks to warn user
 {
  .unknown=true,
  .codebits=13,
  .algo={
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1400, .smsk=0xFF00 }, //search: AND A, *
  }
 },

 {
  .unknown=true,
  .codebits=14,
  .algo={
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x2C00, .smsk=0xFF00 }, //search: AND A, *
  }
 },

 {
  .unknown=true,
  .codebits=15,
  .algo={
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x5400, .smsk=0xFF00 }, //search: AND A, *
  }
 },

 {
  .unknown=true,
  .codebits=16,
  .algo={
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
    { .sopc=0x1C00, .smsk=0xFF00 }, //search: AND A, *
  }
 },

};

static uint16_t _FPDKCALIB_GetAlgoLength(const FPDKCALIBCP* algo)
{
  uint16_t algowords;
  for( algowords=0; algowords<FPDKCALIBCP_MAXWORDS; algowords++ )
  {
    if( 0 == algo[algowords].sopc )
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
        calibdata->loopcycles = fpdk_calib_algos[p].loopcycles;
        calibdata->algo = p;
        break;
      }
    }
  }
  if( 0xFFFF == calibdata->pos )
    return false;
  
  if( fpdk_calib_algos[calibdata->algo].unknown )
  {
    calibdata->type = FPDKCALIB_UNKNOWN;
    return true;
  }

  uint16_t algowords = _FPDKCALIB_GetAlgoLength(fpdk_calib_algos[calibdata->algo].algo);

  uint8_t calibreg0 = 0;
  uint8_t calibreg1 = 0;
  uint8_t calibreg2 = 0;

  calibdata->frequency = 0;
  calibdata->millivolt = 0;

  //extract type, reg, mvolt, freq
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
      case CO_VAL_CAL_TYP: calibdata->type = code16[calibdata->pos+x]&0xFF; break;
      case CO_VAL_CAL_REG0: calibreg0  = code16[calibdata->pos+x]&0xFF; break;
      case CO_VAL_CAL_REG1: calibreg1  = code16[calibdata->pos+x]&0xFF; break;
      case CO_VAL_CAL_REG2: calibreg2  = code16[calibdata->pos+x]&0xFF; break;
      default:
        break;
    }
  }

  //insert calibration code + setreg + fixup
  for( uint16_t i=0; i<algowords; i++ )
  {
    code16[calibdata->pos+i] = fpdk_calib_algos[calibdata->algo].algo[i].copc;

    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_SETREG0 )
      code16[calibdata->pos+i] |= calibreg0;
    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_SETREG1 )
      code16[calibdata->pos+i] |= calibreg1;
    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_SETREG2 )
      code16[calibdata->pos+i] |= calibreg2;

    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_FIXUP )
      code16[calibdata->pos+i] += calibdata->pos;
  }

  return true;
}

bool FPDKCALIB_InsertCalibrationPDK(const FPDKICDATA* icdata, uint8_t* code, const uint16_t pos, const uint32_t frequency, const uint32_t millivolt, FPDKCALIBDATA* calibdata)
{
  uint16_t* code16 = (uint16_t*)code;

  calibdata->pos = 0xFFFF;

  //select algo
  for( uint8_t p=0; p<sizeof(fpdk_calib_algos)/sizeof(FPDKCALIBALGO); p++ )
  {
    if( (fpdk_calib_algos[p].codebits == icdata->codebits) &&
        (fpdk_calib_algos[p].algo[0].sopc == 0xFFFF) 
      )
    {
      calibdata->pos = pos;
      calibdata->loopcycles = fpdk_calib_algos[p].loopcycles;
      calibdata->algo = p;
      break;
    }
  }
  if( 0xFFFF == calibdata->pos )
    return false;
  
  uint16_t algowords = _FPDKCALIB_GetAlgoLength(fpdk_calib_algos[calibdata->algo].algo);

  calibdata->type      = FPDKCALIB_IHRC;
  calibdata->frequency = frequency;
  calibdata->millivolt = millivolt;

  //insert calibration code + setreg + fixup
  for( uint16_t i=0; i<algowords; i++ )
  {
    code16[calibdata->pos+i] = fpdk_calib_algos[calibdata->algo].algo[i].copc;

    if( fpdk_calib_algos[calibdata->algo].algo[i].cocf & CO_BC_FIXUP )
      code16[calibdata->pos+i] += calibdata->pos;
  }
  
  //calculate GOTO and insert at PDK IHRCR subroutine
  uint16_t opgoto;
  switch( fpdk_calib_algos[calibdata->algo].codebits )
  {
    case 13: opgoto = 0x1800; break;
    case 14: opgoto = 0x3000; break;
    case 15: opgoto = 0x6000; break;
    case 16: opgoto = 0xC000; break;
  }
  code16[icdata->codewords-2] = opgoto | pos;                              //insert GOTO pos

  //PFC151/161...
  code16[icdata->codewords-10] = opgoto | pos;                             //insert GOTO pos

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

