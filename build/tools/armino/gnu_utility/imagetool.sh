#!/bin/bash 

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
BINDIR=${TOP_PATH}/output/bin
CONFIG=${TOP_PATH}/../os/.config
source ${CONFIG}

if [ "${CONFIG_ARCH_FPU}" == "y" ];then
IMG3_ALL_BIN=$THIS_PATH/km4_image3_all_fpu.bin
IMG3_PSRAM_BIN=$THIS_PATH/km4_image3_psram_fpu.bin
else
IMG3_ALL_BIN=$THIS_PATH/km4_image3_all.bin
IMG3_PSRAM_BIN=$THIS_PATH/km4_image3_psram.bin
fi

cd $THIS_PATH
ENCTOOL=./EncTool

	if [ "$1" != "y" ];then
		cp $IMG3_ALL_BIN $BINDIR/km4_image3_all-en.bin
		cp $IMG3_PSRAM_BIN $BINDIR/km4_image3_psram-en.bin
	else
	echo "========== RDP Encryption =========="
		$ENCTOOL rdp $IMG3_ALL_BIN $BINDIR/km4_image3_all-en.bin $2
		$ENCTOOL rdp $IMG3_PSRAM_BIN $BINDIR/km4_image3_psram-en.bin $2
	fi

