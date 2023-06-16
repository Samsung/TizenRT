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

# Attach original RTK postbuild script for debug

# linker_image2:
	# @echo "========= linker img2 start ========="
	# $(ECHO) "  LD      target_img2.axf"

	# $(Q)$(LD) $(LD_ARG) target_img2.axf $(RAM_OBJS_LIST) $(LINK_ROM_LIB) $(LINK_APP_LIB) $(LINK_OS_LIB) \
	# $(ASDK_TOOLCHAIN)/lib/gcc/arm-none-eabi/10.2.0/v7-a+neon-fp16/fpu/crtbegin.o \
	# $(ASDK_TOOLCHAIN)/lib/gcc/arm-none-eabi/10.2.0/v7-a+neon-fp16/fpu/crtend.o -lm -lstdc++

	# $(Q)$(MOVE) text.map $(IMAGE_TARGET_FOLDER)/text_image2.map
	# $(Q)$(MOVE) target_img2.axf $(IMAGE_TARGET_FOLDER)
	# $(Q)$(NM) $(IMAGE_TARGET_FOLDER)/target_img2.axf | sort > $(IMAGE_TARGET_FOLDER)/target_img2.map
	# $(Q)$(OBJDUMP) -d $(IMAGE_TARGET_FOLDER)/target_img2.axf > $(IMAGE_TARGET_FOLDER)/target_img2.asm
	# @#$(OBJDUMP) -h -S --disassemble-all $(IMAGE_TARGET_FOLDER)/target_img2.axf > $(IMAGE_TARGET_FOLDER)/target_img2.txt
	# $(Q)$(COPY) $(IMAGE_TARGET_FOLDER)/target_img2.axf $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf
	# $(Q)$(STRIP) $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf
	# @#$(Q)$(NM) $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf | sort > $(IMAGE_TARGET_FOLDER)/target_pure_img2.map
	# @# $(FROMELF) -Obinary $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf $(IMAGE_TARGET_FOLDER)/target_img2.bin

	# $(ECHO) "  BIN      ca32_image2_all.bin"
	# $(Q)$(FROMELF) -j .text -j .ARM.extab -j .ARM.exidx -j .ctors -j .dtors -j .preinit_array -j .init_array -j .fini_array -j .data -j .mmu_tbl \
	# -Obinary $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf $(IMAGE_TARGET_FOLDER)/ram_2.bin

	# @#$(FROMELF) -j .xip_image2.text \
	# #-Obinary $(IMAGE_TARGET_FOLDER)/target_pure_img2.axf $(IMAGE_TARGET_FOLDER)/xip_image2.bin

	# @echo "========== Image Info HEX =========="
	# $(Q)$(CC_SIZE) -A --radix=16 $(IMAGE_TARGET_FOLDER)/target_img2.axf
	# $(Q)$(CC_SIZE) -t --radix=16 $(IMAGE_TARGET_FOLDER)/target_img2.axf
	# @echo "========== Image Info HEX =========="

	# @echo "========== Image Info DEC =========="
	# $(Q)$(CC_SIZE) -A --radix=10 $(IMAGE_TARGET_FOLDER)/target_img2.axf
	# $(Q)$(CC_SIZE) -t --radix=10 $(IMAGE_TARGET_FOLDER)/target_img2.axf
	# @echo "========== Image Info DEC =========="

	# $(Q)$(RM) -f $(RAM_OBJS_LIST)
	# @echo "========== linker img2 end =========="

	# @echo "========== Image manipulating start =========="
	# @#$(Q)$(PREPENDTOOL) $(IMAGE_TARGET_FOLDER)/ram_2.bin  __ram_image2_text_start__  $(IMAGE_TARGET_FOLDER)/target_img2.map
	# @#$(PREPENDTOOL) $(IMAGE_TARGET_FOLDER)/xip_image2.bin  __flash_text_start__  $(IMAGE_TARGET_FOLDER)/target_img2.map

	# $(Q)cat $(IMAGE_TARGET_FOLDER)/ram_2.bin > $(IMAGE_TARGET_FOLDER)/ca32_image2_all.bin

	# $(Q)$(CODE_ANALYZE_PYTHON)

	# @#cat $(IMAGE_TARGET_FOLDER)/xip_image2_prepend.bin $(IMAGE_TARGET_FOLDER)/ram_2_prepend.bin > $(IMAGE_TARGET_FOLDER)/ca32_image2_all.bin
	# @#$(UTILITYDIR)/pad.sh $(IMAGE_TARGET_FOLDER)/km0_image2_all.bin

	# @#$(UTILITYDIR)/image_tool/imagetool.sh $(IMAGE_TARGET_FOLDER)/km0_image2_all.bin $(KM4_BUILDDIR)/asdk/image

	# @echo "========== Image manipulating end =========="

