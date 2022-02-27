#!/bin/bash

export RASPBIAN_ROOTFS=$HOME/rpi/rootfs
#export PATH=/opt/cross-pi-gcc/bin:/opt/cross-pi-gcc/libexec/gcc/arm-linux-gnueabihf/8.3.0:$PATH
export PATH=$PATH:~/rpi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/

export RASPBERRY_VERSION=2
export ARM_COMPILATION=1

cd build_rpi 
rm * -R
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi_PhBe.cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUA_NAMESPACE_ZERO=FULL ..