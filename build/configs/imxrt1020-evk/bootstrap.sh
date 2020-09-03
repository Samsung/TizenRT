#!/bin/bash -e


CURDIR=$(readlink -f "$0")
CURDIR_PATH=$(dirname "$CURDIR")

BINFILE=${CURDIR_PATH}/../../tools/nxp/imxrt1020/ivt_flashloader.bin
SDPHOST=${CURDIR_PATH}/../../tools/nxp/imxrt1020/sdphost
BLHOST=${CURDIR_PATH}/../../tools/nxp/imxrt1020/blhost

BASEADDR=0x20208000
IVTADDR=0x20208400
TTYDEV="/dev/ttyACM0"

echo "Connecting the chip ROM ... "
${SDPHOST} -p $TTYDEV error-status
if [ $? -ne 0 ]
then
    echo -e "## CHECK BOOTMODE PIN SETTINGS & RESET \n"
    exit 1
fi

echo
echo "Download $BINFILE to $BASEADDR"
${SDPHOST} -p $TTYDEV -- write-file $BASEADDR $BINFILE
if [ $? -ne 0 ]
then
    exit 1
fi

echo
echo "Start at $IVTADDR"
${SDPHOST} -p $TTYDEV -- jump-address $IVTADDR
if [ $? -ne 0 ]
then
    exit 1
fi
echo

echo "wait ..."

sleep 2
${BLHOST} -p $TTYDEV -- get-property 1
if [ $? -ne 0 ]
then
    exit 1
fi

