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

TOPDIR=$(realpath ..)
BUILDDIR=${TOPDIR}/build
BINDIR=${BUILDDIR}/output/bin

function kernel_binary_signing()
{
	# Call script for Kernel binary signing
	echo "Not Support Kernel binary signing"
}

function user_binary_signing()
{
	export PYTHONPATH=${BUILDDIR}/tools/armino/bk_py_libs:$(PYTHONPATH)
	sign_script=${BUILDDIR}/tools/armino/misc/app_sign.py
	tfm_build_dir=${BUILDDIR}/output/bin/tfm_build/_build
	if [ -f ${BINDIR}/app1 ]; then
		echo "Sign app1"
		python3 ${sign_script} ${BINDIR}/app1 ${tfm_build_dir}/root_ec256_privkey.pem ${tfm_build_dir}/root_ec256_pubkey.pem
	fi
	if [ -f ${BINDIR}/app2 ]; then
		echo "Sign app2"
		python3 ${sign_script} ${BINDIR}/app2 ${tfm_build_dir}/root_ec256_privkey.pem ${tfm_build_dir}/root_ec256_pubkey.pem
	fi
}

############################################################################
#
# Signing for binary types
#
############################################################################
if [ "$1" == "kernel" ];then
	kernel_binary_signing;
elif [ "$1" == "user" ];then
	user_binary_signing;
else
	echo "Error : Not supported Binary Type"
	exit 1
fi
