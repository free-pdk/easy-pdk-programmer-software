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
#include "fpdkicdata.h"
#include <strings.h>

static const FPDKICDATA fpdk_ic_table[] =
{
  { .name                         = "PMS131",
    .name_variant_1               = "PMC131",
    .otpid                        = 0x0C83,
    .id12bit                      = 0xC83,
    .type                         = FPDK_IC_OTP2_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x600,
    .ramsize                      = 0x58,
    .exclude_code_start           = 0x5F9,
    .exclude_code_end             = 0x600,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PMS132",
    .name_variant_1               = "PMS132B",
    .otpid                        = 0x2F09,
    .id12bit                      = 0x109,
    .type                         = FPDK_IC_OTP2_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x800,
    .ramsize                      = 0x80,
    .exclude_code_start           = 0x7F6,
    .exclude_code_end             = 0x800,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PMS133",
    .name_variant_1               = "PMS134",
    .otpid                        = 0x2B19,
    .id12bit                      = 0xC19,
    .type                         = FPDK_IC_OTP2_1,
    .addressbits                  = 12,
    .codebits                     = 15,
    .codewords                    = 0x1000,
    .ramsize                      = 0x100,
    .exclude_code_start           = 0xFF6,
    .exclude_code_end             = 0x1000,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2, ??? 1
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PMS150C",
    .name_variant_1               = "PMS15A",
    .otpid                        = 0x2A16,
    .id12bit                      = 0xA16,
    .type                         = FPDK_IC_OTP1_2,
    .addressbits                  = 12,
    .codebits                     = 13,
    .codewords                    = 0x400,
    .ramsize                      = 0x40,
    .exclude_code_start           = 0x3F6,
    .exclude_code_end             = 0x400,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
  },

  { .name                         = "PMS152",
    .otpid                        = 0x2A27,
    .id12bit                      = 0xA27,
    .type                         = FPDK_IC_OTP2_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x500,
    .ramsize                      = 0x50,
    .exclude_code_start           = 0x4F6,
    .exclude_code_end             = 0x500,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PMS154B",
    .name_variant_1               = "PMS154C",
    .otpid                        = 0x2C06,
    .id12bit                      = 0xE06,
    .type                         = FPDK_IC_OTP1_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x800,
    .ramsize                      = 0x80,
    .exclude_code_first_instr     = true,
    .exclude_code_start           = 0x7E8,
    .exclude_code_end             = 0x800,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 11.0,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
  },

  { .name                         = "PFS154",
    .otpid                        = 0x2AA1,
    .id12bit                      = 0xAA1,
    .type                         = FPDK_IC_FLASH_1,
    .addressbits                  = 13,
    .codebits                     = 14,
    .codewords                    = 0x800,
    .ramsize                      = 0x80,
    .exclude_code_start           = 0x7E0,  //OTP area 16 words, contains empty space for user and BGTR IHRCR factory values
    .exclude_code_end             = 0x7F0,
    .vdd_cmd_read                 = 2.5,
    .vpp_cmd_read                 = 5.5,
    .vdd_read_hv                  = 2.5,
    .vpp_read_hv                  = 5.5,
    .vdd_cmd_write                = 2.5,
    .vpp_cmd_write                = 5.5,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 8.5,
    .write_block_size             = 4,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
    .vdd_cmd_erase                = 2.5,
    .vpp_cmd_erase                = 5.5,
    .vdd_erase_hv                 = 3.0,
    .vpp_erase_hv                 = 9.0,
    .erase_clocks                 = 2
  },

  { .name                         = "PMS171B",
    .otpid                        = 0x2D36,
    .id12bit                      = 0xD36,
    .type                         = FPDK_IC_OTP2_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x600,
    .ramsize                      = 0x60,
    .exclude_code_start           = 0x5F9,
    .exclude_code_end             = 0x600,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PFS172",
    .otpid                        = 0x2AA6,
    .id12bit                      = 0xCA6,
    .type                         = FPDK_IC_FLASH_2,
    .addressbits                  = 13,
    .codebits                     = 14,
    .codewords                    = 0x800,
    .ramsize                      = 0x80,
    .exclude_code_start           = 0x7E0, //OTP area 16 words, contains empty space for user and BGTR IHRCR factory values
    .exclude_code_end             = 0x7F0,
    .vdd_cmd_read                 = 2.5,
    .vpp_cmd_read                 = 5.5,
    .vdd_read_hv                  = 2.5,
    .vpp_read_hv                  = 0.0, 
    .vdd_cmd_write                = 2.5,
    .vpp_cmd_write                = 5.5,
    .vdd_write_hv                 = 5.3,
    .vpp_write_hv                 = 0.0,
    .write_block_size             = 4,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 4,
    .vdd_cmd_erase                = 2.5,
    .vpp_cmd_erase                = 5.5,
    .vdd_erase_hv                 = 5.3,
    .vpp_erase_hv                 = 0.0,
    .erase_clocks                 = 14, //2 (not sure why we need so many clocks)
  },

  { .name                         = "PFS173",
    .otpid                        = 0x2AA2,
    .id12bit                      = 0xEA2,
    .type                         = FPDK_IC_FLASH_1,
    .addressbits                  = 13,
    .codebits                     = 15,
    .codewords                    = 0xC00,
    .ramsize                      = 0x100,
    .exclude_code_start           = 0xBE0, //OTP area 16 words, contains empty space for user and BGTR IHRCR factory values
    .exclude_code_end             = 0xBF0,
    .vdd_cmd_read                 = 2.5,
    .vpp_cmd_read                 = 5.5,
    .vdd_read_hv                  = 2.5,
    .vpp_read_hv                  = 5.5,
    .vdd_cmd_write                = 2.5,
    .vpp_cmd_write                = 5.5,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 9.0,
    .write_block_size             = 4,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
    .vdd_cmd_erase                = 2.5,
    .vpp_cmd_erase                = 5.5,
    .vdd_erase_hv                 = 3.0,
    .vpp_erase_hv                 = 9.0,
    .erase_clocks                 = 4
  },

  { .name                         = "PMC251",
    .otpid                        = 0x0058,
    .id12bit                      = 0x058,
    .type                         = FPDK_IC_OTP3_1,
    .addressbits                  = 13,
    .codebits                     = 16,
    .codewords                    = 0x400,
    .ramsize                      = 0x3B,
    .exclude_code_start           = 0x3F9,
    .exclude_code_end             = 0x400,
    .vdd_cmd_read                 = 2.5, 
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 2.5,
    .vpp_read_hv                  = 5.5,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 1,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "PMS271",
    .name_variant_1               = "PMC271",
    .otpid                        = 0x0A58,
    .id12bit                      = 0xA58,
    .type                         = FPDK_IC_OTP3_1,
    .addressbits                  = 13,
    .codebits                     = 16,
    .codewords                    = 0x400,
    .ramsize                      = 0x40,
    .exclude_code_start           = 0x3F9,
    .exclude_code_end             = 0x400,
    .vdd_cmd_read                 = 2.5, 
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 2.5,
    .vpp_read_hv                  = 5.5,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 1,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

  { .name                         = "MCU390",
    .otpid                        = 0x2A31,
    .id12bit                      = 0xC31,
    .type                         = FPDK_IC_OTP1_2,
    .addressbits                  = 12,
    .codebits                     = 14,
    .codewords                    = 0x800,
    .ramsize                      = 0x80,
    .exclude_code_start           = 0x700,
    .exclude_code_end             = 0x800,
    .vdd_cmd_read                 = 3.0,
    .vpp_cmd_read                 = 5.0,
    .vdd_read_hv                  = 3.0,
    .vpp_read_hv                  = 5.0,
/*
    .vdd_cmd_write                = 4.3,
    .vpp_cmd_write                = 8.0,
    .vdd_write_hv                 = 5.8,
    .vpp_write_hv                 = 10.5,
    .write_block_size             = 2,
    .write_block_clock_groups     = 1,
    .write_block_clocks_per_group = 8,
*/
  },

};

FPDKICDATA* FPDKICDATA_GetICDataById12Bit(const uint16_t id12bit)
{
  unsigned int i;
  for( i=0; i<(sizeof(fpdk_ic_table)/sizeof(FPDKICDATA)); i++ )
  {
    if( (id12bit&0xFFF) == (fpdk_ic_table[i].id12bit) )
      return (FPDKICDATA*)&fpdk_ic_table[i];
  }

  return 0;
}

static FPDKICDATA* _FPDKICDATA_GetICDataById12BitAndCodebits(const FPDKICTYPE type, const uint16_t id12bit, const uint8_t codebits)
{
  unsigned int i;
  for( i=0; i<(sizeof(fpdk_ic_table)/sizeof(FPDKICDATA)); i++ )
  {
    if( ((id12bit&0xFFF) == (fpdk_ic_table[i].id12bit)) && 
        (type == fpdk_ic_table[i].type) &&
        (codebits == fpdk_ic_table[i].codebits)
      )
    {
      return (FPDKICDATA*)&fpdk_ic_table[i];
    }
  }

  return 0;
}

FPDKICDATA* FPDKICDATA_GetICDataByCmdResponse(const FPDKICTYPE type, const uint32_t cmdrsp)
{
  unsigned int shift;
  
  if( FPDK_IS_FLASH_TYPE(type) )
  {
    FPDKICDATA* icdata = FPDKICDATA_GetICDataById12Bit(cmdrsp);
    if( !icdata )
      icdata = FPDKICDATA_GetICDataById12Bit(cmdrsp<<1);
    return icdata;
  }

  switch( type )
  {
    case FPDK_IC_OTP2_1:
    case FPDK_IC_OTP3_1:
      shift=1;
      break;

    case FPDK_IC_OTP1_2:
    case FPDK_IC_OTP2_2:
      shift=2;
      break;

    default:
      return 0;
  }

  FPDKICDATA* icdata = _FPDKICDATA_GetICDataById12BitAndCodebits(type, cmdrsp, 16);   //try as 16 codebits words
  if( !icdata ) 
    icdata = _FPDKICDATA_GetICDataById12BitAndCodebits(type, cmdrsp>>shift, 15);      //try as 15 codebits words
  if( !icdata ) 
    icdata = _FPDKICDATA_GetICDataById12BitAndCodebits(type, cmdrsp>>(shift*2), 14);  //try as 14 codebits words
  if( !icdata ) 
    icdata = _FPDKICDATA_GetICDataById12BitAndCodebits(type, cmdrsp>>(shift*3), 13);  //try as 13 codebits words

  return icdata;
}

FPDKICDATA* FPDKICDATA_GetICDataByName(const char* name)
{
  unsigned int i;
  for( i=0; i<(sizeof(fpdk_ic_table)/sizeof(FPDKICDATA)); i++ )
  {
    if( 0 == strcasecmp(name, fpdk_ic_table[i].name) )
      return (FPDKICDATA*)&fpdk_ic_table[i];

    if( 0 == strcasecmp(name, fpdk_ic_table[i].name_variant_1) )
      return (FPDKICDATA*)&fpdk_ic_table[i];

    if( 0 == strcasecmp(name, fpdk_ic_table[i].name_variant_2) )
      return (FPDKICDATA*)&fpdk_ic_table[i];
  }

  return 0;
}

