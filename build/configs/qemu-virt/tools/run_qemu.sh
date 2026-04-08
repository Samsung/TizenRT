#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
if [ -f "${SCRIPT_DIR}/os/.config" ]; then
    TOPDIR="${SCRIPT_DIR}"
else
    TOPDIR=$(cd "${SCRIPT_DIR}/../../.." && pwd)
fi

CONFIG_FILE="${TOPDIR}/os/.config"
QEMU_FLASH="${TOPDIR}/qemu_flash.bin"
QEMU_FLASH2="${TOPDIR}/qemu_flash2.bin"
DISK_IMAGE="${TOPDIR}/disk.raw"
DEFAULT_IMAGE="tizenrt-qemu-virt-runtime:local"
QEMU_IMAGE="${QEMU_VIRT_DOCKER_IMAGE:-${DEFAULT_IMAGE}}"

get_config_value() {
    if [ ! -f "${CONFIG_FILE}" ]; then
        return 0
    fi

    grep "^$1=" "${CONFIG_FILE}" | cut -d'=' -f2- | sed 's/^"//; s/"$//'
}

build_runtime_image_if_needed() {
    if docker image inspect "${QEMU_IMAGE}" >/dev/null 2>&1; then
        return 0
    fi

    if [ -n "${QEMU_VIRT_DOCKER_IMAGE:-}" ]; then
        echo "Configured runtime image not found: ${QEMU_IMAGE}" >&2
        exit 1
    fi

    docker build -t "${QEMU_IMAGE}" -f "${TOPDIR}/tools/qemu_test/Dockerfile" "${TOPDIR}"
}

SECOND_FLASH_OPTION=()
if [ "$(get_config_value "CONFIG_SECOND_FLASH_PARTITION")" = "y" ] && [ -f "${QEMU_FLASH2}" ]; then
    SECOND_FLASH_OPTION=(-drive "if=pflash,format=raw,file=/workspace/qemu_flash2.bin")
fi

SMP_OPTION=()
if [ "$(get_config_value "CONFIG_SMP")" = "y" ]; then
    SMP_NCPUS=$(get_config_value "CONFIG_SMP_NCPUS")
    SMP_OPTION=(-smp "${SMP_NCPUS:-2}")
fi

VIRTIO_BLK_OPTION=()
if [ "$(get_config_value "CONFIG_QEMU_VIRT_VIRTIO_BLK")" = "y" ]; then
    if [ ! -f "${DISK_IMAGE}" ]; then
        truncate -s 64M "${DISK_IMAGE}"
    fi
    VIRTIO_BLK_OPTION=(
        -drive "file=/workspace/disk.raw,format=raw,if=none,id=blk0"
        -device "virtio-blk-device,drive=blk0,bus=virtio-mmio-bus.0"
    )
fi

if [ ! -f "${QEMU_FLASH}" ]; then
    echo "Flash image not found: ${QEMU_FLASH}" >&2
    echo "Run ./os/dbuild.sh download ... first." >&2
    exit 1
fi

build_runtime_image_if_needed

exec docker run --rm -it \
    -v "${TOPDIR}:/workspace" \
    -w /workspace \
    -p 1234:1234 \
    "${QEMU_IMAGE}" \
    qemu-system-arm \
    -machine virt,virtualization=off,gic-version=2 \
    -cpu cortex-a15 \
    -m 64M \
    -nographic \
    -monitor none \
    -net none \
    -drive if=pflash,format=raw,file=/workspace/qemu_flash.bin \
    "${SECOND_FLASH_OPTION[@]}" \
    "${VIRTIO_BLK_OPTION[@]}" \
    "${SMP_OPTION[@]}" \
    "$@"
