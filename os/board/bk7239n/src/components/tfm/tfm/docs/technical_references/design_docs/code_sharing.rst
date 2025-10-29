######################################################
Code sharing between independently linked XIP binaries
######################################################

:Author: Tamas Ban
:Organization: Arm Limited
:Contact: tamas.ban@arm.com

**********
Motivation
**********
Cortex-M devices are usually constrained in terms of flash and RAM. Therefore,
it is often challenging to fit bigger projects in the available memory. The PSA
specifications require a device to both have a secure boot process in place at
device boot-up time, and to have a partition in the SPE which provides
cryptographic services at runtime. These two entities have some overlapping
functionality. Some cryptographic primitives (e.g. hash calculation and digital
signature verification) are required both in the bootloader and the runtime
environment. In the current TF-M code base, both firmware components use the
mbed-crypto library to implement these requirements. During the build process,
the mbed-crpyto library is built twice, with different configurations (the
bootloader requires less functionality) and then linked to the corresponding
firmware component. As a result of this workflow, the same code is placed in the
flash twice. For example, the code for the SHA-256 algorithm is included in
MCUboot, but the exact same code is duplicated in the SPE cryptography
partition. In most cases, there is no memory isolation between the bootloader
and the SPE, because both are part of the PRoT code and run in the secure
domain. So, in theory, the code of the common cryptographic algorithms could be
reused among these firmware components. This could result in a big reduction in
code footprint, because the cryptographic algorithms are usually flash hungry.
Code size reduction can be a good opportunity for very constrained devices,
which might need to use TF-M Profile Small anyway.

*******************
Technical challenge
*******************
Code sharing in a regular OS environment is easily achievable with dynamically
linked libraries. However, this is not the case in Cortex-M systems where
applications might run bare-metal, or on top of an RTOS, which usually lacks
dynamic loading functionality. One major challenge to be solved in the Cortex-M
space is how to share code between independently linked XIP applications that
are tied to a certain memory address range to be executable and have absolute
function and global data memory addresses. In this case, the code is not
relocatable, and in most cases, there is no loader functionality in the system
that can perform code relocation. Also, the lack of an MMU makes the address
space flat, constant and not reconfigurable at runtime by privileged code.

One other difficulty is that the bootloader and the runtime use the same RAM
area during execution. The runtime firmware is executed strictly after the
bootloader, so normally, it can reuse the whole secure RAM area, as it would be
the exclusive user. No attention needs to be paid as to where global data is
placed by the linker. The bootloader does not need to retain its state. The low
level startup of the runtime firmware can freely overwrite the RAM with its data
without corrupting bootloader functionality. However, with code sharing between
bootloader and runtime firmware, these statements are no longer true. Global
variables used by the shared code must either retain their value or must be
reinitialised during low level startup of the runtime firmware. The startup code
is not allowed to overwrite the shared global variables with arbitrary data. The
following design proposal provides a solution to these challenges.

**************
Design concept
**************
The bootloader is sometimes implemented as ROM code (BL1) or stored in a region
of the flash which is lockable, to prevent tampering. In a secure system, the
bootloader is immutable code and thus implements a part of the Root of Trust
anchor in the device, which is trusted implicitly. The shared code is primarily
part of the bootloader, and is reused by the runtime SPE firmware at a later
stage. Not all of the bootloader code is reused by the runtime SPE, only some
cryptographic functions.

Simplified steps of building with code sharing enabled:

  - Complete the bootloader build process to have a final image that contains
    the absolute addresses of the shared functions, and the global variables
    used by these functions.
  - Extract the addresses of the functions and related global variables that are
    intended to be shared from the bootloader executable.
  - When building runtime firmware, provide the absolute addresses of the shared
    symbols to the linker, so that it can pick them up, instead of instantiating
    them again.

The execution flow looks like this:

.. code-block:: bash

    SPE        MCUboot func1()    MCUboot func2()     MCUboot func3()
     |
     |     Hash()
     |------------->|
                    |----------------->|
                                       |
                          Return       |
          Return    |<-----------------|
     |<-------------|
     |
     |
     |----------------------------------------------------->|
                                                            |
             Function pointer in shared global data()       |
     |<-----------------------------------------------------|
     |
     |                       Return
     |----------------------------------------------------->|
                                                            |
                             Return                         |
     |<-----------------------------------------------------|
     |
     |

The execution flow usually returns from a shared function back to the SPE with
an ordinary function return. So usually, once a shared function is called in the
call path, all further functions in the call chain will be shared as well.
However, this is not always the case, as it is possible for a shared function to
call a non-shared function in SPE code through a global function pointer.

