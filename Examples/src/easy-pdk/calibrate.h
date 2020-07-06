#ifndef __EASY_PDK_CALIBRATE_H__
#define __EASY_PDK_CALIBRATE_H__

#if !defined(__PDK_DEVICE_H__)
	#error "You must #include "pdk/device.h" "
#endif

#define EASY_PDK_CALTYPE_IHRC 1
#define EASY_PDK_CALTYPE_ILRC 2

// Placeholders for EASYPDK calibrations executed / replaced by easypdkprog
#define EASY_PDK_CALIBRATE_IHRC(frequency,millivolt)  EASY_PDK_CALIBRATE_RC_M(EASY_PDK_CALTYPE_IHRC, IHRCR_ADDR, frequency, millivolt)
#if defined(ILRCR_ADDR)
  #define EASY_PDK_CALIBRATE_ILRC(frequency,millivolt)  EASY_PDK_CALIBRATE_RC_M(EASY_PDK_CALTYPE_ILRC, ILRCR_ADDR, frequency, millivolt)
#endif
#define EASY_PDK_CALIBRATE_BG()                       EASY_PDK_CALIBRATE_BG_M(BGTR_ADDR, GPCC_ADDR, GPCS_ADDR)

#define EASY_PDK_CALIBRATE_RC_M(type,reg,frequency,millivolt) \
__asm__(                                \
  "and a, #'R'                       \n"\
  "and a, #'C'                       \n"\
  "and a, #("_STR(type)")            \n"\
  "and a, #(("_STR(frequency)"))     \n"\
  "and a, #(("_STR(frequency)")>>8)  \n"\
  "and a, #(("_STR(frequency)")>>16) \n"\
  "and a, #(("_STR(frequency)")>>24) \n"\
  "and a, #(("_STR(millivolt)"))     \n"\
  "and a, #(("_STR(millivolt)")>>8)  \n"\
  "and a, #("_STR(reg)")             \n"\
)

#define EASY_PDK_CALIBRATE_BG_M(bgtr,gpcc,gpcs) \
__asm__(                                \
  "and a, #'B'                       \n"\
  "and a, #'G'                       \n"\
  "and a, #(3)                       \n"\
  "and a, #(3950)                    \n"\
  "and a, #(3950>>8)                 \n"\
  "and a, #("_STR(bgtr)")            \n"\
  "and a, #("_STR(gpcc)")            \n"\
  "and a, #("_STR(gpcs)")            \n"\
  "and a, #0                         \n"\
  "and a, #0                         \n"\
  "and a, #0                         \n"\
  "and a, #0                         \n"\
  "and a, #0                         \n"\
  "and a, #0                         \n"\
)

#endif //__EASY_PDK_CALIBRATE_H__
