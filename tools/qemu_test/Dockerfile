ARG UBUNTU_VERSION=24.04
ARG QEMU_VERSION=10.2.0-rc3

FROM ubuntu:${UBUNTU_VERSION} AS qemu-builder

ARG DEBIAN_FRONTEND=noninteractive
ARG QEMU_VERSION

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        curl \
        libcapstone-dev \
        libfdt-dev \
        libglib2.0-dev \
        libpixman-1-dev \
        meson \
        ninja-build \
        pkg-config \
        python3 \
        python3-venv \
        xz-utils \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

RUN curl -L "https://download.qemu.org/qemu-${QEMU_VERSION}.tar.xz" -o qemu.tar.xz \
    && tar -xf qemu.tar.xz \
    && mv "qemu-${QEMU_VERSION}" qemu \
    && rm qemu.tar.xz

WORKDIR /src/qemu

# Match the qemu-virt flash layout by building QEMU with 4 KiB pflash sectors.
RUN grep -n 'VIRT_FLASH_SECTOR_SIZE' hw/arm/virt.c \
    && sed -i '1243s/.*/#define VIRT_FLASH_SECTOR_SIZE (4 * KiB)/' hw/arm/virt.c \
    && grep -n 'VIRT_FLASH_SECTOR_SIZE' hw/arm/virt.c \
    && ! grep -q '256 \\* KiB' hw/arm/virt.c \
    && ./configure \
        --target-list=arm-softmmu,aarch64-softmmu \
        --disable-slirp \
        --disable-werror \
    && make -j"$(nproc)"

FROM ubuntu:${UBUNTU_VERSION}

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        libcapstone4 \
        libfdt1 \
        libglib2.0-0 \
        libpixman-1-0 \
        python3 \
        python3-pexpect \
        zlib1g \
    && rm -rf /var/lib/apt/lists/*

COPY --from=qemu-builder /src/qemu/build/qemu-system-arm /usr/local/bin/
COPY --from=qemu-builder /src/qemu/build/qemu-system-aarch64 /usr/local/bin/

WORKDIR /workspace

EXPOSE 1234
