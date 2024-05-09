#!/usr/bin/env bash
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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
GNUUTL=${BUILDDIR}/tools/amebalite/gnu_utility
TOOL_PATH=${THIS_PATH}/../../tools/amebalite
BOOT_PATH=${TOOL_PATH}/bootloader
FLOADER_PATH=${TOOL_PATH}/flashloader

echo "Realtek Postbuild Actions"
export TOPDIR=$BUILDDIR/

#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libc.a
#arm-none-eabi-readelf -A /root/tizenrt/os/board/rtl8721csm/src/libboard.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libapps.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libapps.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libexternal.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libframework.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libfs.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libkernel.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libmm.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libfs.a
#arm-none-eabi-readelf -A $BUILDDIR/output/libraries/libwque.a


#if [ 1 -eq 0 ]; then
#arm-none-eabi-gcc -T$BUILDDIR/configs/rtl8721csm/scripts/rlx8721d_img2_is.ld -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -nostartfiles -nostdlib -specs nosys.specs -Wl,--gc-sections -Wl,--warn-section-align -Wl,-Map=$BUILDDIR/output/text.map  -Wl,--cref -Wl,--build-id=none -save-temps  -Wl,--no-enum-size-warning  -Wl,--warn-common -o $BUILDDIR/output/target_img2.axf  -Wl,--start-group \
#$BUILDDIR/output/libraries/libc.a \
#/root/tizenrt/os/board/rtl8721csm/src/libboard.a \
#$BUILDDIR/output/libraries/libapps.a \
#$BUILDDIR/output/libraries/libfs.a \
#$BUILDDIR/output/libraries/libarch.a \
#$BUILDDIR/output/libraries/libdrivers.a \
#$BUILDDIR/output/libraries/libexternal.a \
#$BUILDDIR/output/libraries/libframework.a \
#$BUILDDIR/output/libraries/libmm.a \
#$BUILDDIR/output/libraries/libwque.a \
#$BUILDDIR/output/libraries/libkernel.a -Wl,--end-group -lm -lstdc++


#if [ 1 -eq 0 ]; then
#mv $BUILDDIR/output/text.map $BUILDDIR/output/text_image2_ns.map

cp $BINDIR/tinyara.axf $BINDIR/target_img2.axf

arm-none-eabi-nm $BINDIR/target_img2.axf | sort > $BINDIR/target_img2.map

arm-none-eabi-objdump -d $BINDIR/target_img2.axf > $BINDIR/target_img2.asm

#arm-none-eabi-objdump -h -S --disassemble-all $BINDIR/target_img2.axf > $BINDIR/target_img2.txt

cp $BINDIR/target_img2.axf $BINDIR/target_pure_img2.axf

arm-none-eabi-strip $BINDIR/target_pure_img2.axf

arm-none-eabi-objcopy -j .bluetooth_trace.text \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/APP.trace

arm-none-eabi-objcopy -j .sram_only.text.data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/sram_only.bin

arm-none-eabi-objcopy -j .psram_image2.text.data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/psram_2.bin

arm-none-eabi-objcopy -j .sram_image2.text.data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/sram_2.bin

arm-none-eabi-objcopy -j .xip_image2.text -j .ARM.extab -j .ARM.exidx \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/xip_image2.bin

#For Bluetooth Trace
if [ "${CONFIG_BT_EN}" == "y" ];then
	arm-none-eabi-objcopy -j .bluetooth_trace.text \
	-Obinary $BINDIR/target_pure_img2.axf $BINDIR/APP.trace
fi

echo "========== Image Info HEX =========="
arm-none-eabi-size -A --radix=16 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=16 $BINDIR/target_img2.axf

echo "========== Image Info DEC =========="
arm-none-eabi-size -A --radix=10 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=10 $BINDIR/target_img2.axf

echo "========== Image manipulating start =========="

