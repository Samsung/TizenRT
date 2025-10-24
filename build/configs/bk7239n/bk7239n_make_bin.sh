#!/usr/bin/env bash
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
TOOL_PATH=${THIS_PATH}/../../tools/armino
BOOT_PATH=${TOOL_PATH}/bootloader

ARMINO_OBJCOPY=arm-none-eabi-objcopy
ARMINO_READELF=arm-none-eabi-readelf
ARMINO_NM=arm-none-eabi-nm

echo "Beken Postbuild Actions"
export TOPDIR=$BUILDDIR/

PACKET_WRAPPER=${TOOL_PATH}/packet_wrapper.py

if [ ! -f ${CONFIG} ];then
	echo "No .config file"
	exit 1
fi

source ${CONFIG}

function image_encryption()
{
	echo "image_encryption"
}

function package_prepost()
{
	cp $BOOT_PATH/bootloader.bin $BINDIR/bootloader.bin
	cp $TOOL_PATH/configuration.json $BINDIR/configuration.json
	BOOTLOADER_STR=$BINDIR/bootloader.bin
	sed -i 's/BOOTLOADER_IMAGE/'${TERST_BINDIR}'\/bootloader.bin/g' $BINDIR/configuration.json
	echo ">>>>>>>>: " ${BOOTLOADER_STR}
	sed -i 's/APP_IMAGE/'${TERST_BINDIR}'\/app.bin/g' $BINDIR/configuration.json
}

function concatenate_binary()
{
	echo "concatenate_binary"

	#${ARMINO_OBJCOPY} -O binary ${BINDIR}/tinyara.axf ${BINDIR}/app.bin
	${ARMINO_OBJCOPY} -O binary $TOOL_PATH/app_test.elf ${BINDIR}/app.bin
	${ARMINO_READELF} -a -h -l -S -g -s ${BINDIR}/tinyara.axf > ${BINDIR}/app.txt
	${ARMINO_NM} -n -l -C -a -A -g ${BINDIR}/tinyara.axf > ${BINDIR}/app.nm
	arm-none-eabi-objdump -d ${BINDIR}/tinyara.axf > $BINDIR/target_img2.asm

	${TOOL_PATH}/image_tool/cmake_Gen_image genfile -injsonfile  ${TOOL_PATH}/configuration.json 	\
		-infile $BINDIR/bootloader.bin $BINDIR/app.bin												\
		-outfile $BINDIR/all-app_original.bin

	${TOOL_PATH}/image_tool/cmake_encrypt_crc -crc $BINDIR/all-app_original.bin

	rm $BINDIR/all-app_original.bin
	cp $BINDIR/all-app_original_crc.bin $BINDIR/all-app_original.bin
	mv $BINDIR/all-app_original_crc.bin $BINDIR/all-app.bin
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
	bash $BUILDDIR/configs/bk7239n/bk7239n_signing.sh kernel
}

function make_ss_bin() {
	script=${TOOL_PATH}/security_storage/security_storage.py 
	ss_config=${TOOL_PATH}/security_storage/security_storage.csv
	ss_bin=${BUILDDIR}/output/bin/ss.bin
	python3 -B ${script} ${ss_config} ${ss_bin}
}

function pack_secure_bin()
{
	export PYTHONPATH=${BUILDDIR}/tools/armino/bk_py_libs:$(PYTHONPATH)
	workdir=${BUILDDIR}/output/bin/tfm_build/_build
	cp ${BUILDDIR}/output/bin/tinyara.axf.bin $workdir/cpu0_app.bin
	script=${BUILDDIR}/tools/armino/beken_utils/main.py
	python3 -B $script pack all --config_dir $workdir
	cp $workdir/bootloader_ota.bin ${BINDIR}/bootloader_ota.bin
	cp $workdir/bootloader.bin ${BINDIR}/bootloader.bin
	cp $workdir/primary_all_code_signed.bin ${BINDIR}/kernel.bin
	cp $workdir/partition.bin ${BINDIR}/partition.bin
	TMP_FILE=${BINDIR}/kernel.bin.tmp
    dd if=${BINDIR}/kernel.bin of=$TMP_FILE bs=1 skip=16
    mv $TMP_FILE ${BINDIR}/kernel.bin
	make_ss_bin
}

function pack_non_secure_bin()
{
	TMP_FILE=${BUILDDIR}/output/bin/tmp.bin
	touch $TMP_FILE
	printf "\xFF%.0s" {1..16} > "$TMP_FILE"
	FILE=${BUILDDIR}/output/bin/tinyara.axf.bin
	cat "$FILE" >> "$TMP_FILE"
	mv "$TMP_FILE" "$FILE"
	cp $FILE ${BINDIR}/kernel.bin
	cp ${BUILDDIR}/tools/armino/bootloader/bootloader_${CONFIG_SOC_STR}.bin ${BINDIR}/bootloader.bin
}

arm-none-eabi-objdump -d ${BINDIR}/tinyara.axf > $BINDIR/tinyara.asm

#image_encryption;
#package_prepost;
#concatenate_binary;

# python2 ${PACKET_WRAPPER} --tools-dir ${TOOL_PATH} --build-dir ${BUILDDIR}

if [ "$CONFIG_TFM" = "y" ]; then
    pack_secure_bin
else
    pack_non_secure_bin
fi

echo "############# ARMINO BK7239N #############"

