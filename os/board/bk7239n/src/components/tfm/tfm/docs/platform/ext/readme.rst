###################################
Details for the platform/ext folder
###################################
This folder has code that has been imported from other projects. This means the
files in this folder and subfolders have Apache 2.0 license which is different
to BSD 3.0 license applied to the parent TF-M project.

.. Note::
    This folder is strictly Apache 2.0 with the exception of cmake files.
    Maintainers should be consulted if this needs to be revisited.

***********
Sub-folders
***********

accelerator
===========
This folder contains cmake and code files to interact cryptographic
accelerators.

In order to use a cryptographic accelerator, a platform must set
``CRYPTO_HW_ACCELERATOR_TYPE`` in preload.cmake. This option maps directly to
the subdirectories of the accelerator directory. Currently available
accelerators are : the CryptoCell ``cc312``, the STMicroelectronics accelerator
``stm`` .

A minimal API is exposed to interact with accelerators, the details of this api
are in ``accelerator/interface/crypto_hw.h``. Implementation of the API is
inside the subdirectory of the individual accelerator.

To configure a cryptographic accelerator at build time, two cmake options can be
specified.

- ``CRYPTO_HW_ACCELERATOR``
   - ``ON`` All possible mbedtls cryptographic operations will be offloaded to
     the accelerator.
   - ``OFF`` The cryptographic accelerator will be ignored and software
     cryptography will be used.

cmsis
=====
This folder contains core and compiler specific header files imported from the
``CMSIS_5`` project.

common
======

armclang and gcc
----------------
These contain the linker scripts used to configure the memory regions in TF-M
regions.

template
--------
This directory contains platform-independent dummy implementations of the
interfaces in ``platform/include``. These implementations can be built directly
for initial testing of a platform port, or used as a basic template for a real
implementation for a particular target. They **must not** be used in production
systems.

driver
======
This folder contains the headers with CMSIS compliant driver definitions that
that TF-M project expects a target to provide.

target_cfg.h
------------
This file is expected to define the following macros respectively.

- ``TFM_DRIVER_STDIO`` - This macro should expand to a structure of type
  ``ARM_DRIVER_USART``. TFM redirects its standard input and output to this
  instance of USART.
- ``NS_DRIVER_STDIO`` - This macro should expand to a structure of type
  ``ARM_DRIVER_USART``. Non-Secure application redirects its standard input and
  output to this instance of USART.

target
======
This folder contains the files for individual target. For a buildable target,
the directory path from the ``target`` directory to its ``CMakeLists.txt`` file
is the argument that would be given to ``-DTFM_PLATFORM=``.

The standard directory structure is as follows:

- target
   - <Vendor name>
      - common
      - <buildable target 1>
      - <buildable target 2>
      - <buildable target 3>

Each buildable target must contain the cmake files mandated in the section
below.

The ``common`` directory is not required, but can be used to contain code that
is used by multiple targets.

There must not be any directories inside the vendor directory that is not either
the ``common`` directory or a buildable platform, to avoid confusion about what
directories are a valid ``TFM_PLATFORM``.

Buildable target required cmake files
-------------------------------------

A buildable target must provide 3 mandatory cmake files. These files must all be
placed in the root of the buildable target directory.

preload.cmake
^^^^^^^^^^^^^

This file contains variable definitions that relate to the underlying hardware
of the target.

- ``TFM_SYSTEM_PROCESSOR``: The processor used by the target. The format is that
  same as the format used in the ``-mcpu=`` argument of GNUARM or ARMCLANG. The
  special ``+modifier`` syntax must not be used.

- ``TFM_SYSTEM_ARCHITECTURE``: The architecture used by the target. The format is
  that same as the format used in the ``-march=`` argument of GNUARM or ARMCLANG.
  The special ``+modifier`` syntax must not be used.

- ``TFM_SYSTEM_DSP``: Whether the target has the DSP feature of the given
  ``TFM_SYSTEM_PROCESSOR``
