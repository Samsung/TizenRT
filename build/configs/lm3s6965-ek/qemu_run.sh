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
# build/configs/lm3s6965-ek/qemu_run.sh
#
# Run build/output/bin/tinyara on the QEMU lm3s6965evb machine (TI Stellaris
# LM3S6965, Cortex-M3, 256 KB flash, 64 KB SRAM, UART0 console).  Extra
# arguments are passed to QEMU.  Quit with Ctrl-a x.

TOPDIR=$(cd "$(dirname "$0")/../../.." && pwd)

exec qemu-system-arm -M lm3s6965evb -nographic \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline \
	-kernel "${TOPDIR}/build/output/bin/tinyara" "$@"
