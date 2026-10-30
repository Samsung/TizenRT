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
# build/configs/qemu-armv7a/qemu_run.sh
#
# Run build/output/bin/tinyara on the QEMU virt machine (Cortex-A7,
# GICv2, PL011 console).  The number of CPUs is CONFIG_SMP_NCPUS of
# os/.config (1 without SMP).  Extra arguments are passed to QEMU.
# Quit with Ctrl-a x.

TOPDIR=$(cd "$(dirname "$0")/../../.." && pwd)

NCPUS=$(sed -n 's/^CONFIG_SMP_NCPUS=//p' "${TOPDIR}/os/.config" 2>/dev/null)

exec qemu-system-arm -cpu cortex-a7 -smp "${NCPUS:-1}" -nographic \
	-machine virt,virtualization=off,gic-version=2 -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline \
	-kernel "${TOPDIR}/build/output/bin/tinyara" "$@"
