#ifndef __PDK_DEVICE_PERIPH_MISC2_H__
#define __PDK_DEVICE_PERIPH_MISC2_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/device/periph/misc2.h" by itself."
#endif

// MISC2 register definitions
#define MISC2_COMP_EDGE_SEL_BIT0     5

#define MISC2_COMP_EDGE_INT_BOTH     0x00
#define MISC2_COMP_EDGE_INT_RISE     (1 << MISC2_COMP_EDGE_SEL_BIT0)
#define MISC2_COMP_EDGE_INT_FALL     (2 << MISC2_COMP_EDGE_SEL_BIT0)

#endif //__PDK_DEVICE_PERIPH_MISC2_H__
