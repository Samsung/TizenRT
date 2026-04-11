#!/usr/bin/env bash
#============================================================================
#  Copyright 2026 Samsung Electronics All Rights Reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#============================================================================

set -e

THIS_PATH=$(test -d "${0%/*}" && cd "${0%/*}" && pwd)
TOP_PATH=${THIS_PATH}/../../..
BINDIR=${TOP_PATH}/build/output/bin

if [ ! -f "${BINDIR}/tinyara.bin" ]; then
	echo "ERROR: ${BINDIR}/tinyara.bin not found"
	exit 1
fi

cp "${BINDIR}/tinyara.bin" "${BINDIR}/kernel.bin"

if [ -f "${BINDIR}/tinyara" ]; then
	cp "${BINDIR}/tinyara" "${BINDIR}/kernel"
fi
