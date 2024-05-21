#!/usr/bin/env bash
###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
CONFIG=${TOP_PATH}/os/.config
BUILDDIR=${TOP_PATH}/build
BINDIR=${BUILDDIR}/output/bin
GNUUTL=${BUILDDIR}/tools/amebasmart/gnu_utility
TOOL_PATH=${THIS_PATH}/../../tools/amebasmart
BOOT_PATH=${TOOL_PATH}/bootloader
FLOADER_PATH=${TOOL_PATH}/flashloader

echo "Realtek Postbuild Actions"
export TOPDIR=$BUILDDIR/

cp $BINDIR/tinyara.axf $BINDIR/target_img2.axf

arm-none-eabi-nm $BINDIR/target_img2.axf | sort > $BINDIR/target_img2.map

arm-none-eabi-objdump -d $BINDIR/target_img2.axf > $BINDIR/target_img2.asm

cp $BINDIR/target_img2.axf $BINDIR/target_pure_img2.axf

arm-none-eabi-strip $BINDIR/target_pure_img2.axf

arm-none-eabi-objcopy -j .code -j .data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/ram_2.bin

arm-none-eabi-objcopy -j .xip_image2.text -j .ARM.extab -j .ARM.exidx -j .ctors -j .dtors -j .preinit_array -j .init_array -j .fini_array \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/xip_image2.bin

echo "========== Image Info HEX =========="
arm-none-eabi-size -A --radix=16 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=16 $BINDIR/target_img2.axf
echo "========== Image Info HEX =========="

echo "========== Image Info DEC =========="
arm-none-eabi-size -A --radix=10 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=10 $BINDIR/target_img2.axf
echo "========== Image Info DEC =========="

echo "========== linker img2 end =========="

echo "========== Image manipulating start =========="

cat $BINDIR/ram_2.bin > $BINDIR/ca32_image2_all.bin

python $GNUUTL/code_analyze.py

python $GNUUTL/rtk_data_binary.py

echo "========== Image manipulating end =========="

cp $GNUUTL/bl1_sram.bin $BINDIR/bl1_sram.bin
cp $GNUUTL/bl1.bin $BINDIR/bl1.bin

$GNUUTL/pad_align.sh $BINDIR/xip_image2.bin 32
$GNUUTL/fiptool update $BINDIR/fip.bin --tb-fw $GNUUTL/bl2.bin --tos-fw $GNUUTL/bl32.bin --nt-fw $BINDIR/ca32_image2_all.bin

if [ ! -f $BINDIR/fip.bin ] ; then
	echo "No fip.bin"
	exit 1
fi

$GNUUTL/prepend_header.sh $BINDIR/xip_image2.bin  __flash_text_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/bl1_sram.bin  __ca32_bl1_sram_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/bl1.bin  __ca32_bl1_dram_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/fip.bin  __ca32_fip_dram_start__  $BINDIR/target_img2.map

$GNUUTL/prepend_header.sh $GNUUTL/rtk_ext_flash_data.bin  __flash_ext_data_start__  $BINDIR/target_img2.map
cp $GNUUTL/rtk_ext_flash_data_prepend.bin $BINDIR/rtk_ext_flash_data.bin
rm $GNUUTL/rtk_ext_flash_data.bin
rm $GNUUTL/rtk_ext_flash_data_prepend.bin

cat $BINDIR/xip_image2_prepend.bin $BINDIR/bl1_sram_prepend.bin $BINDIR/bl1_prepend.bin $BINDIR/fip_prepend.bin > $BINDIR/ap_image_all.bin

$GNUUTL/imagetool_hp.sh $BINDIR/ap_image_all.bin



function copy_bootloader()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Copy_bootloader =========="
	if [ ! -f $BOOT_PATH/km4_boot_all.bin ];then
		echo "No km4_boot_all.bin"
		exit 1
	fi
	cp $BOOT_PATH/km4_boot_all.bin $BINDIR/km4_boot_all.bin

}

function concatenate_binary_without_signing()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

 	echo "========== Concatenate_binary =========="

	if [ ! -f $BINDIR/km0_km4_app.bin ] || [ ! -f $BINDIR/ap_image_all.bin ];then
		echo "No km0_km4_app.bin or ap_image_all.bin"
		exit 1
	fi
 	cp $BINDIR/km0_km4_app.bin $GNUUTL/km0_km4_app_temp.bin
 	$GNUUTL/rmcert.sh $GNUUTL/km0_km4_app_temp.bin
 	$GNUUTL/pad.sh $GNUUTL/km0_km4_app_temp.bin
 	cat $GNUUTL/cert.bin $GNUUTL/km0_km4_app_temp.bin $BINDIR/ap_image_all.bin > $BINDIR/km0_km4_ap_image_all.bin
 	rm -rf $GNUUTL/km0_km4_app_temp.bin

}

function concatenate_binary_with_signing()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	#signing
	echo "========== Binary SIGNING =========="
	bash $BUILDDIR/configs/rtl8730e/rtl8730e_signing.sh kernel
}

#*****************************************************************************#
#              COPY flashloader into bin output folder                        #
#*****************************************************************************#
function copy_flashloader()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Copy flashloader into bin output folder=========="
	cp $FLOADER_PATH/flash_loader_ram_1.bin $BINDIR/flash_loader_ram_1.bin
	cp $FLOADER_PATH/target_FPGA.axf $BINDIR/target_FPGA.axf
}

#*****************************************************************************#
#              COPY km0_km4_image_all into bin output folder                       #
#*****************************************************************************#
function copy_km0_km4_image()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Copy ap_image_all into bin output folder=========="
	if [ ! -f $GNUUTL/km0_km4_app.bin ];then
		echo "No km0_km4_app.bin"
		exit 1
	fi
	cp $GNUUTL/km0_km4_app.bin $BINDIR/km0_km4_app.bin
}

#*****************************************************************************#
#              Temporary: Remove the Large Size Binary                        #
#*****************************************************************************#
function remove_large_binary_temp()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Remove large size binary, temporary fix=========="
	if [ ! -f $BINDIR/tinyara.axf.bin ];then
		echo "No tinyara.axf.bin"
		exit 1
	fi
	rm $BINDIR/tinyara.axf.bin
}


copy_bootloader;
copy_flashloader;
copy_km0_km4_image;
if [ "${CONFIG_BINARY_SIGNING}" == "y" ];then
	concatenate_binary_with_signing;

	# Binary Signing is not support in public, so copy the non-signing binary
	concatenate_binary_without_signing;
else
	concatenate_binary_without_signing;
fi
remove_large_binary_temp;
 
