// ADC test (requires ADC capable IC like PFS173). ADC pin on PA.0, serial output of ADC values on PA7 at 115200 baud.

#include <stdint.h>
#include <stdio.h>
#include "easypdk/pdk.h" 

volatile uint16_t txdata;                       //txdata, serial data shift register

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
  EASY_PDK_INIT_SYSCLOCK_8MHZ();                //use 8MHz sysclock
  EASY_PDK_USE_FACTORY_IHRCR_16MHZ()            //use factory IHCR tuning value (tuned for 8MHz SYSCLK @ 5.0V)
  EASY_PDK_USE_FACTORY_BGTR();                  //use factory BandGap tuning value (tuned @ 5.0V)
  return 0;                                     //perform normal initialization
}

void main(void)
{
  //setup timer2 (TM2) interrupt for 115200 baud
  TM2C = TM2C_CLK_IHRC;                         //use IHRC -> 16 Mhz
  TM2S = TM2S_PRESCALE_NONE | TM2S_SCALE_DIV2;  //no prescale, scale 2 ~> 8MHz
  TM2B = 69;                                    //divide by 69 ~> 115942 Hz (apx. 115200)

  PAC = 0x80;                                   //enable PA.7 as output
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  INTEN = INTEN_TM2;                            //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  //setup ADC
#ifdef ADCRGC
  ADCRGC = ADCRG_ADC_REF_VDD;                   //ADC reference voltage is VDD
#endif
  ADCM = ADCM_CLK_SYSCLK_DIV16;                 //SYSCLK 8MHZ/16 -> 500 kHz ADC clock
  ADCC = ADCC_ADC_ENABLE | ADCC_CH_AD15_BANDGAP;//enable ADC and use channel 16 (internal bandgap voltage 1.2V)
                                                //NOTE: a delay of 400usec is required after initialization, before first ADC conversion can start

  //printf has a huge footprint (big CODE + big RAM). Do not use it in your projects unless you have to.  
  printf("Measuring VDD (VBandGap): ");

  ADCC |= ADCC_ADC_CONV_START;                  //start ADC conversion
  while( !(ADCC & ADCC_ADC_CONV_COMPLETE) );    //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)
  uint8_t adcval = ADCR;                        //read the ADC value

  //We measured the internal bandgap voltage which should be 1.2V. This means: 1.2V/adcval = VDD/255 -> VDD = (1.2V*255)/adcval
  uint32_t vdd = (1200UL*255)/adcval;
  printf("%lu mV\n",vdd);


  //setup and switch to other ADC channel
  PAC &= ~(1<<0);                               //disable PA.0 GPIO output
  PAPH &= ~(1<<0);                              //disable pull up on PA.0, NOTE: also disable PxPL if available (e.g. on port B)
  PADIER &= ~(1<<0);                            //disable PA.0 GPIO input
  ADCC = ADCC_ADC_ENABLE | ADCC_CH_AD10_PA0;    //enable ADC and use channel 10 (PA.0)
                                                //NOTE: a delay of 400usec is required after initialization, before first ADC conversion can start
  printf("Start ADC on PA.0\n");
  for(;;)
  {
    ADCC |= ADCC_ADC_CONV_START;                //start ADC conversion
    while( !(ADCC & ADCC_ADC_CONV_COMPLETE) );  //busy wait for ADC conversion to finish (we also could use the ADC interrupt...)

    uint8_t adcval = ADCR;                      //read the ADC value
    printf("PA.0 : %d\r", adcval);
  }
}
