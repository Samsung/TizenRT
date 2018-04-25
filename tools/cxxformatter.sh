#!/bin/bash
###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

CMD=clang-format
OPTION="-i -style=file"

if ! which ${CMD} >/dev/null; then
	echo "Need to install clang-format"
	exit 1
fi

function USAGE()
{
	echo "usage: $0 TARGET"
	echo "     TARGET: folder path or target files"
	echo "Apply TizenRT C++ coding rule"
	echo ""
	echo "example:"
	echo "    $0 kernel/"
	echo "    $0 framework/src/media apps/system"
	echo "    $0 ../../abc.c"
}

if [ -z $1 ]; then
	USAGE
	exit 1
fi
if [ $1 == "--help" ]; then
	USAGE
	exit 1
fi

for target in "$@"; do
	if [ -d "${target}" ]; then
		find "${target}" -name *.cpp -o -name *.cxx -o -name *.cc -o -name *.h -exec ${CMD} ${OPTION} {} \;
	else
		${CMD} ${OPTION} ${target}
	fi
done

