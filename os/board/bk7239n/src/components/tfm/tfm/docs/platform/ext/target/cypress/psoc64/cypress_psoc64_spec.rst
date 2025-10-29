########################
Cypress PSoC64 Specifics
########################

*************
Prerequisites
*************

PSoC64 must first be provisioned with SecureBoot firmware and a provisioning packet
containing policy and secure keys. Please refer to the guide at
https://www.cypress.com/documentation/software-and-drivers/psoc-64-secure-mcu-secure-boot-sdk-user-guide
Use the following policy file for provisioning and signing:
policy_multi_CM0_CM4_tfm.json

Please make sure you have all required software installed as explained in the
:doc:`TF-M getting started </getting_started/tfm_getting_started>`.

Please also make sure that all the source code are fetched by following
:doc:`general building instruction </technical_references/instructions/tfm_build_instruction>`.

Install CySecureTools or update it to the newest version (2.0.0 at the time of
writing)

.. code-block:: bash

    pip3 install --upgrade --force-reinstall cysecuretools

PyOCD is installed by CySecureTools as a dependency.

For more details please refer to
`CySecureTools <https://pypi.org/project/cysecuretools>`_ page.

Install OpenOCD with PSoC6 support. Download the latest revision 4.1.0
or newer from:
https://github.com/cypresssemiconductorco/openocd/releases

Lastly, the PSoC64 board KitProg firmware needs to be 2.00.744 or greater.
Please use Cypress Modus Toolbox or CyProgrammer to update it if needed.

******************************************
Building Multi-Core TF-M on Cypress PSoC64
******************************************

Configuring the build
=====================

The build configuration for TF-M is provided to the build system using command
line arguments. Required arguments are noted below.

   * - -DTFM_PLATFORM=cypress/psoc64
     - Specifies target platform name ``psoc64``

   * - -DTFM_TOOLCHAIN_FILE=<path to toolchain file>
     - Specifies the compiler toolchain
       The possible values are:

         - ``<TFM root dir>/toolchain_ARMCLANG.cmake``
         - ``<TFM root dir>/toolchain_GNUARM.cmake``
         - ``<TFM root dir>/toolchain_IARARM.cmake``

see :ref:`tfm_cmake_configuration` for
more information.

Build Instructions
==================

The following instructions build multi-core TF-M without regression test suites
in Isolation Level 1 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

The following instructions build multi-core TF-M with regression test suites
in Isolation Level 1 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          -DTEST_S=ON -DTEST_NS=ON \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

The following instructions build multi-core TF-M with PSA API test suite for
the attestation service in Isolation Level 1 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          -DTEST_PSA_API=INITIAL_ATTESTATION \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

The following instructions build multi-core TF-M without regression test suites
in Isolation Level 2 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          -DTFM_ISOLATION_LEVEL=2 \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

The following instructions build multi-core TF-M with regression test suites
in Isolation Level 2 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          -DTFM_ISOLATION_LEVEL=2 \
          -DTEST_S=ON -DTEST_NS=ON \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

The following instructions build multi-core TF-M with PSA API test suite for
the protected storage service in Isolation Level 2 on Linux.
Both the compiler and the debugging type can be changed to other configurations
listed above.

.. code-block:: bash

    cd <TF-M base folder>
    cd <trusted-firmware-m folder>

    mkdir <build folder>
    pushd <build folder>
    cmake -DTFM_PLATFORM=cypress/psoc64 \
          -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
          -DTFM_ISOLATION_LEVEL=2 \
          -DTEST_PSA_API=PROTECTED_STORAGE \
          ../
    popd
    cmake --build <build folder> -- -j VERBOSE=1

*******
Signing
*******

Signing keys
============

Copy secure keys used in the board provisioning process to
platform/ext/target/cypress/psoc64/security/keys:

The keys included in the repository are for reference and development only.
DO NOT USE THESE KEYS IN ANY ACTUAL DEPLOYMENT!

Note: provisioned board in SECURE state is required, otherwise refer to
Cypress documentation for details on the provisioning process.
https://www.cypress.com/documentation/software-and-drivers/psoc-64-secure-mcu-secure-boot-sdk-user-guide

If the board was previously provisioned with signing keys and policy, copy
secure signing keys used in the board provisioning process to
platform/ext/target/cypress/psoc64/security/keys:

TFM_S_KEY.json
  Private OEM key for signing CM0P image