For shared global variables, a dedicated data section must be allocated in the
linker configuration file. This area must have the same memory address in both
MCUboot's and the SPE's linker files, to ensure the integrity of the variables.
For simplicity's sake, this section is placed at the very beginning of the RAM
area. Also, the RAM wiping functionality at the end of the secure boot flow
(that is intended to remove any possible secrets from the RAM) must not clear
this area. Furthermore, it must be ensured that the linker places shared globals
into this data section. There are two way to achieve this:

 - Put a filter pattern in the section body that matches the shared global
   variables.
 - Mark the global variables in the source code with special attribute
   `__attribute__((section(<NAME_OF_SHARED_SYMBOL_SECTION>)))`

RAM memory layout in MCUboot with code sharing enabled:

.. code-block:: bash

    +------------------+
    |  Shared symbols  |
    +------------------+
    | Shared boot data |
    +------------------+
    |      Data        |
    +------------------+
    |    Stack (MSP)   |
    +------------------+
    |      Heap        |
    +------------------+

RAM memory layout in SPE with code sharing enabled:

.. code-block:: bash

    +-------------------+
    |  Shared symbols   |
    +-------------------+
    | Shared boot data  |
    +-------------------+
    |    Stack (MSP)    |
    +-------------------+
    |    Stack (PSP)    |
    +-------------------+
    | Partition X Data  |
    +-------------------+
    | Partition X Stack |
    +-------------------+
              .
              .
              .
    +-------------------+
    | Partition Z Data  |
    +-------------------+
    | Partition Z Stack |
    +-------------------+
    |     PRoT Data     |
    +-------------------+
    |       Heap        |
    +-------------------+

Patching mbedTLS
================
In order to share some global function pointers from mbed-crypto that are
related to dynamic memory allocation, their scope must be extended from private
to global. This is needed because some compiler toolchain only extract the
addresses of public functions and global variables, and extraction of addresses
is a requirement to share them among binaries. Therefore, a short patch was
created for the mbed-crypto library, which "globalises" these function pointers:

`lib/ext/mbedcrypto/0005-Enable-crypto-code-sharing-between-independent-binar.patch`

The patch need to manually applied in the mbedtls repo, if code sharing is
enabled. The patch has no effect on the functional behaviour of the
cryptographic library, it only extends the scope of some variables.

*************
Tools support
*************
All the currently supported compilers provide a way to achieve the above
objectives. However, there is no standard way, which means that the code sharing
functionality must be implemented on a per compiler basis. The following steps
are needed:

 - Extraction of the addresses of all global symbols.
 - The filtering out of the addresses of symbols that aren't shared. The goal is
   to not need to list all the shared symbols by name. Only a simple pattern
   has to be provided, which matches the beginning of the symbol's name.
   Matching symbols will be shared. Examples are in :
   `bl2/src/shared_symbol_template.txt`
 - Provision of the addresses of shared symbols to the linker during the SPE
   build process.
 - The resolution of symbol collisions during SPE linking. Because mbed-crypto
   is linked to both firmware components as a static library, the external
   shared symbols will conflict with the same symbols found within it. In order
   to prioritize the external symbol, the symbol with the same name in
   mbed-crypto must be marked as weak in the symbol table.

The above functionalities are implemented in the toolchain specific CMake files:

 - `toolchain_ARMCLANG.cmake`
 - `toolchain_GNUARM.cmake`

By the following two functions:

 - `compiler_create_shared_code()`: Extract and filter shared symbol addresses
   from MCUboot.
 - `compiler_link_shared_code()`: Link shared code to the SPE and resolve symbol
   conflict issues.

ARMCLANG
========
The toolchain specific steps are:

 - Extract all symbols from MCUboot: add `-symdefs` to the compiler command line
 - Filter shared symbols: call CMake script `FilterSharedSymbols.cmake`
 - Weaken duplicated (shared) symbols in the mbed-crypto static library that are
   linked to the SPE: `arm-none-eabi-objcopy`
 - Link shared code to SPE: Add the filtered output of `-symdefs` to the SPE
   source file list.

GNUARM
======
The toolchain specific steps are:

 - Extract all symbols from MCUboot: `arm-none-eabi-nm`
 - Filter shared symbols: call CMake script: `FilterSharedSymbols.cmake`
 - Strip unshared code from MCUboot:  `arm-none-eabi-strip`
 - Weaken duplicated (shared) symbols in the mbed-crypto static library that are
   linked to the SPE: `arm-none-eabi-objcopy`
 - Link shared code to SPE: Add `-Wl -R <SHARED_STRIPPED_CODE.axf>` to the
   compiler command line

