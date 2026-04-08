#!/usr/bin/env bash
#============================================================================
#  Copyright 2025 Samsung Electronics All Rights Reserved.
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

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
CONFIG=${TOP_PATH}/os/.config
BUILDDIR=${TOP_PATH}/build
BINDIR=${BUILDDIR}/output/bin

S1_BOOT_MAKE_DIR=${TOP_PATH}/os/board/qemu-virt/src/bootloader
S1_BOOT_BIN=${BINDIR}/s1_boot.bin

# Build the S1 bootloader
echo "========== Build S1 bootloader =========="
make -C ${S1_BOOT_MAKE_DIR} clean TOPDIR=${TOP_PATH}
make -C ${S1_BOOT_MAKE_DIR} TOPDIR=${TOP_PATH}

echo "Bootloader image created at ${S1_BOOT_BIN}"

echo "========== Post Build Jobs =========="

cp $BINDIR/tinyara $BINDIR/target_img2.axf

arm-none-eabi-nm $BINDIR/target_img2.axf | sort > $BINDIR/target_img2.map

arm-none-eabi-objdump -d $BINDIR/target_img2.axf > $BINDIR/target_img2.asm
if [ -f "$BINDIR/common_dbg" ]; then
arm-none-eabi-objdump -d $BINDIR/common_dbg > $BINDIR/common_dbg.asm
fi
if [ -f "$BINDIR/app1_dbg" ]; then
arm-none-eabi-objdump -d $BINDIR/app1_dbg > $BINDIR/app1_dbg.asm
fi

cp $BINDIR/target_img2.axf $BINDIR/target_pure_img2.axf

arm-none-eabi-strip $BINDIR/target_pure_img2.axf

$TOP_PATH/build/configs/qemu-virt/tools/prepend_header.sh $BINDIR/tinyara.bin  __flash_text_start__  $BINDIR/target_img2.map

arm-none-eabi-objcopy -j .code -j .data \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/ram_2.bin

arm-none-eabi-objcopy -j .xip_image2.text -j .ARM.extab -j .ARM.exidx -j .ctors -j .dtors -j .preinit_array -j .init_array -j .fini_array \
-Obinary $BINDIR/target_pure_img2.axf $BINDIR/xip_image2.bin

echo "========== Image Info HEX =========="
arm-none-eabi-size -A --radix=16 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=16 $BINDIR/target_img2.axf
echo "========== Image Info HEX =========="

echo "========== Image Info DEC =========="
arm-none-eabi-size -A --radix=10 $BINDIR/target_img2.axf
arm-none-eabi-size -t --radix=10 $BINDIR/target_img2.axf
echo "========== Image Info DEC =========="

echo "========== linker img2 end =========="
