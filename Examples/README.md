Hello World! for Padauk ICs.
============================

Helloworld is a sample program which show how to setup the processor and how to send a "Hello World!" string over a software emulated serial output on PA7.

Easy PDK programmer can capture serial data and shows it as output in easypdkprog (baud rate is autodetected, first character sent must be 0x55 for autobaud).


**INSERT IC**

You can insert any SOP based PMS150C/PFS154/PFS173 IC into a SOP socket connected directly to Easy PDK programmer.


**CHECK IC**

 easypdkprog probe


**WRITE PROGRAM TO IC**

PFS154:
 easypdkprog --icname=PFS154  write helloworld_pfs154.ihx

PFS173:
 easypdkprog --icname=PFS173  write helloworld_pfs173.ihx

PMS150C:
 easypdkprog --icname=PMS150C write helloworld_pms150c.ihx


**RUN PROGRAM ON IC**

 easypdkprog --runvdd=4.0 start

Hello World!
Hello World!
Hello World!
...
