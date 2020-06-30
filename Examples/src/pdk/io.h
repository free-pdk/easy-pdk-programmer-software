#ifndef __PDK_IO_H__
#define __PDK_IO_H__

#if defined(PMS150C)
  #include "io_pms150c.h"
#elif defined(PFS154)
  #include "io_pfs154.h"
#elif defined(PFS172)
  #include "io_pfs172.h"
#elif defined(PFS173)
  #include "io_pfs173.h"
#else
#  error "Unknown device. Please define device!"
#endif

#include "sfr_defs.h"
#include "sysclock.h"

#endif //__PDK_IO_H__
