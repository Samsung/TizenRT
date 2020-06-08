#!/bin/bash 

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
BINDIR=${TOP_PATH}/output/bin

cd $THIS_PATH

ENCTOOL=./EncTool

	if [ "$1" != "y" ];then
		cp $THIS_PATH/km4_image3_all.bin $BINDIR/km4_image3_all-en.bin
		cp $THIS_PATH/km4_image3_psram.bin $BINDIR/km4_image3_psram-en.bin
	else
	echo "========== RDP Encryption =========="
		$ENCTOOL rdp $THIS_PATH/km4_image3_all.bin $BINDIR/km4_image3_all-en.bin $2
		$ENCTOOL rdp $THIS_PATH/km4_image3_psram.bin $BINDIR/km4_image3_psram-en.bin $2
	fi

