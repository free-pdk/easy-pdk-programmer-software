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
#ifndef __FPDKPROTO_H_
#define __FPDKPROTO_H_

#define __FPDKPROTO__ "1.3"
#define FPDKPROTO_MAJOR 1
#define FPDKPROTO_MINOR 3

typedef enum FPDKPROTO_CMD
{
  FPDKPROTO_CMD_GETVERINFO   = 'I',
  FPDKPROTO_CMD_SETLED       = 'L',
  FPDKPROTO_CMD_GETBUTTON    = 'B',

  FPDKPROTO_CMD_SETVOLTOUT   = 'O',
  FPDKPROTO_CMD_GETVOLTAGES  = 'U',

  FPDKPROTO_CMD_SETBUF       = 'S',
  FPDKPROTO_CMD_GETBUF       = 'G',

  FPDKPROTO_CMD_PROBEIC      = 'P',
  FPDKPROTO_CMD_BLANKCKIC    = 'Z',
  FPDKPROTO_CMD_ERASEIC      = 'E',
  FPDKPROTO_CMD_READIC       = 'R',
  FPDKPROTO_CMD_WRITEIC      = 'W',
  FPDKPROTO_CMD_VERIFYIC     = 'V',
  FPDKPROTO_CMD_CALIBRATEIC  = 'C',

  FPDKPROTO_CMD_EXECUTEIC    = 'X',
  FPDKPROTO_CMD_STOPIC       = 'Q',
  FPDKPROTO_CMD_DBGDAT       = 'D',

} FPDKPROTO_CMD;

typedef enum FPDKPROTO_RSP
{
  FPDKPROTO_RSP_ERROR        = 'E',
  FPDKPROTO_RSP_ACK          = 'A',
  FPDKPROTO_RSP_DBGDAT       = 'D',

} FPDKPROTO_RSP;

typedef enum FPDK_ERR
{
  FPDK_ERR_UKNOWN            = 0xFFFF,
  FPDK_ERR_VPPVDD            = 0xFFFE,
  FPDK_ERR_HVPPHVDD          = 0xFFFD,
  FPDK_ERR_CMDRSP            = 0xFFFC,
  FPDK_ERR_VERIFY            = 0xFFFB,
  FPDK_ERR_NOTBLANK          = 0xFFFA,

  FPDK_ERR_ERROR             = 0xFFF0
} FPDK_ERR;

#endif //__FPDKPROTO_H_
