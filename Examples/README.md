Example programs for Padauk ICs.
================================

Easy PDK programmer can capture serial data and shows it as output in easypdkprog (baud rate is autodetected, first character sent must be 0x55 for autobaud).

* Helloworld is a sample program which shows how to setup the processor and how to send a "Hello World!" string over a software emulated serial output on PA7.

* ADCtest shows how to use the ADC. First it measures the internal bandgap voltage of 1.2V and can then estimate VDD from it. Then it constantly measures and outputs the ADC value on PA.0.

* COMPtest shows how to use the comparator to estimate VDD using internal bandgap voltage of 1.2V and the internal resistor ladder.

* PolySound is PWM based sound player emulating the 4 channel NES sound chip. Connect a speaker to PA.3 and GND. Original project from Joel Yliluoma : https://bisqwit.iki.fi/jutut/kuvat/programming_examples/pic/

**INSERT IC**

You can insert any SOP based PMS150C/PMS171B/PFS154/PFS173 IC into a SOP socket connected directly to Easy PDK programmer.

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

PFS172:
```
$ ./easypdkprog --icname=PFS172  write helloworld_pfs172.ihx
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

PMS171B:
```
$ ./easypdkprog -n PMS171B write helloworld_pms171b.ihx
Writing IC (186 words)... done.
Calibrating IC
* IHRC SYSCLK=8000000Hz @ 4.00V ... calibration result: 7982674Hz (0x41)  done.
```

**RUN PROGRAM ON IC**

```
$ ./easypdkprog --runvdd=4.0 start
Hello World!
Hello World!
Hello World!
```
