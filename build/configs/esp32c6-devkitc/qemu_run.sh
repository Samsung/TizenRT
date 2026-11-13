#!/usr/bin/env bash
###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
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
# build/configs/esp32c6-devkitc/qemu_run.sh
#
# Run build/output/bin/tinyara.merged.bin (the image of the whole flash)
# on the 'esp32c6' machine of the Espressif QEMU, which the
# tizenrt/tizenrt:2.0.1-qemu-esp32c6 image (Dockerfile in this directory)
# has in /opt/qemu-esp.  Set QEMU_ESP to use another binary.  The ROM of
# the machine loads the image from the SPI flash like the chip does.
# snapshot=on keeps the image file unchanged.  Extra arguments are passed
# to QEMU.  Quit with Ctrl-a x.

TOPDIR=$(cd "$(dirname "$0")/../../.." && pwd)
QEMU_ESP=${QEMU_ESP:-/opt/qemu-esp/bin/qemu-system-riscv32}

exec "${QEMU_ESP}" -nographic -machine esp32c6 \
	-drive "file=${TOPDIR}/build/output/bin/tinyara.merged.bin,if=mtd,format=raw,snapshot=on" \
	"$@"
