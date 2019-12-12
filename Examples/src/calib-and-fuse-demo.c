#include <stdint.h>
#include "easypdk/pdk.h"

unsigned char _sdcc_external_startup(void)
{
  //
  // F U S E S
  //
  // Set specific FUSES (defines in IC specific header file, e.g. "easypdk/pfs173.h"), easypdk programmer will set FUSE values after writing
  EASY_PDK_FUSE(FUSE_SECURITY_OFF|FUSE_BOOTUP_FAST);

  //
  // C A L I B R A T I O N S
  //
  // Calibrations should always be inserted in "_sdcc_external_startup()" function to ensure reachability at startup.
  //
  // SYSCLK setup and calibration (Your application should use only one of the IHRC and/or ILRC setup and claibrations).
  
  // setup ILRC as sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_ILRC();                //use ILRC as sysclock, 60-96kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ,4000);      //tune SYSCLK to 95kHz @ 4.000V (value for PFS173)

  // setup ILRC/4 as sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_ILRC_DIV4();           //use ILRC/4 as sysclock, 15-24kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ/4,3300);    //tune SYSCLK to 24kHz @ 3.300V (value for PFS173)

#if defined(CLKMD_ILRC_DIV16)
  // setup ILRC/16 as sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_ILRC_DIV16();          //use ILRC/16 as sysclock, 4-6kHz, depends on IC variant
  EASY_PDK_CALIBRATE_ILRC(ILRC_FREQ/16,3000);   //tune SYSCLK to 6kHz @ 3.000V (value for PFS173)
#endif

#if defined(PFS154) || defined(PFS173)
  // setup 8MHz sysclock and use factory calibration (available on PFS154 and PFS173)
  EASY_PDK_INIT_SYSCLOCK_8MHZ();                //use 8MHz sysclock
  EASY_PDK_USE_FACTORY_IHRCR_16MHZ();           //use factory calibration value
#endif

  // setup 8MHz sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_8MHZ();                //use 8MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(8000000,5000);        //tune SYSCLK to 8MHz @ 5.000V

  // setup 4MHz sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_4MHZ();                //use 4MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(4000000,4500);        //tune SYSCLK to 4MHz @ 4.500V

  // setup 2MHz sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_2MHZ();                //use 2MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(2000000,4000);        //tune SYSCLK to 4MHz @ 4.000V

  // setup 1MHz sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_1MHZ();                //use 1MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(1000000,3300);        //tune SYSCLK to 1MHz @ 3.300V

  // setup 500kHz sysclock and let easypdk programmer do the calibration (after writing)
  EASY_PDK_INIT_SYSCLOCK_500KHZ();              //use 500KHZ sysclock
  EASY_PDK_CALIBRATE_IHRC(500000,3000);         //tune SYSCLK to 500kHz @ 3.000V


  // BANDGAP calibration:
  EASY_PDK_USE_FACTORY_BGTR();                  //use factory BandGap calibration value
                                                // -OR-
  EASY_PDK_CALIBRATE_BG();                      //let easypdk progammer do the bandgap calibration (after writing) - bandgap tuning requires any IHRC SYSCLK


  return 0;                                     //perform normal initialization
}

void main(void)
{
  for(;;);                                      //demo endless loop
}