TFM_S_KEY_PRIV.pem
  Private OEM key for signing CM0P image in PEM format

TFM_NS_KEY.json
  Private OEM key for signing CM4 image

TFM_NS_KEY_PRIV.pem
  Private OEM key for signing CM4 image in PEM format

Alternatively, you can generate a new set of signing keys using cysecuretools
create-keys command and provision the keys to the board, if the previously
provisioned policy allows board's re-provisioning.

Initialize cysecuretools environment:

.. code-block:: bash

    cd platform/ext/target/cypress/psoc64/security
    cysecuretools -t cy8ckit-064s0s2-4343w init

Generate a new set of keys:

.. code-block:: bash

    cysecuretools -t cy8ckit-064s0s2-4343w -p policy/policy_multi_CM0_CM4_tfm.json create-keys

Re-provision the new keys to the board:

.. code-block:: bash

    cysecuretools -t cy8ckit-064s0s2-4343w -p policy/policy_multi_CM0_CM4_tfm.json re-provision-device

Signing the images
==================

Sign the images using CySecureTools CLI tool.
Note: the tool overwrites unsigned file with a signed one, it also creates an
unsigned copy <filename>_unsigned.hex.

SPE image:

.. code-block:: bash

    cysecuretools \
    --policy platform/ext/target/cypress/psoc64/security/policy/policy_multi_CM0_CM4_tfm.json \
    --target cy8ckit-064s0s2-4343w \
    sign-image \
    --hex <build folder>/bin/tfm_s.hex \
    --image-type BOOT \
    --image-id 1

NSPE image:

.. code-block:: bash

    cysecuretools \
    --policy platform/ext/target/cypress/psoc64/security/policy/policy_multi_CM0_CM4_tfm.json \
    --target cy8ckit-064s0s2-4343w \
    sign-image \
    --hex <build folder>/bin/tfm_ns.hex \
    --image-type BOOT \
    --image-id 16

* CySecureTools sign-image overwrites unsigned file with a signed one,
  also it creates an unsigned copy _unsigned.hex.

* image-type option: "--image-type BOOT" creates a signed hex file with offsets
  for the primary image slot. Use "--image-type UPGRADE" if you want to create
  an image for the secondary "upgrade" slot.
  When booting, CyBootloader will validate image in the secondary slot and copy
  it to the primary boot slot.

* image-id option: Each image has its own ID. By default, SPE image running on
  CM0P core has ID=1, NSPE image running on CM4 core has ID=16. Refer to the
  policy file for the actual ID's.


**********************
Programming the Device
**********************

After building and signing, the TFM images must be programmed into flash
memory on the PSoC64 device. There are three methods to program it.

DAPLink mode
============

Using KitProg3 mode button, switch it to DAPLink mode.
Mode LED should start blinking rapidly and depending on the host computer
settings DAPLINK will be mounted as a media storage device.
Otherwise, mount it manually.

Copy tfm hex files one by one to the DAPLINK device:

.. code-block:: bash

    cp <build folder>/bin/tfm_ns.hex <mount point>/DAPLINK/; sync
    cp <build folder>/bin/tfm_s.hex <mount point>/DAPLINK/; sync

OpenOCD
=======

Using KitProg3 mode button, switch to KitProg3 CMSIS-DAP BULK. Status LED
should be ON and not blinking for CMSIS-DAP BULK.

Before programming the images, erase PS partition if needed.
This will clear all data and force PS to reformat partition.

.. code-block:: bash

    ${OPENOCD_PATH}/bin/openocd \
            -s ${OPENOCD_PATH}/scripts \
            -f interface/kitprog3.cfg \
            -f target/psoc6_2m_secure.cfg \
            -c "init; reset init" \
            -c "flash erase_address 0x101c0000 0x10000" \
            -c "shutdown"

Note that the ``0x101C0000`` in the command above must match the PS start
address of the secure primary image specified in the file:

    platform/ext/target/cypress/psoc64/partition/flash_layout.h

so be sure to change it if you change that file.

To program the signed tfm_s and tfm_ns images to the device with openocd
(assuming OPENOCD_PATH is pointing at the openocd installation directory)
run the following commands:

