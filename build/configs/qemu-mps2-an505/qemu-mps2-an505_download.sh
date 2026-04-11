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

if [ -z "$TOPDIR" ]; then
	TOPDIR=$(cd "$(dirname "$0")"/../../.. && pwd)
fi

source "${TOPDIR}/build/configs/qemu-mps2-an505/board_metadata.txt"
source "${TOPDIR}/os/.bininfo"
source "${TOPDIR}/os/.config"

BOOTPARAM_NAME="${BOOTPARAM:-bootparam}"

BIN_DIR="${TOPDIR}/build/output/bin"
IMAGE_DIR="${TOPDIR}/qemu_mps2_an505_images"
RUN_SCRIPT="${TOPDIR}/run_qemu.sh"
MANIFEST="${IMAGE_DIR}/manifest.txt"

copy_if_present() {
	local source_name=$1
	local target_name=$2

	if [ -n "${source_name}" ] && [ -f "${BIN_DIR}/${source_name}" ]; then
		cp "${BIN_DIR}/${source_name}" "${IMAGE_DIR}/${target_name}"
		echo "${target_name}=${source_name}" >> "${MANIFEST}"
	fi
}

rm -rf "${IMAGE_DIR}"
mkdir -p "${IMAGE_DIR}"

: > "${MANIFEST}"
echo "board=${BOARD}" >> "${MANIFEST}"
echo "kernel=${KERNEL_BIN_NAME}" >> "${MANIFEST}"

copy_if_present "${KERNEL_BIN_NAME}" "kernel-runtime.bin"
copy_if_present "kernel_without_header.bin" "kernel-runtime-raw.bin"
copy_if_present "kernel" "kernel-runtime.elf"
copy_if_present "${COMMON_BIN_NAME}" "common.trpk"
copy_if_present "${APP1_BIN_NAME}" "app1.trpk"
copy_if_present "${BOOTPARAM_NAME}.bin" "bootparam.bin"

TOPDIR_ENV="${TOPDIR}" IMAGE_DIR_ENV="${IMAGE_DIR}" BIN_DIR_ENV="${BIN_DIR}" \
KERNEL_BIN_ENV="${KERNEL_BIN_NAME}" COMMON_BIN_ENV="${COMMON_BIN_NAME}" \
APP1_BIN_ENV="${APP1_BIN_NAME}" BOOTPARAM_ENV="${BOOTPARAM_NAME}.bin" \
python3 <<'PY'
import os
from pathlib import Path


def parse_config(path: Path):
    config = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        config[key] = value.strip().strip('"')
    return config


topdir = Path(os.environ["TOPDIR_ENV"])
image_dir = Path(os.environ["IMAGE_DIR_ENV"])
bin_dir = Path(os.environ["BIN_DIR_ENV"])
config = parse_config(topdir / "os" / ".config")

flash_size = int(config["CONFIG_FLASH_SIZE"], 0)
part_sizes = [item for item in config["CONFIG_FLASH_PART_SIZE"].split(",") if item]
part_names = [item for item in config["CONFIG_FLASH_PART_NAME"].split(",") if item]

image = bytearray(b"\xff" * flash_size)
offset = 0

sources = {
    "kernel": bin_dir / os.environ["KERNEL_BIN_ENV"],
    "common": bin_dir / os.environ["COMMON_BIN_ENV"],
    "app1": bin_dir / os.environ["APP1_BIN_ENV"],
    "bootparam": bin_dir / os.environ["BOOTPARAM_ENV"],
}

for name, size_kb in zip(part_names, part_sizes):
    part_size = int(size_kb, 0) * 1024
    source = sources.get(name)
    if source and source.exists():
        data = source.read_bytes()
        if len(data) > part_size:
            raise SystemExit(f"{source} exceeds partition '{name}' size {part_size}")
        image[offset:offset + len(data)] = data
    offset += part_size

(image_dir / "flash-runtime.bin").write_bytes(image)
PY

if [ -f "${IMAGE_DIR}/flash-runtime.bin" ]; then
	echo "flash-runtime.bin=generated" >> "${MANIFEST}"
fi

cp "${TOPDIR}/build/configs/qemu-mps2-an505/tools/run_qemu.sh" "${RUN_SCRIPT}"
chmod +x "${RUN_SCRIPT}"

echo "qemu-mps2-an505 images staged at ${IMAGE_DIR}"