- ``CRYPTO_HW_ACCELERATOR_TYPE``: The type of cryptographic accelerator the
  target has, if it has one. This maps exactly to the subdirectories of
  ``platform/ext/accelerator``

Other than these particular cmake variables, it is permissible for the
``preload.cmake`` file to contain ``add_definitions`` statements, in order for
set compile definitions that are global for the hardware. This is commonly used
to select a particular set of code from a vendor SDK.

It is not permissible to contains code other than the above in a
``preload.cmake`` file, any general cmake code should be placed in
``CMakeLists.txt`` and any configuration options should be contained in
``config.cmake``

config.cmake
^^^^^^^^^^^^

This file collects platform-specific overrides to the configuration options.
This should only contain cmake options that are included in
``config_default.cmake``. These options should be set as ``CACHE`` variables, as
they are in ``config_default.cmake``.

CMakeLists.txt
^^^^^^^^^^^^^^

This file should contain all other required cmake code for the platform. This
primarily consists of the following:

- Adding an include directory to the target ``platform_region_defs``, which
  contains the headers ``flash_layout.h`` and ``region_defs.h``

- Adding startup and scatter files to the ``tfm_s``, ``tfm_ns`` and ``bl2``
  targets.

- linking ``CMSIS_5_tfm_ns`` to the correct version of the CMSIS RTX libraries,
  as defined in ``lib/ext/CMSIS_5/CMakeLists.txt``

- Adding required source files, include directories and compile definitions to
  the ``platform_s``, ``platform_ns`` and ``platform_bl2`` targets.

preload_ns.cmake
^^^^^^^^^^^^^^^^

This optional 4th cmake file is required only if the target is utilising
``TFM_MULTI_CORE_TOPOLOGY``. This file has the same format as ``preload.cmake``,
but instead details the hardware of the NS core that is **not** running the main
TF-M secure code.

Flash layout header file
------------------------
Target must provide a header file, called ``flash_layout.h``, which defines the
information explained in the follow subsections. The defines must be named
as they are in the subsections.

BL2 bootloader
^^^^^^^^^^^^^^
The BL2 bootloader requires the following definitions:

- ``FLASH_BASE_ADDRESS`` - Defines the first valid address in the flash.
- ``FLASH_AREA_BL2_OFFSET`` - Defines the offset from the flash base address
  where the BL2 - MCUBOOT area starts.
- ``FLASH_AREA_BL2_SIZE`` - Defines the size of the BL2 area.
- ``FLASH_AREA_SCRATCH_OFFSET`` - Defines the offset from the flash base
  address where the scratch area starts, which is used during image swapping.
- ``FLASH_AREA_SCRATCH_SIZE`` - Defines the size of the scratch area. The
  minimal size must be as the biggest sector size in the flash.
- ``FLASH_DEV_NAME`` - Specifies the flash device used by BL2.

The BL2 requires further definitions depending on the number of images, the
meaning of these macros are also slightly different:

- Required definitions in case of 1 image (S and NS images are concatenated
  and handled together as one binary blob):

    - ``FLASH_AREA_0_OFFSET`` - Defines the offset from the flash base address
      where the primary image area starts, which hosts the active firmware
      image.
    - ``FLASH_AREA_0_SIZE`` - Defines the size of the primary image area.
    - ``FLASH_AREA_2_OFFSET`` - Defines the offset from the flash base address
      where the secondary image area starts, which is a placeholder for new
      firmware images.
    - ``FLASH_AREA_2_SIZE`` - Defines the size of the secondary image area.

