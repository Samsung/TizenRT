#!/bin/bash
###########################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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
GNUUTL=${BUILDDIR}/tools/amebad/gnu_utility

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

arm-none-eabi-nm $BINDIR/target_pure_img2.axf | sort > $BINDIR/target_pure_img2.map

arm-none-eabi-objcopy -Obinary $BINDIR/target_pure_img2.axf $BINDIR/target_img2.bin

arm-none-eabi-objcopy -j .ram_image2.entry -j .ram_image2.text -j .ram_image2.data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/ram_2.bin

arm-none-eabi-objcopy -j .xip_image2.text \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/xip_image2.bin

arm-none-eabi-objcopy -j .psram_image2.text -j .psram_image2.data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/psram_2.bin

arm-none-eabi-size -A --radix=16 $BINDIR/target_img2.axf

arm-none-eabi-size -t --radix=16 $BINDIR/target_img2.axf

echo "========== Image manipulating start =========="
$GNUUTL/prepend_header.sh $BINDIR/ram_2.bin  __ram_image2_text_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/xip_image2.bin  __flash_text_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/psram_2.bin  __psram_image2_text_start__  $BINDIR/target_img2.map
python $GNUUTL/code_analyze.py

cat $BINDIR/xip_image2_prepend.bin $BINDIR/ram_2_prepend.bin $BINDIR/psram_2_prepend.bin  > $BINDIR/km4_image2_all.bin

$GNUUTL/pad.sh $BINDIR/km4_image2_all.bin

function image_encryption()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	if [ "${CONFIG_AMEBAD_TRUSTZONE}" == "y" ];then
		RDP_KEY="${CONFIG_AMEBAD_RDP_KEY:2}"
		$GNUUTL/imagetool.sh ${CONFIG_AMEBAD_RDP} ${RDP_KEY}
	fi
}
image_encryption;

function concatenate_binary()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	if [ "${CONFIG_AMEBAD_TRUSTZONE}" != "y" ];then
		echo "========== Concatenate_binary for TZ disabled =========="
		cat $BINDIR/xip_image2_prepend.bin $BINDIR/ram_2_prepend.bin $BINDIR/psram_2_prepend.bin  > $BINDIR/km4_image2_all.bin
		cat $GNUUTL/km0_image2_all.bin $BINDIR/km4_image2_all.bin  > $BINDIR/km0_km4_image2.bin
	else
		echo "========== Concatenate_binary for TZ enabled =========="
		cat $GNUUTL/km0_image2_all.bin $BINDIR/km4_image2_all.bin $BINDIR/km4_image3_all-en.bin $BINDIR/km4_image3_psram-en.bin > $BINDIR/km0_km4_image2.bin
	fi
}
concatenate_binary;

#fi

