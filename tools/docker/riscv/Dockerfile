#
# TizenRT build image with the RISC-V toolchain and QEMU for the
# NuttX-based rv-virt (qemu-rv) and esp32c6 boards.
#
# It extends the public tizenrt/tizenrt:2.0.1 image (Ubuntu 16.04,
# glibc 2.23, kconfig-frontends) with:
#
# - qemu-system-riscv32 8.2, built statically from the Ubuntu 24.04
#   source package like tools/docker/Dockerfile does for qemu-system-arm
#   (the base image has no QEMU for RISC-V).
# - The Espressif riscv32-esp-elf GCC 14.2.0.  The xPack
#   riscv-none-elf-gcc 13.2.0 that the NuttX documentation recommends
#   needs glibc 2.27 and does not run on the base image; the Espressif
#   toolchain needs glibc 2.15 only and has the rv32imac multilib that
#   both boards use.
# - esptool, which the esp32c6 build needs on every build to make the
#   flash image (elf2image, merge_bin).  The Python 3.8 of the base image
#   has no ssl module, so the pure Python wheels are fetched in the first
#   stage and installed offline.
#
# Build:
#   docker build -t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
#
# Behind a proxy that re-signs TLS, pass the proxy and the CA bundle of
# the host (the downloads use it when the secret is given):
#   docker build --build-arg http_proxy --build-arg https_proxy \
#       --secret id=ca_bundle,src=/etc/ssl/certs/ca-certificates.crt \
#       -t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
#
# os/dbuild.sh uses the local image when
# CONFIG_DOCKER_VERSION="2.0.1-qemu-riscv".
#

FROM ubuntu:24.04 AS qemu-build

ARG DEBIAN_FRONTEND=noninteractive

RUN sed -i 's/^Types: deb$/Types: deb deb-src/' /etc/apt/sources.list.d/ubuntu.sources && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        bison build-essential ca-certificates dpkg-dev flex libfdt-dev \
        libglib2.0-dev libpcre2-dev libpixman-1-dev meson ninja-build \
        pkg-config python3 python3-venv zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /build
RUN apt-get update && apt-get source --download-only qemu && \
    rm -rf /var/lib/apt/lists/* && \
    tar xf qemu_*.orig.tar.* && mv qemu-*/ qemu

# The +ds source has no firmware blobs: do not unpack the edk2 images.
# The boards run in M-mode with '-bios none', so OpenSBI is not needed.
WORKDIR /build/qemu/build
RUN sed -i 's/unpack_edk2_blobs = bzip2.found()/unpack_edk2_blobs = false/' \
        ../meson.build && \
    ../configure --prefix=/opt/qemu --target-list=riscv32-softmmu --static \
        --without-default-features --enable-fdt=system --disable-docs \
        --disable-install-blobs --disable-werror && \
    make -j"$(nproc)" && make install && \
    /opt/qemu/bin/qemu-system-riscv32 --version

FROM ubuntu:24.04 AS tools

ARG DEBIAN_FRONTEND=noninteractive
ARG ESP_GCC_VERSION=14.2.0_20241119
ARG ESP_GCC_SHA256=7faaa86d272f3e43c233f8a5ffeba327673224a752c2eb72394655d5e7950000
ARG ESPTOOL_VERSION=4.8.1

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates curl python3 python3-pip python3-setuptools \
        python3-wheel xz-utils && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt
RUN --mount=type=secret,id=ca_bundle,required=false \
    if [ -s /run/secrets/ca_bundle ]; then export CURL_CA_BUNDLE=/run/secrets/ca_bundle; fi && \
    curl -fsSL -o esp-gcc.tar.xz \
        https://github.com/espressif/crosstool-NG/releases/download/esp-${ESP_GCC_VERSION}/riscv32-esp-elf-${ESP_GCC_VERSION}-x86_64-linux-gnu.tar.xz && \
    echo "${ESP_GCC_SHA256}  esp-gcc.tar.xz" | sha256sum -c - && \
    tar xJf esp-gcc.tar.xz && rm esp-gcc.tar.xz

# esptool with only what elf2image and merge_bin import (pyserial,
# intelhex), as wheels that the Python 3.8 of the base image can install
# without the network.
WORKDIR /wheels
RUN --mount=type=secret,id=ca_bundle,required=false \
    if [ -s /run/secrets/ca_bundle ]; then export PIP_CERT=/run/secrets/ca_bundle; fi && \
    pip3 wheel --no-deps --wheel-dir /wheels \
        esptool==${ESPTOOL_VERSION} pyserial==3.5 intelhex==2.3.0

FROM tizenrt/tizenrt:2.0.1

COPY --from=qemu-build /opt/qemu /opt/qemu
COPY --from=tools /opt/riscv32-esp-elf /opt/riscv32-esp-elf
COPY --from=tools /wheels /tmp/wheels

RUN pip3 install --no-index --no-deps /tmp/wheels/*.whl && \
    rm -rf /tmp/wheels && \
    esptool.py version

ENV PATH="/opt/riscv32-esp-elf/bin:/opt/qemu/bin:${PATH}"
