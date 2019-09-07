Hello World! for Padauk ICs.
============================

Helloworld is a sample program which shows how to setup the processor and how to send a "Hello World!" string over a software emulated serial output on PA7.

Easy PDK programmer can capture serial data and shows it as output in easypdkprog (baud rate is autodetected, first character sent must be 0x55 for autobaud).


**INSERT IC**

You can insert any SOP based PMS150C/PFS154/PFS173 IC into a SOP socket connected directly to Easy PDK programmer.

**CHECK IC**
```
$ ./easypdkprog probe
Probing IC... found.
TYPE:FLASH RSP:0xAA1 VPP=4.50 VDD=2.00
IC is supported: PFS154 ICID:0xAA1
```

**WRITE PROGRAM TO IC**

PFS154:
```
$ ./easypdkprog --icname=PFS154  write helloworld_pfs154.ihx
Erasing IC... done.
Writing IC... done.
Calibrating IC (@4.00V IHRC SYSCLK=8000000Hz)... calibration result: 7946104Hz (0x84)  done.
```

PFS173:
```
$ ./easypdkprog --icname=PFS173  write helloworld_pfs173.ihx
Erasing IC... done.
Writing IC... done.
Calibrating IC (@4.00V IHRC SYSCLK=8000000Hz)... calibration result: 7946104Hz (0x84)  done.
```

PMS150C:
```
$ ./easypdkprog --icname=PMS150C write helloworld_pms150c.ihx
Writing IC... done.
Calibrating IC (@4.00V IHRC SYSCLK=8000000Hz)... calibration result: 7946104Hz (0x84)  done.
```


**RUN PROGRAM ON IC**

```
$ ./easypdkprog --runvdd=4.0 start
Hello World!
Hello World!
Hello World!
```
