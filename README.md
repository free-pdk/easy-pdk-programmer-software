EASY PDK PROGRAMMER SOFTWARE
[![License](https://img.shields.io/:license-gpl3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0.html) [![Build Status](https://travis-ci.org/free-pdk/easy-pdk-programmer-software.svg?branch=master)](https://travis-ci.org/free-pdk/easy-pdk-programmer-software) [![Downloads](https://img.shields.io/github/downloads/free-pdk/easy-pdk-programmer-software/total.svg?maxAge=3600)](https://github.com/free-pdk/easy-pdk-programmer-software/releases/latest)
----------------------------

Download: [![Release](https://img.shields.io/github/release/free-pdk/easy-pdk-programmer-software.svg?maxAge=60)](https://github.com/free-pdk/easy-pdk-programmer-software/releases/latest) <== click here to download.


```
Usage: easypdkprog [OPTION...] list|probe|read|write|erase|start [FILE]
easypdkprog -- read, write and execute programs on PADAUK microcontroller
https://free-pdk.github.io

      --allowsecfuse         Allow setting the security fuse.
  -b, --bin                  Binary file output. Default: ihex8
  -f, --fuse=FUSE            FUSE value, e.g. 0x31FD
  -i, --icid=ID              IC ID 12 bit, e.g. 0xAA1
      --noblankchk           Skip blank check before write
      --nocalibrate          Skip calibration after write.
      --noerase              Skip erase before write
  -n, --icname=NAME          IC name, e.g. PFS154
      --noverify             Skip verify after write
  -p, --port=PORT            COM port of programmer. Default: Auto search
  -r, --runvdd=VDD           Voltage for running the IC. Default: 5.0
      --securefill           Fill unused space with 0 (NOP) to prevent readout
  -s, --serial=SERIAL        SERIAL value (64bit), e.g. 0x123456789ABCDEF0
  -v, --verbose              Verbose output
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

Examples:

list all supported ICs:
```  easypdkprog list```

probe IC in socket (can determine IC type/name):
```  easypdkprog probe```

read IC to readout.hex file:
```  easypdkprog -n PFS154 read readout.hex```

read IC to readout.bin file:
```  easypdkprog -n PFS154 read readout.hex -b```

write IC:
```  easypdkprog -n PFS154 write myprog.hex```

erase IC (flash based only):
```  easypdkprog -n PFS154 erase```

start IC in socket (interactive mode):
 all serial output sent form IC-PA.7 (autobaud detection) is displayed on screen
 all input from keyboard is sent as serial to IC-PA.0 (using same detected baud as receive)
 
```  easypdkprog start```

  
Hardware sources can be found here: https://github.com/free-pdk/easy-pdk-programmer-hardware

