// Serial echo test (showing send / receive), PA0 = RX / PA7 = TX (57600 BAUD)

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "easypdk/pdk.h" 

volatile uint16_t txdata;                       //tx serial data register
volatile uint8_t  rxdata;                       //rx serial data register
volatile bool rxdata_avail;                     //rxdata holds a new value
static uint8_t rxtxphase;                       //internal phase (4 phases => rx can be shifted to tx by 0%/25%/50%/75%)
static uint8_t rxphaseshift;                    //rx phase shift
static uint8_t rxdatatmp;                       //internal receive shift register
static uint8_t rxctr;                           //internal receive bit counter 

void interrupt(void) __interrupt(0)
{
  if( INTRQ & INTRQ_TM2 )                       //TM2 interrupt request?
  {
    rxtxphase++;                                //update phase
    INTRQ &= ~INTRQ_TM2;                        //mark TM2 interrupt request processed
    if( txdata && (0==(rxtxphase&3)))           //txdata contains bits to send and we are at phase0 timer cycle?
    {
      if( txdata&1 )                            //check bit (1/0) for sending
        __set1( PA, 7 );                        //send 1 on PA7
      else
        __set0( PA, 7 );                        //send 0 on PA7
      txdata >>= 1;                             //shift txdata 
    }

    if( 0==rxctr )                              //idle?
    {
      if( 0 == (PA & 1) )                       //PA0 low (startbit)?
      {
        rxctr = 1;                              //set counter to first bit
        rxphaseshift = rxtxphase&3;             //store the pase shift
      }
    }
    else
    {
      if( (rxtxphase&3) == rxphaseshift )       //correct rx phase?
      {
        if( rxctr<9 )                           //still receiving bits?
        {
          rxdatatmp >>= 1;                      //shift receive shift register
          if( PA & 1 )
            __set1( rxdatatmp, 7 );             //set highest bit
          rxctr++;                              //update rx bit counter
        }
        else                                    //check stop bit
        {
          if( PA & 1 )                          //PA0 high (stopbit)?
          {
            rxdata = rxdatatmp;                 //set rx data
            rxdata_avail = true;                //mark rx data available
          }
          rxctr = 0;                            //set rx idle
        }
      }
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

int getchar(void)
{
  while(!rxdata_avail);                         //wait for character to arrive
  int c = rxdata;                               //get the character
  rxdata_avail = false;                         //reset input buffer
  return c;
}

unsigned char _sdcc_external_startup(void)
{
  EASY_PDK_INIT_SYSCLOCK_8MHZ();                //use 8MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(7833600,4000);        //tune SYSCLK to 7.8336MHz @ 4.000V (we choose this to hit the correct baud rate)
  return 0;                                     //perform normal initialization
}

void main(void)
{
  //setup timer2 (TM2) interrupt for serial
  TM2C = TM2C_CLK_IHRC;                         //use IHRC -> (2 * 7.8336MHz) = 15.6672MHz
  TM2S = TM2S_PRESCALE_NONE | TM2S_SCALE_NONE;  //no prescale, no scale
  TM2B = 33;                                    //divide by (33+1) ~> 230400 Hz (/4 phases = 57600)

  PAC = 0x80;                                   //enable PA.7 as output, PA.0 as input
  PADIER = 0x01;                                //select PA.0 as digital input (required for PFS173)
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  INTEN = INTEN_TM2;                            //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  puts("Echoing serial receive (type something):");

  for(;;)
  {
    putchar(getchar());
  }
}
