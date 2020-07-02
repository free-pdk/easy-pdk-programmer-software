#ifndef __PDK_UTIL_H__
#define __PDK_UTIL_H__

//macros so we can use defines in assembler strings
#define _STRINGIFY(x)         #x
#define _STR(x)               _STRINGIFY(x)
#define _STR_VAR(x)           "_"_STRINGIFY(x)
#define _VAR(x)               _ ## x

//definitions for built in opcodes
#define __nop()               __asm__("nop\n")
#define __engint()            __asm__("engint\n")
#define __disgint()           __asm__("disgint\n")
#define __stopsys()           __asm__("stopsys\n")
#define __stopexe()           __asm__("stopexe\nnop\n")
#define __reset()             __asm__("reset\n")
#define __wdreset()           __asm__("wdreset\n")
#define __set0(var,bit)       __asm__("set0 "_STR_VAR(var)", #"_STR(bit)"\n")
#define __set1(var,bit)       __asm__("set1 "_STR_VAR(var)", #"_STR(bit)"\n")

#endif //__PDK_UTIL_H__
