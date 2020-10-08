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

//place holder for EASYPDK serial inserted from easypdkprog
#define EASY_PDK_SERIAL(sname) static const uint8_t sname[8] = {'F','P','S','E','R','I','A','L'}

//place holder for EASYPDK calibrations executed / replaced by easypdkprog
#define EASY_PDK_CALTYPE_IHRC 1
#define EASY_PDK_CALTYPE_ILRC 2

#define EASY_PDK_CALIBRATE_RC_M(type,reg,frequency,millivolt) \
__asm__(                                 \
  "and a, #'R'                        \n"\
  "and a, #'C'                        \n"\
  "and a, #(("_ASMD(type)"))          \n"\
  "and a, #(("_ASMD(frequency)"))     \n"\
  "and a, #(("_ASMD(frequency)")>>8)  \n"\
  "and a, #(("_ASMD(frequency)")>>16) \n"\
  "and a, #(("_ASMD(frequency)")>>24) \n"\
  "and a, #(("_ASMD(millivolt)"))     \n"\
  "and a, #(("_ASMD(millivolt)")>>8)  \n"\
  "and a, #(("_ASMD(reg)"))           \n"\
)

#define EASY_PDK_CALIBRATE_BG_M(bgtr,gpcc,gpcs) \
__asm__(                                 \
  "and a, #'B'                        \n"\
  "and a, #'G'                        \n"\
  "and a, #(3)                        \n"\
  "and a, #(3950)                     \n"\
  "and a, #(3950>>8)                  \n"\
  "and a, #(("_ASMD(bgtr)"))          \n"\
  "and a, #(("_ASMD(gpcc)"))          \n"\
  "and a, #(("_ASMD(gpcs)"))          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
  "and a, #0                          \n"\
)

#endif //__PDKCOMMON_H__
