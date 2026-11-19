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
# build/configs/rv-virt/qemu_run.sh
#
# Run build/output/bin/tinyara on the QEMU RISC-V virt machine (RV32,
# M-mode, ACLINT timer, PLIC, 16550 console).  Extra arguments are passed
# to QEMU.  Quit with Ctrl-a x.

TOPDIR=$(cd "$(dirname "$0")/../../.." && pwd)

exec qemu-system-riscv32 -M virt,aclint=on -cpu rv32 -smp 1 -bios none \
	-nographic -kernel "${TOPDIR}/build/output/bin/tinyara" "$@"
