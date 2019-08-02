#!/bin/bash -e


CURDIR=$(readlink -f "$0")
CURDIR_PATH=$(dirname "$CURDIR")
NXPTOOLS_PATH=${CURDIR_PATH}/../../tools/nxp/imxrt1050

BINFILE=${NXPTOOLS_PATH}/ivt_flashloader.bin
SDPHOST=${NXPTOOLS_PATH}/sdphost
BLHOST=${NXPTOOLS_PATH}/blhost


BASEADDR=0x20000000
IVTADDR=0x20000400
TTYDEV="/dev/ttyACM0"

echo "Connecting the chip ROM ... "
${SDPHOST} -p $TTYDEV error-status
if [ $? -ne 0 ]
then
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

