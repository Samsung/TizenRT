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
# build/configs/qemu-armv7r/qemu_run.sh
#
# Run build/output/bin/tinyara on the QEMU virt machine with a Cortex-R5F
# CPU, a GICv2 and a PL011 UART console.  Upstream QEMU has no Cortex-R on
# virt, so this uses the OpenVela QEMU of the image built from
# build/configs/qemu-armv7r/Dockerfile (/opt/qemu-vela, or QEMU_VELA).
# -semihosting lets the ostest configuration power QEMU off.  Extra
# arguments are passed to QEMU.  Quit with Ctrl-a x.

TOPDIR=$(cd "$(dirname "$0")/../../.." && pwd)
QEMU_VELA=${QEMU_VELA:-/opt/qemu-vela}

exec "${QEMU_VELA}/bin/qemu-system-arm" -M virt -cpu cortex-r5f -semihosting \
	-nographic -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline \
	-kernel "${TOPDIR}/build/output/bin/tinyara" "$@"
