#!/bin/bash

set -e
set -x

if [ -z $TRAVIS_OS_NAME ]; then
    echo "This file is for automated builds only. Please use the make for local builds."
    exit -1
fi

EPDKVER=$(git describe --abbrev=0)

GIT=$(git describe --tags --always)
DATE=$(date +'%Y%m%d')
if [ "_$OSTYPE" = "_firmware" ]; then
  make -C Firmware/source all EPDKVER=\\\"$EPDKVER\\\" EPDKSUB=
  mkdir -p build/EASYPDKPROG_FIRMWARE
  cp Firmware/source/build/EASYPDKPROG.dfu Firmware/LICENCE-ADDITONAL Firmware/README build/EASYPDKPROG_FIRMWARE
  cd build
  zip -r -9 "EASYPDKPROG_FIRMWARE_${DATE}_${GIT}.zip" "EASYPDKPROG_FIRMWARE"
  cd ..
  exit 0
fi

if [ "_$OSTYPE" = "_msys" ]; then
  unset CC
  export CC=i686-w64-mingw32-gcc
  export STRIP=i686-w64-mingw32-strip
fi

make all EPDKVER=$EPDKVER 

DESTDIR="build/EASYPDKPROG"
mkdir -p $DESTDIR

if [ "$OSTYPE" == "msys" ]; then
    OS="WIN"
    cp easypdkprog $DESTDIR/easypdkprog.exe
    cp -r Windows_STM32VCPDriver $DESTDIR
else
    if [[ $OSTYPE =~ darwin.* ]]; then
        OS="MAC"
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