#16Bytes aligned for otf en
$GNUUTL/pad16.sh $BINDIR/sram_2.bin
$GNUUTL/pad16.sh $BINDIR/psram_2.bin
$GNUUTL/pad16.sh $BINDIR/xip_image2.bin

$GNUUTL/prepend_header.sh $BINDIR/sram_only.bin __sram_only_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/sram_2.bin  __sram_image2_start__   $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/psram_2.bin  __psram_image2_start__   $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/xip_image2.bin  __km4_flash_text_start__  $BINDIR/target_img2.map

python $GNUUTL/code_analyze.py

cat $BINDIR/xip_image2_prepend.bin $BINDIR/psram_2_prepend.bin $BINDIR/sram_2_prepend.bin $BINDIR/sram_only_prepend.bin > $BINDIR/km4_image2_all.bin

$GNUUTL/pad.sh $BINDIR/km4_image2_all.bin

if [ "${BUILD_TYPE}" == "MFG" ];then
	cat $GNUUTL/kr4_image2_all.bin $BINDIR/km4_image2_all.bin \
	mv $BINDIR/kr4_km4_image2.bin $BINDIR/kr4_km4_image2_mp.bin
fi

if [ -f $BINDIR/kr4_km4_image2.bin ];then
	$BINDIR/kr4_km4_image2.bin;
fi
echo "========== Image manipulating end =========="

function copy_bootloader()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	if [ "${CONFIG_AMEBALITE_TRUSTZONE}" != "y" ];then
		echo "========== Copy_bootloader for TZ disabled =========="
		cp $BOOT_PATH/km4_boot_all.bin $BINDIR/km4_boot_all.bin
	else
		echo "========== Copy_bootloader for TZ enabled =========="
		cp $BOOT_PATH/km4_boot_all_tz.bin $BINDIR/km4_boot_all.bin
	fi
}

function concatenate_binary_without_signing()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	if [ "${CONFIG_AMEBALITE_TRUSTZONE}" != "y" ];then
		echo "========== Concatenate_binary for TZ disabled =========="
		cat $GNUUTL/cert.bin $GNUUTL/manifest.bin $GNUUTL/kr4_image2_all.bin $BINDIR/km4_image2_all.bin > $BINDIR/kr4_km4_image2.bin
	else
		echo "========== Concatenate_binary for TZ enabled =========="
		cat $GNUUTL/cert.bin $GNUUTL/manifest.bin $GNUUTL/kr4_image2_all.bin $BINDIR/km4_image2_all.bin $BINDIR/km4_image3_all.bin > $BINDIR/kr4_km4_image2.bin
	fi
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
	bash $BUILDDIR/configs/rtl8720e/rtl8720e_signing.sh kernel
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

function copy_km4_image3()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	if [ "${CONFIG_ARCH_FPU}" == "y" ];then
		IMG3_ALL_BIN=$GNUUTL/km4_image3_all_fpu.bin
	else
		IMG3_ALL_BIN=$GNUUTL/km4_image3_all.bin
	fi

	echo "========== Copy km4_image3_all into bin output folder=========="
	if [ "${CONFIG_AMEBALITE_TRUSTZONE}" == "y" ];then
		cp $IMG3_ALL_BIN $BINDIR/km4_image3_all.bin
	fi
}

#*****************************************************************************#
#              COPY ap_image_all into bin output folder                       #
#*****************************************************************************#
function copy_kr4_image()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Copy kr4_image2_all into bin output folder=========="
	cp $GNUUTL/kr4_image2_all.bin $BINDIR/kr4_image2_all.bin
}

copy_bootloader;
copy_flashloader;
copy_km4_image3;
copy_kr4_image;
if [ "${CONFIG_BINARY_SIGNING}" == "y" ];then
	concatenate_binary_with_signing;

	# Binary Signing is not support in public, so copy the non-signing binary
	concatenate_binary_without_signing;
else
	concatenate_binary_without_signing;
fi
