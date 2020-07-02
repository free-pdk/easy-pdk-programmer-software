#include <stdint.h>
#include "pdk/device.h"
#include "easy-pdk/calibrate.h"

unsigned char _sdcc_external_startup(void)
{
  //
  // F U S E S
  //
  // Set specific FUSES (defines in IC specific header file, e.g. "pdk/io_pfs173.h"), easypdk programmer will set FUSE values after writing
  PDK_SET_FUSE(FUSE_SECURITY_OFF|FUSE_BOOTUP_FAST);

  //
  // C A L I B R A T I O N S
  //
  // Calibrations should always be inserted in "_sdcc_external_startup()" function to ensure reachability at startup.
  //
  // SYSCLK setup and calibration (Your application should use only one of the IHRC and/or ILRC setup and calibrations).
  
  // setup ILRC as sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_ILRC_SYSCLOCK();                      //use ILRC as sysclock, 60-96kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ,4000);      //tune SYSCLK to 95kHz @ 4.000V (value for PFS173)

  // setup ILRC/4 as sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_ILRC_DIV4_SYSCLOCK();                 //use ILRC/4 as sysclock, 15-24kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ/4,3300);    //tune SYSCLK to 24kHz @ 3.300V (value for PFS173)

  // setup ILRC/16 as sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_ILRC_DIV16_SYSCLOCK();                //use ILRC/16 as sysclock, 4-6kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ/16,3000);   //tune SYSCLK to 6kHz @ 3.000V (value for PFS173)

#if defined(PDK_USE_FACTORY_IHRCR_16MHZ)
  // setup 8MHz sysclock and use factory calibration (available on MTP devices)
  PDK_USE_8MHZ_IHRC_SYSCLOCK();                 //use 8MHz sysclock
  PDK_USE_FACTORY_IHRCR_16MHZ();                //use factory calibration value
#endif

	// NOTE: Technically this frequency is unsupported, but it seems to work (at least on some devices)
  // setup 16MHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_16MHZ_IHRC_SYSCLOCK();                 //use 16MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(16000000,5000);        //tune SYSCLK to 16MHz @ 5.000V

  // setup 8MHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_8MHZ_IHRC_SYSCLOCK();                 //use 8MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(8000000,5000);        //tune SYSCLK to 8MHz @ 5.000V

  // setup 4MHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_4MHZ_IHRC_SYSCLOCK();                 //use 4MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(4000000,4500);        //tune SYSCLK to 4MHz @ 4.500V

  // setup 2MHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_2MHZ_IHRC_SYSCLOCK();                 //use 2MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(2000000,4000);        //tune SYSCLK to 4MHz @ 4.000V

  // setup 1MHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_1MHZ_IHRC_SYSCLOCK();                 //use 1MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(1000000,3300);        //tune SYSCLK to 1MHz @ 3.300V

  // setup 500kHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_500KHZ_IHRC_SYSCLOCK();               //use 500KHZ sysclock
  EASY_PDK_CALIBRATE_IHRC(500000,3000);         //tune SYSCLK to 500kHz @ 3.000V

  // setup 250kHz sysclock and let easypdk programmer do the calibration (after writing)
  PDK_USE_250KHZ_IHRC_SYSCLOCK();               //use 250KHZ sysclock
  EASY_PDK_CALIBRATE_IHRC(250000,3000);         //tune SYSCLK to 250kHz @ 3.000V


  // BANDGAP calibration:
  PDK_USE_FACTORY_BGTR();                       //use factory BandGap calibration value
                                                // -OR-
  EASY_PDK_CALIBRATE_BG();                      //let easypdk programmer do the bandgap calibration (after writing) - bandgap tuning requires any IHRC SYSCLK


  return 0;                                     //perform normal initialization
}

void main(void)
{
  for(;;);                                      //demo endless loop
}
