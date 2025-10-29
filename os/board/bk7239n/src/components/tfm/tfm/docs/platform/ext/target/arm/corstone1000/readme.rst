##################
ARM: Corstone-1000
##################

************
Introduction
************

The ARM's Corstone-1000 platform is a reference implementation of PSA FF-M
architecture where NSPE and SPE environments are partitioned into
Cortex-A35 and Cortex-M0+ respectively.

Cortex-M0+ acting as Secure Enclave is the Root-of-trust of SoC. Its
software comprises of two boot loading stages, i.e. Bl1 and Bl2, based on
mcuboot, and TF-M as run time software. Cortex-A35, also referred as host,
is completely treated as non-secure from the Secure Enclave perspective.
The Cortex-A35 is brought out of rest by Secure Enclave in aarch64 bit mode,
and boots the software ecosystem based on linux, u-boot, UEFI run time
services, TF-A and Optee.

The communication between NSPE and SPE is based on PSA IPC protocol running on
top of OpenAMP.

The secure enclave subsystem has ARM's CC-312 (Crypto Cell) hardware to
accelerate cryptographic operations.

***********
System boot
***********

- The SoC reset brings Secure Enclave (SE), that is Cortex-M0+, out of rest.
- SE executes the BL1 ROM code based on mcuboot.
- BL1 load, verifies and transfer execution to BL2 which is again based on mcuboot.
- BL2 loads and verifies TF-M and host's initial boot loader image.
- BL2 transfer the execution to the TF-M.
- During TF-M initialization, the host is reset.

*****
Build
*****

.. code-block::

    cmake -B build/ -S <tf-m-root>/ -DCMAKE_BUILD_TYPE=Debug -DTFM_TOOLCHAIN_FILE=<tf-m-root>/toolchain_GNUARM.cmake -DTFM_PLATFORM=arm/corstone1000
    cmake --build build -- install

The binaries will be installed inside:

.. code-block::

    ./build/install/outputs/ARM/CORSTONE1000

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
