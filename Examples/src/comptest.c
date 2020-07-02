// COMPARATOR test. VDD estimation using internal VBandGap (1.2V) reference, serial output of comparator results on PA7 at 19200 baud.
//
// We use 1MHz as sysclock so we can test with much lower VDD:
//
//  easypdkprog start --runvdd=5.0
//  easypdkprog start --runvdd=3.3
//  easypdkprog start --runvdd=2.0

#include <stdint.h>
#include <stdio.h>
#include "pdk/device.h"
#include "easy-pdk/calibrate.h"

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
  PDK_USE_1MHZ_IHRC_SYSCLOCK();                 //use 1MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(1000000,5000);        //tune SYSCLK to 1MHz @ 5.000V
  PDK_USE_FACTORY_BGTR();                       //use factory BandGap tuning value (tuned @ 5.0V)
  return 0;                                     //perform normal initialization
}

void main(void)
{
  //setup timer2 (TM2) interrupt for 115200 baud
  TM2C = TM2C_CLK_IHRC;                         //use IHRC -> 16 Mhz
  TM2S = TM2S_PRESCALE_NONE | TM2S_SCALE_DIV8;  //no prescale, scale 8 ~> 2MHz
  TM2B = 103;                                   //divide by (103+1) ~> 19230 Hz (apx. 19200)

  PAC = 0x80;                                   //enable PA.7 as output
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  INTEN = INTEN_TM2;                            //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  //comparator VBandGap / VInt (range 3: VDD = [ 40 / (N+9) ] * 1.20 V ;
  static const char vddcompstr[16][5] = {"5.33","4.80","4.36","4.00","3.69","3.43","3.20","3.00","2.82","2.67","2.53","2.40","2.29","2.18","2.09","2.00"};

  //setup COMPARATOR: IN- = VBandGap, IN+ = VInt (selected by GPCS)
  GPCC = (uint8_t)(GPCC_COMP_ENABLE | GPCC_COMP_MINUS_BANDGAP_1V2 | GPCC_COMP_PLUS_VINT_R);

  for(;;)
  {
    puts("VDD is:");
    for( uint8_t n=0; n<16; n++ )               //loop over all 16 values of the VInt_R resistor ladder
    {
      GPCS = GPCS_COMP_RANGE3 | n;               //range 3 covers 2.0V - 5.33V
      for(uint32_t d=100; d>0; d--);            //small delay after GPCS setup required

      if( GPCC & GPCC_COMP_RESULT_POSITIV )     //test if comparator result is positiv
        putchar('>');
      else
        putchar('<');

      puts(vddcompstr[n]);                      //output the coresponding VDD value (we use a string table to avoid printf)
    }

    for(uint32_t i=180000; i>0; i--);           //wait approx. 3s
  }
}
