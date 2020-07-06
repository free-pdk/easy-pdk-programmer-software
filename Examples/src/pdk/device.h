#ifndef __PDK_DEVICE_H__
#define __PDK_DEVICE_H__

#if defined(PMS131)
  #include "device/pms131.h"
#elif defined(PMS150C)
  #include "device/pms150c.h"
#elif defined(PMS15A)
  #include "device/pms150c.h"
#elif defined(PMS152)
  #include "device/pms152.h"
#elif defined(PMS154B)
  #include "device/pms154b.h"
#elif defined(PMS154C)
  #include "device/pms154c.h"
#elif defined(PMS171B)
  #include "device/pms171b.h"
#elif defined(PFS154)
  #include "device/pfs154.h"
#elif defined(PFS172)
  #include "device/pfs172.h"
#elif defined(PFS173)
  #include "device/pfs173.h"
#else
	#error "Unknown device. Please define device!"
#endif

#include "util.h"
#include "fuse.h"
#include "factory_calibration.h"
#include "sysclock.h"

#endif //__PDK_DEVICE_H__
