#!/bin/bash

set -e
set -x

if [ -z $OSNAME ]; then
    echo "This file is for automated builds only. Please use the make for local builds."
    exit -1
fi

EPDKVER=$(git describe --tags --abbrev=0)

GIT=$(git describe --tags --abbrev=0)

DATE=$(date +'%Y%m%d')
if [ "$OSTYPE" = "firmware" ]; then
  make -C Firmware/source all EPDKVER=\\\"$EPDKVER\\\" EPDKSUB=
  mkdir -p build/EASYPDKPROG_FIRMWARE
  cp Firmware/source/build/EASYPDKPROG.dfu Firmware/LICENCE-ADDITONAL Firmware/README build/EASYPDKPROG_FIRMWARE
  cd build
  zip -r -9 "EASYPDKPROG_FIRMWARE_${DATE}_${GIT}.zip" "EASYPDKPROG_FIRMWARE"
  cd ..
  exit 0
fi

if [ "$OSTYPE" = "msys" ]; then
  unset CC
  export CC=i686-w64-mingw32-gcc
  export STRIP=i686-w64-mingw32-strip
  export OS=Windows_NT
fi

make all EPDKVER=$EPDKVER 

DESTDIR="build/EASYPDKPROG"
mkdir -p $DESTDIR

if [ "$OSTYPE" == "msys" ]; then
    OS="WIN"
    cp easypdkprog.exe $DESTDIR
    cp -r Windows_STM32VCPDriver $DESTDIR
else
    if [[ $OSTYPE =~ darwin.* ]]; then
        OS="MAC"
#        mv easypdkprog easypdkprog.x86_64
#        make distclean 
#        make all EPDKVER=$EPDKVER ECFLAGS="-target arm64-apple-macos11" EHOST="--host=arm64-apple-macos11"
#        mv easypdkprog easypdkprog.arm64
#        lipo -create -output easypdkprog easypdkprog.x86_64 easypdkprog.arm64
#        lipo -archs easypdkprog
        cp easypdkprog $DESTDIR
    else
        OS="LINUX"
        cp easypdkprog $DESTDIR
        cp -r Linux_udevrules $DESTDIR
    fi
fi

cp INSTALL LICENSE $DESTDIR

cp -r Examples $DESTDIR

cd build
zip -r -9 "EASYPDKPROG_${OS}_${DATE}_${GIT}.zip" "EASYPDKPROG"
cd ..
