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

BUILD_CMD=make
CLEAN_CMD="${BUILD_CMD} distclean"

build_targets="artik055s/audio
               artik053/st_things
               artik053/tc
               qemu/build_test
               esp_wrover_kit/hello_with_tash
               imxrt1020-evk/loadable_elf_apps
               rtl8721csm/loadable_apps"

for target in ${build_targets[@]}; do
	# verify build clean
	echo "===================================="
	echo "======= Clean Previous build ======="
	echo "===================================="
	cd ${OSDIR}; ${CLEAN_CMD}

	# configure
	echo "===================================="
	echo "====== Configure \"${target}\" ======"
	echo "===================================="	
	cd ${TOOLDIR}; ./configure.sh ${target}

	# build
	echo "===================================="
	echo "======== Build \"${target}\" ========"
	echo "===================================="	
	cd ${OSDIR}; ${BUILD_CMD} || { echo "\"${target}\" ERROR!!!"; exit 1; }

	echo "===================================="
	echo "======== \"${target}\" Built ========"
	echo "===================================="	
done
