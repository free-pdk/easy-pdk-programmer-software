// Hello World! for dual core Padauk ICs, to be compiled with SDCC. Serial output on PA7 at 115200 baud. Activity on PA0 will be detected from second core.

#include <stdint.h>
#include <stdio.h>
#include "easypdk/pdk.h"

//temporary solution since SDCC PDK port does not support crt0.s or some other way to define first instructions directly
//here we simply overwrite the SDCC compiler header init code with same functionality but 2 gotos in front of it
void header(void) __naked
{
__asm__("                             \n\
        .area NEWHEADER (ABS)         \n\
        .org 0x0000                   \n\
        goto $0                       \n\
        goto _core2                   \n\
$0:                                   \n\
        clear p+1                     \n\
        mov a, #s_OSEG                \n\
        add a, #l_OSEG + 1            \n\
        and a, #0xfe                  \n\
        mov.io sp, a                  \n\
        call __sdcc_external_startup  \n\
        goto s_GSINIT                 \n\
        .area CODE                    \n");
}

volatile uint16_t txdata;                       //txdata, serial data shift register

void interrupt(void) __interrupt(0)
{
  if( INTRQ & INTRQ_TM2 )                       //TM2 interrupt request?
  {
    INTRQ &= ~INTRQ_TM2;                        //mark TM2 interrupt request processed
    if( txdata )                                //txdata contains bits to send?
    {
      if( txdata&1 )                            //check bit (1/0) for sending
        __set1io( PA, 7 );                      //send 1 on PA7
      else
        __set0io( PA, 7 );                      //send 0 on PA7
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
//  EASY_PDK_USE_FACTORY_IHRCR_16MHZ();
  EASY_PDK_CALIBRATE_IHRC(8000000,5000);        //tune SYSCLK to 8MHz @ 5.000V
  return 0;                                     //perform normal initialization
}

//used as signal from core2 - needs to be volatile
volatile uint8_t core2_signal;

void main(void)
{
  FPPEN = FPPEN_ENABLE_FPPA0|FPPEN_ENABLE_FPPA1;//enable both cores, this causes SYSCLOCK to be only 50% for every core

  //setup timer2 (TM2) interrupt for 115200 baud
  TM2C = TM2C_CLK_IHRC;                         //use IHRC -> 16 Mhz
  TM2S = TM2S_PRESCALE_NONE | TM2S_SCALE_DIV2;  //no prescale, scale 2 ~> 8MHz
  TM2B = 69;                                    //divide by 69 ~> 115942 Hz (apx. 115200)

  PAC = 0x80;                                   //enable PA.7 as output
  txdata = 0xD55F;                              //setup 2 stop bits, 0x55 char for autobaud, 1 start bit, 5 stop bits
  INTEN = INTEN_TM2;                            //enable TM2 interrupt, send out initial stop bits and autobaud char
  __engint();                                   //enable global interrupts

  puts("Press a key on the host computer to send some bits, core2 will detect this and core1 will output a '2' for every 1 bit");

  for(;;)
  {
    puts("Hello World!");

    for(uint32_t i=750000; i>0; i--)            //wait approx. 3s
    {
      if( core2_signal )                        //in case core2 detected a bit on PA0, core2_signal variable will be set
      {
        core2_signal = 0;                       //reset core2_signal
        putchar('2');                           //output character '2'
      }
    }
  }
}

//core2 entry point (execution needs to be enabled in FPPEN register)
//keep in mind :
// - you have to setup your own stack (SP) in case you want to call other functions
// - you can not use C code or call other C functions (this might use the virtual p register and global variables for passing arguments)
// - just use this core for very basic things, e.g. set flags to memory to signal core0 to process them
void core2(void) __naked
{
__asm__("                        \n\
        mov a, #0x01             \n\
        mov.io __padier, a       \n\
$1:                              \n\
        t1sn.io __pa, #0         \n\
        set1 _core2_signal, #0   \n\
        goto $1                  \n");
}
