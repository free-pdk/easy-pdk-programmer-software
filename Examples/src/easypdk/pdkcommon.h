#ifndef __PDKCOMMON_H__
#define __PDKCOMMON_H__

//macros so we can use defines in assembler strings
#define _STRINGIFY(x) #x
#define _ASMV(x) "_"_STRINGIFY(x)
#define _ASMD(x) _STRINGIFY(x)
#define _ASME(x) _ ## x
#define _ASMS(x) _ASME(x)

//definitions for built in opcodess
#define __nop()      __asm__("nop\n")
#define __engint()   __asm__("engint\n")
#define __disgint()  __asm__("disgint\n")
#define __stopsys()  __asm__("stopsys\n")
#define __stopexe()  __asm__("stopexe\nnop\n")
#define __wdreset()  __asm__("wdreset\n")
#define __set0(x,y)  __asm__("set0 "_ASMV(x)", #"_ASMD(y)"\n")
#define __set1(x,y)  __asm__("set1 "_ASMV(x)", #"_ASMD(y)"\n")

//macros for clock setup
#define EASY_PDK_INIT_SYSCLOCK_16MHZ()      {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC;}
#define EASY_PDK_INIT_SYSCLOCK_8MHZ()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV2;}
#define EASY_PDK_INIT_SYSCLOCK_4MHZ()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV4;}
#define EASY_PDK_INIT_SYSCLOCK_2MHZ()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV8;}
#define EASY_PDK_INIT_SYSCLOCK_1MHZ()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV16;}
#define EASY_PDK_INIT_SYSCLOCK_500KHZ()     {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_IHRC_DIV32;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC()       {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC_DIV4()  {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC_DIV4;}
#define EASY_PDK_INIT_SYSCLOCK_ILRC_DIV16() {_clkmd=CLKMD_ENABLE_ILRC|CLKMD_ENABLE_IHRC|CLKMD_ILRC_DIV16;}

//place holder for EASYPDK serial inserted from easypdkprog
#define EASY_PDK_SERIAL(sname) static const uint8_t sname[8] = {'F','P','S','E','R','I','A','L'}

//place holder for EASYPDK calibrations executed / replaced by easypdkprog
#define EASY_PDK_CALIBRATE_IHRC_H8(frequency,millivolt) \
__asm__(                                 \
  "and a, #'H'                        \n"\
  "and a, #'8'                        \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
)
#define EASY_PDK_CALIBRATE_ILRC_L8(frequency,millivolt) \
__asm__(                                 \
  "and a, #'L'                        \n"\
  "and a, #'8'                        \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
)

#define EASY_PDK_CALIBRATE_ILRC_l8(frequency,millivolt) \
__asm__(                                 \
  "and a, #'l'                        \n"\
  "and a, #'8'                        \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
)

#define EASY_PDK_CALIBRATE_IHRC_H9(frequency,millivolt) \
__asm__(                                 \
  "and a, #'H'                        \n"\
  "and a, #'9'                        \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
  "and a, #0                          \n"\
)

#define EASY_PDK_CALIBRATE_ILRC_L9(frequency,millivolt) \
__asm__(                                 \
  "and a, #'L'                        \n"\
  "and a, #'9'                        \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
  "and a, #0                          \n"\
)

#define EASY_PDK_CALIBRATE_BG_B19() \
__asm__(                                 \
  "and a, #'B'                        \n"\
  "and a, #'1'                        \n"\
  "and a, #'9'                        \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
)

#define EASY_PDK_CALIBRATE_BG_B1A() \
__asm__(                                 \
  "and a, #'B'                        \n"\
  "and a, #'1'                        \n"\
  "and a, #'A'                        \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
)

#define EASY_PDK_CALIBRATE_BG_B3C() \
__asm__(                                 \
  "and a, #'B'                        \n"\
  "and a, #'3'                        \n"\
  "and a, #'C'                        \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
)

#define EASY_PDK_CALIBRATE_BG_B63() \
__asm__(                                 \
  "and a, #'B'                        \n"\
  "and a, #'6'                        \n"\
  "and a, #'3'                        \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
)


#endif //__PDKCOMMON_H__
