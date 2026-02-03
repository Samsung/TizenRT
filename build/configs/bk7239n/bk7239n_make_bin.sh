#!/usr/bin/env bash
###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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

set -e
set -o pipefail

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
	# Copy default ss.bin from security_storage directory to output
	ss_storage_dir=${TOOL_PATH}/security_storage
	default_ss_bin=${ss_storage_dir}/ss.bin
	ss_bin=${BUILDDIR}/output/bin/ss.bin
	
	if [ -f "${default_ss_bin}" ]; then
		echo "Copying default ss.bin from ${default_ss_bin} to ${ss_bin}"
		cp -f "${default_ss_bin}" "${ss_bin}"
	else
		echo "Warning: Default ss.bin not found at ${default_ss_bin}, skipping..."
	fi
}

function get_global_app_num_from_config_for_deployment() {
    # Get CONFIG_NUM_APPS value from os/.config
    [ ! -f "$CONFIG" ] && { echo "0"; return 1; }

    local app_nums=$(grep "^CONFIG_NUM_APPS=" "$CONFIG" 2>/dev/null | cut -d'=' -f2 | tr -d '"' | tr -d ' ')

    # Validate and default to 0 if invalid
    [[ "$app_nums" =~ ^[0-9]+$ ]] && echo "$app_nums" || echo "0"
}

# Install binary files for deployment
function install_bin_for_deployment()
{
	local workdir=${BUILDDIR}/output/bin/tfm_build/_build
	local install_dir="${BUILDDIR}/output/bin/install_for_deployment"

	echo "========================================"
	echo "Installing binaries for deployment"
	echo "Working directory: $workdir"
	echo "Install directory: $install_dir"
	echo "========================================"

	# Create installation directory
	mkdir -p "$install_dir"
	chmod a+wx "$install_dir"

	if [ -d "$install_dir" ]; then
		rm -rf "$install_dir"/*
	fi

	# Copy core binary files
	[ -f "$BINDIR/tinyara.axf.bin" ] && cp -f "$BINDIR/tinyara.axf.bin" "$install_dir/cpu0_app.bin" || echo "Warning: cpu0_app.bin not found, skipping..."
	[ -f "$workdir/bl2.bin" ] && cp -f "$workdir/bl2.bin" "$install_dir/" || echo "Warning: bl2.bin not found, skipping..."
	if [ "$CONFIG_BL2_UPDATE_WITH_PC" = "y" ]; then
		[ -f "$workdir/bl2_B.bin" ] && cp -f "$workdir/bl2_B.bin" "$install_dir/" || echo "Warning: bl2_B.bin not found, skipping..."
	fi
	[ -f "$workdir/tfm_s.bin" ] && cp -f "$workdir/tfm_s.bin" "$install_dir/" || echo "Warning: tfm_s.bin not found, skipping..."

	# Copy configuration files
	[ -f "$workdir/partitions.csv" ] && cp -f "$workdir/partitions.csv" "$install_dir/" || echo "Warning: partitions.csv not found, skipping..."
	[ -f "$workdir/ota.csv" ] && cp -f "$workdir/ota.csv" "$install_dir/" || echo "Warning: ota.csv not found, skipping..."
	[ -f "$workdir/security.csv" ] && cp -f "$workdir/security.csv" "$install_dir/" || echo "Warning: security.csv not found, skipping..."
	[ -f "$workdir/pack.json" ] && cp -f "$workdir/pack.json" "$install_dir/" || echo "Warning: pack.json not found, skipping..."
	[ -f "$workdir/ppc_config.bin" ] && cp -f "$workdir/ppc_config.bin" "$install_dir/" || echo "Warning: ppc_config.bin not found, skipping..."

	# Copy key files
	[ -f "$workdir/root_ec256_privkey.pem" ] && cp -f "$workdir/root_ec256_privkey.pem" "$install_dir/" || echo "Warning: root_ec256_privkey.pem not found, skipping..."
	[ -f "$workdir/root_ec256_pubkey.pem" ] && cp -f "$workdir/root_ec256_pubkey.pem" "$install_dir/" || echo "Warning: root_ec256_pubkey.pem not found, skipping..."

	# Copy optional app files
	# Copy signed apps
	local app_nums=$(get_global_app_num_from_config_for_deployment)
	[ "$app_nums" -gt 0 ] && echo "Copying $app_nums app(s)..." || echo "Info: No apps to copy"

	for app_num in $(seq 1 "$app_nums" 2>/dev/null); do
		local src_app="${BINDIR}/app${app_num}"
		[ -f "$src_app" ] && cp -f "$src_app" "$install_dir/app${app_num}_for_sign" && echo "  [${app_num}/${app_nums}] app${app_num}_for_sign" || echo "  Warning: app${app_num}_for_sign not found"
	done

	# Copy common binary if CONFIG_SUPPORT_COMMON_BINARY is enabled
	if grep -q "^CONFIG_SUPPORT_COMMON_BINARY=y" "$CONFIG" 2>/dev/null; then
		local src_common="${BINDIR}/common"
		[ -f "$src_common" ] && cp -f "$src_common" "$install_dir/common_for_sign" && echo "  common_for_sign" || echo "  Warning: common_for_sign not found"
	fi

	# Copy ss.bin if exists
	[ -f "$BINDIR/ss.bin" ] && cp -f "$BINDIR/ss.bin" "$install_dir/" && echo "  ss.bin" || echo "  Warning: ss.bin not found"

	#copy partition_layout.txt
	[ -f "$workdir/partition_layout.txt" ] && cp -f "$workdir/partition_layout.txt" "$install_dir/partition_layout.txt" || echo "Warning: partition_layout.txt not found, skipping..."

	echo "Deployment installation completed: $install_dir"
}

function copy_file()
{
	if [ -f "$1" ]; then
		cp -f "$1" "$2"
	fi
}

function copy_signed_files()
{
	local output_dir=${BUILDDIR}/output
	local deploy_output_dir="${BUILDDIR}/tools/armino/beken_utils/output_dir"

	# Copy signed files to install directory
	copy_file  "$deploy_output_dir/bootloader.bin" "$output_dir/"
	copy_file  "$deploy_output_dir/kernel.bin" "$output_dir/"
	copy_file  "$deploy_output_dir/app1" "$output_dir/"
	copy_file  "$deploy_output_dir/app2" "$output_dir/"
}

function pack_secure_bin()
{
    python3 ${THIS_PATH}/psram_layout_check.py ${CONFIG}
	make_ss_bin
	install_bin_for_deployment
	if [ "$CONFIG_BINARY_SIGNING" = "y" ]; then
		echo "Making signing binary"
		input_dir=${BUILDDIR}/tools/armino/beken_utils/input_dir
		cp -rf ${BUILDDIR}/output/bin/install_for_deployment/. $input_dir
		cp ${BINDIR}/tinyara.axf.bin ${BINDIR}/kernel.bin
	else
		echo "Making no signing binary"
		cp ${BINDIR}/tfm_build/_build/partition.bin ${BINDIR}/install_for_deployment/partition.bin
		python3 -B ${TOOL_PATH}/bkfil_pack_tool/make_no_signing_bin.py
	fi
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