.. code-block:: bash

    OPENOCD_PATH=<cyprogrammer dir>/openocd
    BUILD_DIR=<build folder>

    ${OPENOCD_PATH}/bin/openocd \
            -s ${OPENOCD_PATH}/scripts \
            -f interface/kitprog3.cfg \
            -f target/psoc6_2m_secure.cfg \
            -c "init; reset init" \
            -c "flash write_image erase ${BUILD_DIR}/bin/tfm_s.hex" \
            -c "shutdown"

    ${OPENOCD_PATH}/bin/openocd \
            -s ${OPENOCD_PATH}/scripts \
            -f interface/kitprog3.cfg \
            -f target/psoc6_2m_secure.cfg \
            -c "init; reset init" \
            -c "flash write_image erase ${BUILD_DIR}/bin/tfm_ns.hex" \
            -c "reset run"

PyOCD
=====

PyOCD is installed by CySecureTools automatically. It can be used
to program TFM images into the board.

Using KitProg3 mode button, switch to KitProg3 DAPLink mode.
Mode LED should start blinking rapidly.

Optionally, erase PS partition:

.. code-block:: bash

    pyocd erase -b CY8CKIT-064S0S2-4343W -s 0x101c0000+0x10000

To program the signed tfm_s and tfm_ns images to the device with pyocd
run the following commands:

.. code-block:: bash

    pyocd flash -b CY8CKIT-064S0S2-4343W ${BUILD_DIR}/bin/tfm_s.hex

    pyocd flash -b CY8CKIT-064S0S2-4343W ${BUILD_DIR}/bin/tfm_ns.hex


********************************
Provisioning device certificates
********************************

1. If not done yet, change to the psoc64 security directory and initialize
   cysecuretools environment:

.. code-block:: bash

    cd platform/ext/target/cypress/psoc64/security
    cysecuretools -t cy8ckit-064s0s2-4343w init

2. Create and copy rootCA files to "certificates" directory next to the policy
   directory (please refer to documentation of the used OS)

3. Switch the board in DAPLink mode by pressing the mode button or by issuing
   the following fw-loader command (fw-loader comes with Modus ToolBox software).
   Mode LED should be slowly blinking:

.. code-block:: bash

    fw-loader --mode kp3-daplink

4. Run reprov_helper.py. If running the script with default parameters,
   the script can be run as is:

.. code-block:: bash

    python3 reprov_helper.py

   Otherwise, run it with --help parameter to get the full list of options.

5. Confirm selected options. When prompted for a serial number, enter the board
   unique serial number (digits only, e.g. 00183).

6. Script will ask if you want to create new signing keys. Answer Yes to
   generate new signing keys in the keys directory, or No to retain and use the
   existing keys. After re-provisioning, from now on any images for
   this board will have to be signed with these keys.

7. The script will erase user images.
   Then the script will read device public key and create device certificates
   based on the board serial number, root certificate and the device public key.


*************************************************************
Provisioning Amazon Web Services root and device certificates
*************************************************************
Device running Amazon FreeRTOS and using Amazon Web Services (AWS)
requires AWS root and unique device certificate signed with the device key
for authentication. These certificates have to be provisioned into device.
The process is the following:

1. If not done yet, change to the psoc64 security directory and initialize
   cysecuretools environment:

.. code-block:: bash

    cd platform/ext/target/cypress/psoc64/security
    cysecuretools -t cy8ckit-064s0s2-4343w init

2. Obtain and copy rootCA files to  "certificates" directory next to the policy
   directory (please refer to AWS documentation)

3. Switch the board in DAPLink mode by pressing the mode button or by issuing
   the following fw-loader command (fw-loader comes with Modus ToolBox software).
   Mode LED should be slowly blinking:

.. code-block:: bash

    fw-loader --mode kp3-daplink

4. Run reprov_helper.py. If running the script with default parameters,
   the script can be run as is:

.. code-block:: bash

    python3 reprov_helper.py

   Otherwise, run it with --help parameter to get the full list of options.

5. Confirm selected options. When prompted for a serial number, enter the board
   unique serial number (digits only, e.g. 00183).

6. Script will ask if you want to create new signing keys. Answer Yes to
   generate new signing keys in the keys directory, or No to retain and use the
   existing keys. After re-provisioning, from now on any images for
   this board will have to be signed with these keys.

7. The script will erase user images.
   Then the script will read device public key and create device certificates
   based on the board serial number, root certificate and the device public key.

--------------

*Copyright (c) 2017-2021, Arm Limited. All rights reserved.*

*Copyright (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.*
