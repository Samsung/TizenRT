#
# TizenRT image with the OpenVela QEMU for the NuttX-based qemu-armv7r
# board (Cortex-R5 on the QEMU 'virt' machine).
#
# Upstream QEMU accepts only A-profile CPUs on 'virt'
# ('-M virt -cpu cortex-r5f': "CPU type cortex-r5f-arm-cpu not
# supported").  The NuttX 12.13.0 qemu-armv7r board uses the OpenVela
# QEMU fork, which adds Cortex-R support to 'virt'.  The fork (QEMU
# 10.0.0 based) is built statically from a fixed commit and installed
# in /opt/qemu-vela, beside the upstream QEMU 8.2 of
# tizenrt/tizenrt:2.0.1-qemu (tools/docker), which stays the default
# qemu-system-arm.
#
# Build (after tools/docker):
#   docker build -t tizenrt/tizenrt:2.0.1-qemu-armv7r build/configs/qemu-armv7r
#
# The build image of qemu-armv7r is tizenrt/tizenrt:2.0.1-qemu
# (CONFIG_DOCKER_VERSION="2.0.1-qemu"); this image is only needed to run
# build/configs/qemu-armv7r/qemu_run.sh.
#

FROM ubuntu:24.04 AS qemu-build

ARG DEBIAN_FRONTEND=noninteractive
ARG QEMU_VELA_URL=https://github.com/open-vela/platform_external_qemu.git
ARG QEMU_VELA_REV=df9bdfe302b83e0d980b4ac8e84bde6fd855685e

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        bison build-essential ca-certificates flex git libfdt-dev \
        libglib2.0-dev libpcre2-dev libpixman-1-dev ninja-build \
        pkg-config python3 python3-venv zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /build/qemu
RUN git init -q . && \
    git remote add origin ${QEMU_VELA_URL} && \
    git fetch -q --depth 1 origin ${QEMU_VELA_REV} && \
    git checkout -q FETCH_HEAD

# arm-softmmu only, static, without the documentation and the firmware
# blobs (not needed by '-M virt -kernel').

WORKDIR /build/qemu/build
RUN ../configure --prefix=/opt/qemu-vela --target-list=arm-softmmu --static \
        --without-default-features --enable-fdt=system --disable-docs \
        --disable-install-blobs --disable-werror && \
    make -j"$(nproc)" && make install && \
    /opt/qemu-vela/bin/qemu-system-arm --version && \
    /opt/qemu-vela/bin/qemu-system-arm -M virt -cpu help | grep -q cortex-r5f

FROM tizenrt/tizenrt:2.0.1-qemu

COPY --from=qemu-build /opt/qemu-vela /opt/qemu-vela
