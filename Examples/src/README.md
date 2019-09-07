To compile the samples you need an SDCC installation and set the location in Makefile

There are snapshots available here: http://sdcc.sourceforge.net/snap.php

Download the latest SDCC snapshot and unpack it.

Modify the first line of the Makefile to point to your SDCC installation:

```
  SDCC=../../sdcc/bin/sdcc
```

Make sure to enter the complete path to the SDCC executable (sdcc.exe on Windows).

Then use make to compile the samples:
```
  make 
```

Compilation will create a build folder and you can find the compiled (*.ihx) files there.
