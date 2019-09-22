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
  if( _intrq & INTRQ_TM2 )                      //TM2 interrupt request?
  {
    rxtxphase++;                                //update phase
    _intrq &= ~INTRQ_TM2;                       //mark TM2 interrupt request processed
    if( txdata && (0==(rxtxphase&3)))           //txdata contains bits to send and we are at phase0 timer cycle?
    {
      if( txdata&1 )                            //check bit (1/0) for sending
        __set1( _pa, 7 );                       //send 1 on PA7
      else
        __set0( _pa, 7 );                       //send 0 on PA7
      txdata >>= 1;                             //shift txdata 
    }

    if( 0==rxctr )                              //idle?
    {
      if( 0 == (_pa & 1) )                      //PA0 low (startbit)?
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
          if( _pa & 1 )
            __set1( rxdatatmp, 7 );             //set highest bit
          rxctr++;                              //update rx bit counter
        }
        else                                    //check stop bit
        {
          if( _pa & 1 )                         //PA0 high (stopbit)?
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
  _inten &= ~INTEN_TM2;                         //disable TM2 (setup of 16 bit value txdata is non atomic)
  txdata = (c << 1) | 0x200;                    //setup txdata with start and stop bit
  _inten |= INTEN_TM2;                          //enable TM2
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
  //setup timer2 (TM2) interrupt for 115200 baud
  _tm2c = TM2C_CLK_IHRC;                        //use IHRC -> (2 * 7.8336MHz) = 15.6672MHz
  _tm2s = TM2S_PRESCALE_NONE | TM2S_SCALE_NONE; //no prescale, no scale
  _tm2b = 33;                                   //divide by (33+1) ~> 230400 Hz (/4 phases = 57600)

  _pac = 0x80;                                  //enable PA.7 as output
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  _inten = INTEN_TM2;                           //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  puts("Echoing serial receive (type something):");

  for(;;)
  {
    putchar(getchar());
  }
}