- Required definitions in case of 2 images (S and NS images are handled and
  updated separately):

    - ``FLASH_AREA_0_OFFSET`` - Defines the offset from the flash base address
      where the primary image areas start, which host the active firmware
      images. It is also the offset of the primary (active) secure image area.
    - ``FLASH_AREA_0_SIZE`` - Defines the size of the primary secure image area.
    - ``FLASH_AREA_1_OFFSET`` - Defines the offset from the flash base address
      where the primary (active) non-secure image area starts.
    - ``FLASH_AREA_1_SIZE`` - Defines the size of the primary non-secure image
      area.
    - ``FLASH_AREA_2_OFFSET`` - Defines the offset from the flash base address
      where the secondary image areas start, which are placeholders for new
      firmware images. It is also the offset of the secondary secure image area.
    - ``FLASH_AREA_2_SIZE`` - Defines the size of the secondary secure image
      area.
    - ``FLASH_AREA_3_OFFSET`` - Defines the offset from the flash base address
      where the secondary non-secure image area starts.
    - ``FLASH_AREA_3_SIZE`` - Defines the size of the secondary non-secure image
      area.

The table below shows a fraction of the flash layout in case of 2 and 1
updatable images with the related flash areas that hold the firmware images:

+-----------------------+--------------------+-----------------------+-----------------------------+
| Image number: 2                            | Image number: 1                                     |
+=======================+====================+=======================+=============================+
| **Flash area**        | **Content**        | **Flash area**        | **Content**                 |
+-----------------------+--------------------+-----------------------+-----------------------------+
| FLASH_AREA_0          | | Secure image     | FLASH_AREA_0          | | Secure + Non-secure image |
|                       | | primary slot     |                       | | primary slot              |
+-----------------------+--------------------+-----------------------+                             +
| FLASH_AREA_1          | | Non-secure image |                       |                             |
|                       | | primary slot     |                       |                             |
+-----------------------+--------------------+-----------------------+-----------------------------+
| FLASH_AREA_2          | | Secure image     | FLASH_AREA_2          | | Secure + Non-secure image |
|                       | | secondary slot   |                       | | secondary slot            |
+-----------------------+--------------------+-----------------------+                             +
| FLASH_AREA_3          | | Non-secure image |                       |                             |
|                       | | secondary slot   |                       |                             |
+-----------------------+--------------------+-----------------------+-----------------------------+
| FLASH_AREA_SCRATCH    | Scratch area       | FLASH_AREA_SCRATCH    | Scratch area                |
+-----------------------+--------------------+-----------------------+-----------------------------+

- ``IMAGE_EXECUTABLE_RAM_START`` - Defines the start of the region to which
  images are allowed to be loaded. Only used if ``MCUBOOT_UPGRADE_STRATEGY`` is
  configured to be ``RAM_LOAD``.

- ``IMAGE_EXECUTABLE_RAM_SIZE`` - Defines the size of the region to which images
  are allowed to be loaded. Only used if ``MCUBOOT_UPGRADE_STRATEGY`` is
  configured to be ``RAM_LOAD``.

Assemble tool
^^^^^^^^^^^^^
The ``assemble.py`` tool is used to concatenate secure and non-secure binary
to a single binary blob. It requires the following definitions:

- ``SECURE_IMAGE_OFFSET`` - Defines the offset from the single binary blob base
  address, where the secure image starts.
- ``SECURE_IMAGE_MAX_SIZE`` - Defines the maximum size of the secure image area.
- ``NON_SECURE_IMAGE_OFFSET`` - Defines the offset from the single binary blob
  base address,   where the non-secure image starts.
- ``NON_SECURE_IMAGE_MAX_SIZE`` - Defines the maximum size of the non-secure
  image area.

Image tool
^^^^^^^^^^^^^
The ``imgtool.py`` tool is used to handle the tasks related to signing the
binary. It requires the following definition:

- ``IMAGE_LOAD_ADDRESS`` - Defines the address to where the image is loaded and
  is executed from. Only used if ``MCUBOOT_UPGRADE_STRATEGY`` is configured to
  be ``RAM_LOAD``.

***************************************
Expose target support for HW components
***************************************
Services may require HW components to be supported by the target to enable some
features (e.g. PS service with rollback protection, etc). The following
definitions need to be set in the .cmake file if the target has the following
HW components:

- ``TARGET_NV_COUNTERS_ENABLE`` - Specifies that the target has non-volatile
  (NV) counters.

--------------

*Copyright (c) 2017-2021, Arm Limited. All rights reserved.*
*Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.*
