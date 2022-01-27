#ifndef __PDK_H__
#define __PDK_H__

#define __SDCC_VER (__SDCC_VERSION_MAJOR*10000 + __SDCC_VERSION_MINOR*100 + __SDCC_VERSION_PATCH)
#if(!(__SDCC_VER>=40110))
#error "SDCC 4.1.10 or newer is required"
#endif

#if defined(PMS150C)
 #include "pms150c.h"
#elif defined(PMS150G)
 #include "pms150g.h"
#elif defined(PFC151)
 #include "pfc151.h"
#elif defined(PMS152)
 #include "pms152.h"
#elif defined(PFC161)
 #include "pfc161.h"
#elif defined(PMS171B)
 #include "pms171b.h"
#elif defined(PFS154)
 #include "pfs154.h"
#elif defined(PFC154)
 #include "pfc154.h"
#elif defined(PFS172)
 #include "pfs172.h"
#elif defined(PFS173)
 #include "pfs173.h"
#elif defined(PFC232)
 #include "pfc232.h"
#else
 #error "Unknown processor. Please define processor"
#endif

#endif //__PDK_H__
