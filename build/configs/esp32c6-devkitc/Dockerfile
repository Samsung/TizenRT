#
# TizenRT image with the Espressif QEMU for the NuttX-based
# esp32c6-devkitc board.
#
# Upstream QEMU has no ESP32-C6 machine.  The QEMU fork of Espressif
# (QEMU 9.2.2 based) has the 'esp32c6' machine since the
# esp-develop-9.2.2-20261109 release.  The release binaries need glibc
# 2.31 and do not run on the base image (Ubuntu 16.04, glibc 2.23), so
# the fork is built statically from the commit of that release and
# installed in /opt/qemu-esp with the ESP32-C6 ROM image, beside the
# upstream QEMU 8.2 of tizenrt/tizenrt:2.0.1-qemu-riscv
# (tools/docker/riscv), which stays the default qemu-system-riscv32.
#
# Build (after tools/docker/riscv):
#   docker build -t tizenrt/tizenrt:2.0.1-qemu-esp32c6 build/configs/esp32c6-devkitc
#
# Behind a proxy that re-signs TLS, pass the proxy and the CA bundle of
# the host (git uses it when the secret is given):
#   docker build --build-arg http_proxy --build-arg https_proxy \
#       --secret id=ca_bundle,src=/etc/ssl/certs/ca-certificates.crt \
#       -t tizenrt/tizenrt:2.0.1-qemu-esp32c6 build/configs/esp32c6-devkitc
#
# The build image of esp32c6-devkitc is tizenrt/tizenrt:2.0.1-qemu-riscv
# (CONFIG_DOCKER_VERSION="2.0.1-qemu-riscv"); this image is only needed
# to run build/configs/esp32c6-devkitc/qemu_run.sh.
#

FROM ubuntu:24.04 AS qemu-build

ARG DEBIAN_FRONTEND=noninteractive
ARG QEMU_ESP_URL=https://github.com/espressif/qemu.git
ARG QEMU_ESP_REV=7411393a2f61e6f0b05af00276d30780769e0839

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        bison build-essential ca-certificates flex git libfdt-dev \
        libgcrypt20-dev libglib2.0-dev libpcre2-dev libpixman-1-dev \
        ninja-build pkg-config python3 python3-venv zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /build/qemu
RUN --mount=type=secret,id=ca_bundle,required=false \
    if [ -s /run/secrets/ca_bundle ]; then export GIT_SSL_CAINFO=/run/secrets/ca_bundle; fi && \
    git init -q . && \
    git remote add origin ${QEMU_ESP_URL} && \
    git fetch -q --depth 1 origin ${QEMU_ESP_REV} && \
    git checkout -q FETCH_HEAD

# The ESP32-C6 UART model decodes the FIFO thresholds and the RX timeout
# with the register layout of the ESP32-C3: the RX timeout interrupt, which
# the serial driver of NuttX needs to get the console input, never comes.
# The patch decodes UART_CONF1_REG and UART_TOUT_CONF_SYNC_REG as the
# ESP32-C6 has them.

COPY qemu-esp32c6-uart.patch /build/
RUN git apply /build/qemu-esp32c6-uart.patch

# riscv32-softmmu only, static, without the documentation and the
# firmware blobs.  libgcrypt is needed: the ESP32-C6 machine is built
# only with it (AES, SHA, RSA and ECC peripherals).  configure fetches
# the meson subprojects (keycodemapdb, berkeley-softfloat-3) with git.
#
# The meson.build of the fork declares the slirp dependency as found
# even with the feature disabled and net/slirp.c then fails to build
# without libslirp.h: skip that block unless slirp is asked for.

WORKDIR /build/qemu/build
RUN --mount=type=secret,id=ca_bundle,required=false \
    if [ -s /run/secrets/ca_bundle ]; then export GIT_SSL_CAINFO=/run/secrets/ca_bundle; fi && \
    sed -i "s/^if not get_option('slirp').auto() or have_system\$/if get_option('slirp').enabled()/" \
        ../meson.build && \
    ! grep -q "get_option('slirp').auto() or have_system" ../meson.build && \
    ../configure --prefix=/opt/qemu-esp --target-list=riscv32-softmmu --static \
        --without-default-features --enable-fdt=system --enable-gcrypt \
        --disable-docs --disable-install-blobs --disable-werror && \
    make -j"$(nproc)" && make install && \
    install -D -m 0644 ../pc-bios/esp32c6-rom.bin \
        /opt/qemu-esp/share/qemu/esp32c6-rom.bin && \
    /opt/qemu-esp/bin/qemu-system-riscv32 --version && \
    /opt/qemu-esp/bin/qemu-system-riscv32 -M help | grep -q esp32c6

FROM tizenrt/tizenrt:2.0.1-qemu-riscv

COPY --from=qemu-build /opt/qemu-esp /opt/qemu-esp
