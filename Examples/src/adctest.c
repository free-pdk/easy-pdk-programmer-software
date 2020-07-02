// ADC test (requires ADC capable IC like PFS173). ADC pin on PA.0, serial output of ADC values on PA7 at 115200 baud.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "pdk/device.h"
#include "easy-pdk/calibrate.h"

volatile uint16_t txdata;                       //txdata, serial data shift register

//NOTE: Using puts instead of printf which has a huge footprint (big CODE + big RAM). Do not use it in your projects unless you have to.
char str_buf[10];

void interrupt(void) __interrupt(0)
{
  if( INTRQ & INTRQ_TM2 )                       //TM2 interrupt request?
  {
    INTRQ &= ~INTRQ_TM2;                        //mark TM2 interrupt request processed
    if( txdata )                                //txdata contains bits to send?
    {
      if( txdata&1 )                            //check bit (1/0) for sending
        __set1( PA, 7 );                        //send 1 on PA7
      else
        __set0( PA, 7 );                        //send 0 on PA7
      txdata >>= 1;                             //shift txdata 
    }
  }
}

int putchar(int c)
{
  while(txdata);                                //wait for completion of previous transmission
  INTEN &= ~INTEN_TM2;                          //disable TM2 (setup of 16 bit value txdata is non atomic)
  txdata = (c << 1) | 0x200;                    //setup txdata with start and stop bit
  INTEN |= INTEN_TM2;                           //enable TM2
  return (c);
}

unsigned char _sdcc_external_startup(void)
{
  PDK_USE_8MHZ_IHRC_SYSCLOCK();                 //use 8MHz sysclock
#if defined(PDK_USE_FACTORY_IHRCR_16MHZ)
  PDK_USE_FACTORY_IHRCR_16MHZ()                 //use factory IHCR tuning value (tuned for 8MHz SYSCLK @ 5.0V)
#else
	EASY_PDK_CALIBRATE_IHRC(8000000,5000);        //calibrate to 8MHz @ 5.0V (OTP devices don't have factory IHRC calibration)
#endif
  PDK_USE_FACTORY_BGTR();                       //use factory BandGap tuning value (tuned @ 5.0V)
  return 0;                                     //perform normal initialization
}

void puts_sans_newline(char *s)
{
    while (*s)
        if (putchar(*s++) == EOF)
            return;
}

void main(void)
{
  //setup timer2 (TM2) interrupt for 115200 baud
  TM2C = TM2C_CLK_IHRC;                         //use IHRC -> 16 Mhz
  TM2S = TM2S_PRESCALE_NONE | TM2S_SCALE_DIV2;  //no prescale, scale 2 ~> 8MHz
  TM2B = 68;                                    //divide by (68+1) ~> 115942 Hz (apx. 115200)

  PAC = 0x80;                                   //enable PA.7 as output
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  INTEN = INTEN_TM2;                            //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  //setup ADC
#ifdef ADCRGC
  ADCRGC = ADCRGC_ADC_REF_VDD;                   //ADC reference voltage is VDD
#endif
  ADCM = ADCM_CLK_SYSCLK_DIV16;                 //SYSCLK 8MHZ/16 -> 500 kHz ADC clock
  ADCC = (uint8_t)(ADCC_ADC_ENABLE | ADCC_CH_AD16_BANDGAP);//enable ADC and use channel 16 (internal bandgap voltage 1.2V)
                                                //NOTE: a delay of 400usec is required after initialization, before first ADC conversion can start

  puts_sans_newline("Measuring VDD (VBandGap): ");

  ADCC |= ADCC_START_ADC_CONV;                  //start ADC conversion
  while( !(ADCC & ADCC_IS_ADC_CONV_READY) );    //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
  uint8_t adcval = ADCR;                        //read the ADC value

  //We measured the internal bandgap voltage which should be 1.2V. This means: 1.2V/adcval = VDD/255 -> VDD = (1.2V*255)/adcval
  uint32_t vdd = (1200UL*255)/adcval;
  _uitoa(vdd, str_buf, 10);  // Should really be _ultoa, but that doesn't seem available right now
  puts_sans_newline(str_buf);
  puts(" mV");

  //setup and switch to other ADC channel
  PAC &= ~(1<<0);                               //disable PA.0 GPIO output
  PAPH &= ~(1<<0);                              //disable pull up on PA.0, NOTE: also disable PxPL if available (e.g. on port B)
  PADIER &= ~(1<<0);                            //disable PA.0 GPIO input
  ADCC = (uint8_t)(ADCC_ADC_ENABLE | ADCC_CH_AD10_PA0);    //enable ADC and use channel 10 (PA.0)
                                                //NOTE: a delay of 400usec is required after initialization, before first ADC conversion can start
  puts("Start ADC on PA.0");
  for(;;)
  {
    ADCC |= ADCC_START_ADC_CONV;                //start ADC conversion
    while( !(ADCC & ADCC_IS_ADC_CONV_READY) );  //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)

    uint8_t adcval = ADCR;                      //read the ADC value
    puts_sans_newline("PA.0 : ");
    _uitoa(adcval, str_buf, 10); // Should really be _utoa, but that doesn't seem available right now
    puts(str_buf);
  }
}