# fip_image:
	# @make -C make/atf image
	# $(Q)$(PREPENDTOOL) $(IMAGE_TARGET_FOLDER)/bl1_sram.bin  __ca32_bl1_sram_start__  $(IMAGE_TARGET_FOLDER)/target_img2.map
	# $(Q)$(PREPENDTOOL) $(IMAGE_TARGET_FOLDER)/bl1.bin  __ca32_bl1_dram_start__  $(IMAGE_TARGET_FOLDER)/target_img2.map
	# $(Q)$(PREPENDTOOL) $(IMAGE_TARGET_FOLDER)/fip.bin  __ca32_fip_dram_start__  $(IMAGE_TARGET_FOLDER)/target_img2.map
	# @cat $(IMAGE_TARGET_FOLDER)/bl1_sram_prepend.bin $(IMAGE_TARGET_FOLDER)/bl1_prepend.bin $(IMAGE_TARGET_FOLDER)/fip_prepend.bin > $(IMAGE_TARGET_FOLDER)/ap_image_all.bin
	# $(KM4_BUILDDIR)/asdk/gnu_utility/image_tool/imagetool.sh $(IMAGE_TARGET_FOLDER)/ap_image_all.bin

	# @if [ ${BUILD_TYPE} = "MFG" ]; then \
		# $(MOVE) $(IMAGE_TARGET_FOLDER)/ap_image_all.bin $(IMAGE_TARGET_FOLDER)/ap_image_all_mp.bin; \
	# fi

	# @if [ $(COMPILEOS) = $(LINUX_OS) ]; then\
		# chmod 777 $(OTAPREPENDTOOL) $(UTILITYDIR)/checksum; \
	# fi

	# @if [ -f $(IMAGE_TARGET_FOLDER)/ap_image_all.bin ] && [ -f $(KM4_BUILDDIR)/asdk/image/km0_km4_app.bin ]; then \
		# $(OTAPREPENDTOOL) $(KM4_BUILDDIR)/asdk/image/km0_km4_app.bin $(IMAGE_TARGET_FOLDER)/ap_image_all.bin; \
	# fi

	# @rm -f $(IMAGE_TARGET_FOLDER)/*prepend.bin
	# @$(COPY) $(IMAGE_TARGET_FOLDER)/*rom* $(ROM_LIB_DIR)
	# @$(COPY) ./build/atf/bl1/{*.dump,*.elf,*.ld,*.map} $(ROM_LIB_DIR)

echo "Realtek Postbuild Actions"
export TOPDIR=$BUILDDIR/

cp $BINDIR/tinyara.axf $BINDIR/target_img2.axf

arm-none-eabi-nm $BINDIR/target_img2.axf | sort > $BINDIR/target_img2.map

arm-none-eabi-objdump -d $BINDIR/target_img2.axf > $BINDIR/target_img2.asm

#arm-none-eabi-objdump -h -S --disassemble-all $BINDIR/target_img2.axf > $BINDIR/target_img2.txt

cp $BINDIR/target_img2.axf $BINDIR/target_pure_img2.axf

arm-none-eabi-strip $BINDIR/target_pure_img2.axf

#arm-none-eabi-nm $BINDIR/target_pure_img2.axf | sort > $BINDIR/target_pure_img2.map

#arm-none-eabi-objcopy -Obinary $BINDIR/target_pure_img2.axf $BINDIR/target_img2.bin

arm-none-eabi-objcopy -j .text -j .ARM.extab -j .ARM.exidx -j .ctors -j .dtors -j .preinit_array -j .init_array -j .fini_array -j .data -j .mmu_tbl \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/ram_2.bin

#arm-none-eabi-objcopy -j .xip_image2.text \
#-Obinary $BINDIR/target_pure_img2.axf $BINDIR/xip_image2.bin

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
#$GNUUTL/prepend_header.sh $BINDIR/ram_2.bin  __ram_image2_text_start__  $BINDIR/target_img2.map
#$GNUUTL/prepend_header.sh $BINDIR/xip_image2.bin  __flash_text_start__  $BINDIR/target_img2.map

cat $BINDIR/ram_2.bin > $BINDIR/ca32_image2_all.bin

python $GNUUTL/code_analyze.py

#cat $BINDIR/xip_image2_prepend.bin $BINDIR/ram_2_prepend.bin > $BINDIR/ca32_image2_all.bin
#$GNUUTL/pad.sh $BINDIR/km0_image2_all.bin

#$GNUUTL/imagetool_ap.sh $BINDIR/km0_image2_all.bin $(KM4_BUILDDIR)/asdk/image

echo "========== Image manipulating end =========="

cp $GNUUTL/bl1_sram.bin $BINDIR/bl1_sram.bin
cp $GNUUTL/bl1.bin $BINDIR/bl1.bin
cp $GNUUTL/bl2_bl32.bin $BINDIR/bl2_bl32.bin
cat $BINDIR/bl2_bl32.bin $BINDIR/ca32_image2_all.bin > $BINDIR/fip.bin

$GNUUTL/prepend_header.sh $BINDIR/bl1_sram.bin  __ca32_bl1_sram_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/bl1.bin  __ca32_bl1_dram_start__  $BINDIR/target_img2.map
$GNUUTL/prepend_header.sh $BINDIR/fip.bin  __ca32_fip_dram_start__  $BINDIR/target_img2.map
cat $BINDIR/bl1_sram_prepend.bin $BINDIR/bl1_prepend.bin $BINDIR/fip_prepend.bin > $BINDIR/ap_image_all.bin

$GNUUTL/imagetool_hp.sh $BINDIR/ap_image_all.bin

if [ -f $BINDIR/ap_image_all.bin ] && [ -f $BINDIR/km0_km4_app.bin ]; then \
	$GNUUTL/prepend_ota_header.sh $BINDIR/km0_km4_app.bin $BINDIR/ap_image_all.bin; \
fi

function copy_bootloader()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}

	echo "========== Copy_bootloader =========="
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
	# echo "========== Binary SIGNING =========="
	# bash $BUILDDIR/configs/rtl8730e/rtl8730e_signing.sh kernel
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
	cp $GNUUTL/km0_km4_app.bin $BINDIR/km0_km4_app.bin
}

copy_bootloader;
copy_flashloader;
copy_km0_km4_image;
 if [ "${CONFIG_BINARY_SIGNING}" == "y" ];then
	 concatenate_binary_with_signing;
 else
	 concatenate_binary_without_signing;
 fi
