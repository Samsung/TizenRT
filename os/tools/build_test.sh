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
# build_test.sh

TOOLDIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
OSDIR="${TOOLDIR}/.."
OUTPUT_DIR="${TOOLDIR}/build_test/"

BUILD_CMD=make
CLEAN_CMD="${BUILD_CMD} distclean"

build_targets=(
	"artik055s/audio"
	"artik053/st_things"
	"artik053/tc"
	"qemu/build_test"
	"esp_wrover_kit/hello_with_tash"
	"imxrt1020-evk/loadable_elf_apps"
	"rtl8721csm/hello"
	"rtl8721csm/loadable_apps"
)

is_failed=false

if [ -d ${OUTPUT_DIR} ]; then
	rm -rf ${OUTPUT_DIR}
fi
mkdir ${OUTPUT_DIR}

echo "===================================="
echo "========= Build Test START ========="
echo "===================================="

for index in ${!build_targets[@]}; do
	# verify build clean
	echo "===================================="
	echo "======= Clean Previous build ======="
	echo "===================================="
	cd ${OSDIR}; ${CLEAN_CMD}

	# configure
	echo "===================================="
	echo "====== Configure \"${build_targets[$index]}\" ======"
	echo "===================================="	
	cd ${TOOLDIR}; ./configure.sh ${build_targets[$index]}

	# build
	echo "===================================="
	echo "======== Build \"${build_targets[$index]}\" ========"
	echo "===================================="	
	cd ${OSDIR};
	OUTPUT_PATH=${OUTPUT_DIR}"`echo ${build_targets[$index]} | sed -e "s/\//\./g"`"
	set -o pipefail;
	${BUILD_CMD} |& tee ${OUTPUT_PATH}
	if [ $? -eq 0 ]; then
		echo "===================================="
		echo "======== \"${build_targets[$index]}\" Built ========"
		echo "===================================="
		build_results[${index}]="SUCCESS"
	else
		is_failed=true
		build_results[${index}]="FAIL"
		echo "===================================="
		echo "======== \"${build_targets[$index]}\" ERROR!!! ========"
		echo "===================================="
	fi
done

echo "===================================="
echo "======== Clean build output ========"
echo "===================================="
cd ${OSDIR}; ${CLEAN_CMD}

echo "=========================================="
echo "=========== Build Test Result ==========="
echo "=========================================="
for index in ${!build_targets[@]}; do
	printf "%30s ------ %s\n" ${build_targets[$index]} ${build_results[$index]}
done
echo "=========================================="

if ${is_failed}; then
	echo "Some builds are FAILED!! Please find build results in ${OUTPUT_DIR}"
	exit 1
else
	rm -rf ${OUTPUT_DIR}
fi
