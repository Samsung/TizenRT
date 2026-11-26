#!/usr/bin/env bash
set -euo pipefail
# Build QEMU v2.12.0-rc1 with the memory layout used by qemu/build_test.
patch_file=$(realpath "$1")
mkdir -p "$2"
cd "$2"
git init -q
git fetch --depth=1 https://github.com/qemu/qemu.git fa3704d87720d7049d483ff669b9e2ff991e7658
git checkout -q --detach FETCH_HEAD
# The historical .gitmodules URLs use retired git:// endpoints.
git config submodule.ui/keycodemapdb.url https://github.com/qemu/keycodemapdb.git
git config submodule.dtc.url https://github.com/qemu/dtc.git
git submodule update --init --depth=1 ui/keycodemapdb dtc
git apply "$patch_file"
./configure --target-list=arm-softmmu --python=/usr/bin/python3 \
    --disable-werror --disable-docs --disable-gtk --disable-sdl --disable-vnc \
    --disable-capstone --extra-cxxflags="-include type_traits"
make -j"$(nproc)" subdir-arm-softmmu
test -x arm-softmmu/qemu-system-arm
