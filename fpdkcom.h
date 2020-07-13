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
#ifndef __FPDKCOM_H_
#define __FPDKCOM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "fpdkproto.h"
#include "fpdkicdata.h"

int      FPDKCOM_OpenAuto(char portpath[64]);

int      FPDKCOM_Open(const char* devname);

int      FPDKCOM_Close(const int fd);

bool     FPDKCOM_GetVersion(const int fd, 
                            unsigned int *hw_major, unsigned int *hw_minor, 
                            unsigned int *sw_major, unsigned int *sw_minor, 
                            unsigned int *proto_major, unsigned int *proto_minor,
                            char* fwstr, const size_t fwstrlen);

bool     FPDKCOM_SetLed(const int fd, const uint8_t ledbits);

bool     FPDKCOM_GetButtonState(const int fd, bool* buttonstate);

bool     FPDKCOM_SetOutputVoltages(const int fd, const float vdd, const float vpp);

bool     FPDKCOM_MeasureOutputVoltages(const int fd, float* vdd, float* vpp, float* vref);

bool     FPDKCOM_SetBuffer(const int fd, const uint16_t woffset, const uint8_t* dat, const uint16_t len);

int      FPDKCOM_GetBuffer(const int fd, const uint16_t roffset, uint8_t* dat, const uint16_t len);


int      FPDKCOM_IC_Probe(const int fd, float* vpp_found, float* vdd_found, FPDKICTYPE* type);

int      FPDKCOM_IC_BlankCheck(const int fd, const uint16_t icid, const FPDKICTYPE type,
                               const float vdd_cmd, const float vpp_cmd,
                               const float vdd_read, const float vpp_read,
                               const uint8_t addr_bits, const uint8_t data_bits, const uint16_t count,
                               const bool exclude_first_instructionconst, uint16_t exclude_start, const uint16_t exclude_end);

int      FPDKCOM_IC_Erase(const int fd, const uint16_t icid, const FPDKICTYPE type,
                          const float vdd_cmd, const float vpp_cmd, const float vdd_erase, const float vpp_erase, const uint8_t erase_clocks);

int      FPDKCOM_IC_Read(const int fd, const uint16_t icid, const FPDKICTYPE type,
                         const float vdd_cmd, const float vpp_cmd,
                         const float vdd_read, const float vpp_read,
                         const uint16_t addr, const uint8_t addr_bits,
                         const uint16_t data_offs, const uint8_t data_bits,
                         const uint16_t count);

int      FPDKCOM_IC_Write(const int fd, const uint16_t icid, const FPDKICTYPE type,
                          const float vdd_cmd, const float vpp_cmd,
                          const float vdd_write, const float vpp_write,
                          const uint16_t addr, const uint8_t addr_bits,
                          const uint16_t data_offs, const uint8_t data_bits,
                          const uint16_t count, 
                          const uint8_t write_block_size, const uint8_t write_block_clock_groups, const uint8_t write_block_clocks_per_group);

int      FPDKCOM_IC_Verify(const int fd, const uint16_t icid, const FPDKICTYPE type,
                           const float vdd_cmd, const float vpp_cmd,
                           const float vdd_read, const float vpp_read,
                           const uint16_t addr, const uint8_t addr_bits,
                           const uint16_t data_offs, const uint8_t data_bits,
                           const uint16_t count,
                           const bool exclude_first_instruction, const uint16_t exclude_start, const uint16_t exclude_end);


bool     FPDKCOM_IC_Calibrate(const int fd, const uint32_t type, const uint32_t vdd, const uint32_t freq, const uint32_t mult, 
                              uint8_t* fcalval, uint32_t* fcalfreq);


bool     FPDKCOM_IC_StartExecution(const int fd, const float vdd);

bool     FPDKCOM_IC_StopExecution(const int fd);


int      FPDKCOM_IC_ReceiveDebugData(const int fd, uint8_t* dat, const uint8_t len);

bool     FPDKCOM_IC_SendDebugData(const int fd, const uint8_t* dat, const uint8_t len);

#endif //__FPDKCOM_H_
