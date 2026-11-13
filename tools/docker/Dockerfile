#
# TizenRT build image with QEMU for the NuttX-based qemu-armv7a and
# mps2-an521 boards.
#
# It extends the public tizenrt/tizenrt:2.0.1 image (Ubuntu 16.04,
# arm-none-eabi-gcc 10.3.1, kconfig-frontends) with qemu-system-arm.
# QEMU of Ubuntu 16.04 is too old for '-M mps2-an521' and
# '-M virt -cpu cortex-a7', so QEMU 8.2 is built statically from the
# Ubuntu 24.04 source package and copied into the image.
#
# Build:
#   docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
#
# os/dbuild.sh uses the local image when CONFIG_DOCKER_VERSION="2.0.1-qemu".
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
RUN apt-get update && apt-get source --download-only qemu libslirp && \
    rm -rf /var/lib/apt/lists/* && \
    tar xf qemu_*.orig.tar.* && mv qemu-*/ qemu && \
    tar xf libslirp_*.orig.tar.* && mv libslirp-*/ libslirp

# libslirp gives QEMU the user-mode network backend ('-netdev user'), which
# connects the emulated NIC to the host without a tap device.  Ubuntu ships
# only the shared library, so build a static one for the static QEMU.
WORKDIR /build/libslirp
RUN meson setup build --prefix=/usr -Ddefault_library=static && \
    ninja -C build install

# The +ds source has no firmware blobs: do not unpack the edk2 images.
WORKDIR /build/qemu/build
RUN sed -i 's/unpack_edk2_blobs = bzip2.found()/unpack_edk2_blobs = false/' \
        ../meson.build && \
    ../configure --prefix=/opt/qemu --target-list=arm-softmmu --static \
        --without-default-features --enable-fdt=system --enable-slirp \
        --disable-docs --disable-install-blobs --disable-werror && \
    make -j"$(nproc)" && make install && \
    /opt/qemu/bin/qemu-system-arm --version

FROM tizenrt/tizenrt:2.0.1

COPY --from=qemu-build /opt/qemu /opt/qemu
ENV PATH="/opt/qemu/bin:${PATH}"