IAR
===
Functionality currently not implemented, but the toolchain supports doing it.

**************************
Memory footprint reduction
**************************
Build type: MinSizeRel
Platform: mps2/an521
Version: TF-Mv1.2.0 + code sharing patches
MCUboot image encryption support is disabled.

+------------------+-------------------+-------------------+-------------------+
|                  |   ConfigDefault   |  ConfigProfile-M  |  ConfigProfile-S  |
+------------------+----------+--------+----------+--------+----------+--------+
|                  | ARMCLANG | GNUARM | ARMCLANG | GNUARM | ARMCLANG | GNUARM |
+------------------+----------+--------+----------+--------+----------+--------+
| CODE_SHARING=OFF |   122268 | 124572 |   75936 |   75996 |    50336 |  50224 |
+------------------+----------+--------+----------+--------+----------+--------+
| CODE_SHARING=ON  |   113264 | 115500 |   70400 |   70336 |    48840 |  48988 |
+------------------+----------+--------+----------+--------+----------+--------+
| Difference       |     9004 |   9072 |    5536 |    5660 |     1496 |   1236 |
+------------------+----------+--------+----------+--------+----------+--------+

If MCUboot image encryption support is enabled then saving could be up to
~13-15KB.

.. Note::

   Code sharing on Musca-B1 was tested only with SW only crypto, so crypto
   hardware acceleration must be turned off: -DCRYPTO_HW_ACCELERATOR=OFF


*************************
Useability considerations
*************************
Functions that only use local variables can be shared easily. However, functions
that rely on global variables are a bit tricky. They can still be shared, but
all global variables must be placed in the shared symbol section, to prevent
overwriting and to enable the retention of their values.

Some global variables might need to be reinitialised to their original values by
runtime firmware, if they have been used by the bootloader, but need to have
their original value when runtime firmware starts to use them. If so, the
reinitialising functionality must be implemented explicitly, because the low
level startup code in the SPE does not initialise the shared variables, which
means they retain their value after MCUboot stops running.

If a bug is discovered in the shared code, it cannot be fixed with a firmware
upgrade, if the bootloader code is immutable. If this is the case, disabling
code sharing might be a solution, as the new runtime firmware could contain the
fixed code instead of relying on the unfixed shared code. However, this would
increase code footprint.

API backward compatibility also can be an issue. If the API has changed in newer
version of the shared code. Then new code cannot rely on the shared version.
The changed code and all the other shared code where it is referenced from must
be ignored and the updated version of the functions must be compiled in the
SPE binary. The mbedTLS library is API compatible with its current version
(``v2.24.0``) since the ``mbedtls-2.7.0 release`` (2018-02-03).

To minimise the risk of incompatibility, use the same compiler flags to build
both firmware components.

The artifacts of the shared code extraction steps must be preserved so as to
remain available if new SPE firmware (that relies on shared code) is built and
released. Those files are necessary to know the address of shared symbols when
linking the SPE.

************************
How to use code sharing?
************************
Considering the above, code sharing is an optional feature, which is disabled
by default. It can be enabled from the command line with a compile time switch:

 - `TFM_CODE_SHARING`: Set to `ON` to enable code sharing.

With the default settings, only the common part of the mbed-crypto library is
shared, between MCUboot and the SPE. However, there might be other device
specific code (e.g. device drivers) that could be shared. The shared
cryptography code consists mainly of the SHA-256 algorithm, the `bignum` library
and some RSA related functions. If image encryption support is enabled in
MCUboot, then AES algorithms can be shared as well.

Sharing code between the SPE and an external project is possible, even if
MCUboot isn't used as the bootloader. For example, a custom bootloader can also
be built in such a way as to create the necessary artifacts to share some of its
code with the SPE. The same artifacts must be created like the case of MCUboot:

 - `shared_symbols_name.txt`: Contains the name of the shared symbols. Used by
    the script that prevents symbol collision.
 - `shared_symbols_address.txt`: Contains the type, address and name of shared
   symbols. Used by the linker when linking runtime SPE.
 - `shared_code.axf`: GNUARM specific. The stripped version of the firmware
   component, only contains the shared code. It is used by the linker when
   linking the SPE.

.. Note::

   The artifacts of the shared code extraction steps must be preserved to be
   able to link them to any future SPE version.

When an external project is sharing code with the SPE, the `SHARED_CODE_PATH`
compile time switch must be set to the path of the artifacts mentioned above.

********************
Further improvements
********************
This design focuses only on sharing the cryptography code. However, other code
could be shared as well. Some possibilities:

- Flash driver
- Serial driver
- Image metadata parsing code
- etc.

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*