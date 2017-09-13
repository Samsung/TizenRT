#!/bin/bash

COMPILER_PATH=${PWD}/../../../build_tools/tp6x/cross_compiler/toolchain-mips_r2_gcc-4.6-linaro_eglibc-2.15

if [ "${1}" = "default" ] ; then
    echo "COMPILER_PATH = ${COMPILER_PATH}" 
elif [ -n "${1}" ] ; then
    COMPILER_PATH=${1}
    echo "COMPILER_PATH = ${COMPILER_PATH}"
else
    echo "Not invalid COMPILER_PATH."
    exit 1
fi

RELEASE_MODE=1
if [ -n "${2}" ] ; then
    RELEASE_MODE=${2}
fi

SECURED_MODE=0
if [ -n "${3}" ] ; then
    SECURED_MODE=${3}
fi

TTRANSPORT_MODE=IP
if [ -n "${4}" ] ; then
    TTRANSPORT_MODE=${4}
fi

WITHTCP_MODE=yes
if [ -n "${5}" ] ; then
    WITHTCP_MODE=${5}
fi

scons -c TARGET_OS=linux TARGET_ARCH=mips TARGET_TRANSPORT=${TTRANSPORT_MODE} WITH_TCP=${WITHTCP_MODE} SECURED=${SECURED_MODE} RELEASE=${RELEASE_MODE} TARGET_DEVICE=all TC_PREFIX=${COMPILER_PATH}/bin/mips-openwrt-linux-gnu- TC_PATH=${COMPILER_PATH}/bin/

if eval scons TARGET_OS=linux TARGET_ARCH=mips TARGET_TRANSPORT=${TTRANSPORT_MODE} WITH_TCP=${WITHTCP_MODE} SECURED=${SECURED_MODE} RELEASE=${RELEASE_MODE} TARGET_DEVICE=all TC_PREFIX=${COMPILER_PATH}/bin/mips-openwrt-linux-gnu- TC_PATH=${COMPILER_PATH}/bin/; then
    echo "Controllee build successful!!"
else
    echo "Controllee build failed!!"
    exit 1
fi


exit 0

