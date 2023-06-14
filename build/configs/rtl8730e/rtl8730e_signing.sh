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

function kernel_binary_signing()
{
	# Call script for Kernel binary signing
	echo "Not Support Kernel binary signing"
}

function user_binary_signing()
{
	# Call script for user binary signing
	echo "Not Support user binary signing"
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
