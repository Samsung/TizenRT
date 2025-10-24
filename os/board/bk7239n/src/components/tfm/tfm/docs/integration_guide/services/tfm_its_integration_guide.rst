#######################################################
TF-M Internal Trusted Storage Service Integration Guide
#######################################################

************
Introduction
************
TF-M Internal Trusted Storage (ITS) service implements PSA Internal Trusted
Storage APIs.

The service is backed by hardware isolation of the flash access domain and
relies on hardware to isolate the flash area from access by the Non-secure
Processing Environment, as well as the Application Root of Trust at higher
levels of isolation.

The current ITS service design relies on hardware abstraction provided by TF-M.
The ITS service provides a non-hierarchical storage model, as a filesystem,
where all the assets are managed by a linearly indexed list of metadata.

The design addresses the following high level requirements as well:

- **Confidentiality** - Resistance to unauthorised accesses through
  hardware/software attacks. Assumed to be provided by the internal flash
  device, backed by hardware isolation.

- **Access Authentication** - Mechanism to establish requester's identity (a
  non-secure entity, secure entity, or a remote server).

- **Integrity** - Resistance to tampering by attackers with physical access is
  assumed to be provided by the internal flash device itself, while resistance
  to tampering by Non-secure or App RoT attackers also requires hardware
  isolation.

- **Reliability** - Resistance to power failure scenarios and incomplete write
  cycles.

- **Configurability** - High level of configurability to scale up/down memory
  footprint to cater for a variety of devices with varying requirements.

- **Performance** - Optimized to be used for resource constrained devices with
  very small silicon footprint, the PPA (power, performance, area) should be
  optimal.

*******************************
Current ITS Service Limitations
*******************************
- **Fragmentation** - The current design does not support fragmentation, as an
  asset is stored in a contiguous space in a block. This means that the maximum
  asset size can only be up-to a block size. Each block can potentially store
  multiple assets.
  A delete operation implicitly moves all the assets towards the top of the
  block to avoid fragmentation within block. However, this may also result in
  unutilized space at the end of each block.

- **Non-hierarchical storage model** - The current design uses a
  non-hierarchical storage model, as a filesystem, where all the assets are
  managed by a linearly indexed list of metadata. This model locates the
  metadata in blocks which are always stored in the same flash location. That
  increases the number of writes in a specific flash location as every change in
  the storage area requires a metadata update.

- **Protection against physical storage medium failure** - Complete handling of
  inherent failures of storage mediums (e.g. bad blocks in a NAND based device)
  is not supported by the current design.

- **Lifecycle management** - Currently, it does not support any subscription
  based keys and certificates required in a secure lifecycle management. Hence,
  an asset's validity time-stamp can not be invalidated based on the system
  time.

- **Provisioning vs user/device data** - In the current design, all assets are
  treated in the same manner. In an alternative design, it may be required to
  create separate partitions for provisioning content and user/device generated
  content. This is to allow safe update of provisioning data during firmware
  updates without the need to wipe out the user/device generated data.

**************
Code Structure
**************
TF-M Internal Trusted Storage service code is located in
``secure_fw/partitions/internal_trusted_storage/`` and is divided as follows:

    - Core files
    - Flash filesystem interfaces
    - Flash interfaces

The PSA ITS interfaces for the TF-M ITS service are located in
``interface/include/psa``.

PSA Internal Trusted Storage Interfaces
=======================================

The TF-M ITS service exposes the following mandatory PSA ITS interfaces
version 1.0:

.. code-block:: c

    psa_status_t psa_its_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags);
    psa_status_t psa_its_get(psa_storage_uid_t uid, size_t data_offset, size_t data_size, void *p_data, size_t *p_data_length);
    psa_status_t psa_its_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info);
    psa_status_t psa_its_remove(psa_storage_uid_t uid);

These PSA ITS interfaces and TF-M ITS types are defined and documented in
``interface/include/psa/storage_common.h``,
``interface/include/psa/internal_trusted_storage.h``, and
``interface/include/tfm_its_defs.h``

Core Files
==========
- ``tfm_its_req_mngr.c`` - Contains the ITS request manager implementation which
  handles all requests which arrive to the service. This layer extracts the
  arguments from the input and output vectors, and it calls the internal trusted
  storage layer with the provided parameters.

- ``tfm_internal_trusted_storage.c`` - Contains the TF-M internal trusted
  storage API implementations which are the entry points to the ITS service.
  Constructs a filesystem configuration using information from the ITS HAL,
  allocates a filesystem context for ITS and makes appropriate FS calls. Also
  handles requests from the PS partition with a separate FS config and context.

- ``its_utils.c`` - Contains common and basic functionalities used across the
  ITS service code.

Flash Filesystem Interface
==========================
- ``flash_fs/its_flash_fs.h`` - Abstracts the flash filesystem operations used
  by the internal trusted storage service. The purpose of this abstraction is to
  have the ability to plug-in other filesystems or filesystem proxies
  (supplicant).

- ``flash_fs/its_flash_fs.c`` - Contains the ``its_flash_fs`` implementation for
  the required interfaces.

- ``flash_fs/its_flash_fs_mblock.c`` - Contains the metadata block manipulation
  functions required to implement the ``its_flash_fs`` interfaces in
  ``flash_fs/its_flash_fs.c``.

- ``flash_fs/its_flash_fs_dblock.c`` - Contains the data block manipulation
  functions required to implement the ``its_flash_fs`` interfaces in
  ``flash_fs/its_flash_fs.c``.

The system integrator **may** replace this implementation with its own
flash filesystem implementation or filesystem proxy (supplicant).

Flash Interface
===============
The ITS filesystem flash interface is defined by ``struct its_flash_fs_ops_t``
in ``flash_fs/its_flash_fs.h``.

Implementations of the ITS filesystem flash interface for different types of
storage can be found in the ```internal_trusted_storage/flash`` directory.

- ``flash/its_flash.h`` - Helper header that includes the correct ITS flash
  interface implementation for the target, and abstracts the allocation of
  different flash device types.

- ``flash/its_flash_nand.c`` - Implements the ITS flash interface for a NAND
  flash device, on top of the CMSIS flash interface implemented by the target.
  This implementation writes entire block updates in one-shot, so the CMSIS
  flash implementation **must** be able to detect incomplete writes and return
  an error the next time the block is read.

- ``flash/its_flash_nor.c`` - Implements the ITS flash interface for a NOR flash
  device, on top of the CMSIS flash interface implemented by the target.

- ``flash/its_flash_ram.c`` - Implements the ITS flash interface for an emulated
  flash device using RAM, on top of the CMSIS flash interface implemented by the
  target.

The CMSIS flash interface **must** be implemented for each target based on its
flash controller.

The ITS flash interface depends on target-specific definitions from
``platform/ext/target/<TARGET_NAME>/partition/flash_layout.h``.
Please see the `Internal Trusted Storage Service HAL` section for details.

*****************************
ITS Service Integration Guide
*****************************
This section describes mandatory (i.e. **must** implement) or optional
(i.e. **may** implement) interfaces which the system integrator has to take in
to account in order to integrate the internal trusted storage service in a new
platform.

Flash Interface
===============
For ITS service operations, a contiguous set of blocks must be earmarked for
the internal trusted storage area. The design requires either 2 blocks, or any
number of blocks greater than or equal to 4. Total number of blocks can not be
0, 1 or 3. This is a design choice limitation to provide power failure safe
update operations.

Maximum Asset Size
==================
An asset is stored in a contiguous space in a logical filesystem block. The
maximum size of an asset can be up-to the size of the data block. Typically,
each logical block corresponds to one physical flash erase sector (the smallest
unit that can erased), but the ``TFM_HAL_ITS_SECTORS_PER_BLOCK`` configuration
below allows a number of contiguous erase sectors to form one logical block.

Internal Trusted Storage Service HAL
====================================
The ITS service requires the platform to implement the ITS HAL, defined in
``platform/include/tfm_hal_its.h``.

The following C definitions in the HAL are mandatory, and must be defined by the
platform in a header named ``flash_layout.h``:

- ``TFM_HAL_ITS_FLASH_DRIVER`` - Defines the identifier of the CMSIS Flash
  ARM_DRIVER_FLASH object to use for ITS. It must have been allocated by the
  platform and will be declared extern in the HAL header.

- ``TFM_HAL_ITS_PROGRAM_UNIT`` - Defines the size of the ITS flash device's
  physical program unit (the smallest unit of data that can be individually
  programmed to flash). It must be equal to
  ``TFM_HAL_ITS_FLASH_DRIVER.GetInfo()->program_unit``, but made available at
  compile time so that filesystem structures can be statically sized. Valid
  values are powers of two between 1 and the flash sector size, inclusive.

The following C definitions in the HAL may optionally be defined by the platform
in the ``flash_layout.h`` header:

- ``TFM_HAL_ITS_FLASH_AREA_ADDR`` - Defines the base address of the dedicated
  flash area for ITS.

- ``TFM_HAL_ITS_FLASH_AREA_SIZE`` - Defines the size of the dedicated flash area
  for ITS in bytes.

- ``TFM_HAL_ITS_SECTORS_PER_BLOCK`` - Defines the number of contiguous physical
  flash erase sectors that form a logical erase block in the filesystem. The
  typical value is ``1``, but it may be increased so that the maximum required
  asset size will fit in one logical block.

If any of the above definitions are not provided by the platform, then the
``tfm_hal_its_fs_info()`` HAL API must be implemented instead. This function is
documented in ``tfm_hal_its.h``.

The sectors reserved to be used for Internal Trusted Storage **must** be
contiguous.

Internal Trusted Storage Service Optional Platform Definitions
==============================================================
The following optional platform definitions may be defined in
``flash_layout.h``:

- ``ITS_RAM_FS_SIZE`` - Defines the size of the RAM FS buffer when using the
  RAM FS emulated flash implementation. The buffer must be at least as large as
  the area earmarked for the filesystem by the HAL.
- ``ITS_FLASH_NAND_BUF_SIZE`` - Defines the size of the write buffer when using
  the NAND flash implementation. The buffer must be at least as large as a
  logical filesystem block.
- ``ITS_MAX_BLOCK_DATA_COPY`` - Defines the buffer size used when copying data
  between blocks, in bytes. If not provided, defaults to 256. Increasing this
  value will increase the memory footprint of the service.

More information about the ``flash_layout.h`` content, not ITS related, is
available in :doc:`platform readme </platform/ext/readme>` along with other
platform information.

ITS Service Build Definitions
=============================
The ITS service uses a set of C definitions to compile in/out certain features,
as well as to configure certain service parameters. When using the TF-M build
system, these definitions are controlled by build flags of the same name. The
``config/config_default.cmake`` file sets the default values of those flags, but
they can be overwritten based on platform capabilities by setting them in
``platform/ext/target/<TARGET_NAME>/config.cmake``. The list of ITS service
build definitions is:

- ``ITS_CREATE_FLASH_LAYOUT``- this flag indicates that it is required
  to create an ITS flash layout. If this flag is set, ITS service will
  generate an empty and valid ITS flash layout to store assets. It will
  erase all data located in the assigned ITS memory area before generating
  the ITS layout. This flag is required to be set if the ITS memory area
  is located in a non-persistent memory. This flag can be set if the ITS
  memory area is located in a persistent memory without a valid ITS flash
  layout in it. That is the case when it is the first time in the device
  life that the ITS service is executed.
- ``ITS_VALIDATE_METADATA_FROM_FLASH``- this flag allows to
  enable/disable the validation mechanism to check the metadata store in flash
  every time the flash data is read from flash. This validation is required
  if the flash is not hardware protected against data corruption.
- ``ITS_RAM_FS``- setting this flag to ``ON`` enables the use of RAM instead of
  the persistent storage device to store the FS in the Internal Trusted Storage
  service. This flag is ``OFF`` by default. The ITS regression tests write/erase
  storage multiple time, so enabling this flag can increase the life of flash
  memory when testing.
  If this flag is set to ``ON``, ITS_RAM_FS_SIZE must also be provided. This
  specifies the size of the block of RAM to be used to simulate the flash.

  .. Note::
    If this flag is disabled when running the regression tests, then it is
    recommended that the persistent storage area is erased before running the
    tests to ensure that all tests can run to completion. The type of persistent
    storage area is platform specific (eFlash, MRAM, etc.) and it is described
    in corresponding flash_layout.h

- ``ITS_MAX_ASSET_SIZE`` - Defines the maximum asset size to be stored in the
  ITS area. This size is used to define the temporary buffers used by ITS to
  read/write the asset content from/to flash. The memory used by the temporary
  buffers is allocated statically as ITS does not use dynamic memory allocation.
- ``ITS_NUM_ASSETS`` - Defines the maximum number of assets to be stored in the
  ITS area. This number is used to dimension statically the filesystem metadata
  tables in RAM (fast access) and flash (persistent storage). The memory used by
  the filesystem metadata tables is allocated statically as ITS does not use
  dynamic memory allocation.
- ``ITS_BUF_SIZE``- Defines the size of the partition's internal data transfer
  buffer. If not provided, then ``ITS_MAX_ASSET_SIZE`` is used to allow asset
  data to be copied between the client and the filesystem in one iteration.
  Reducing the buffer size will decrease the RAM usage of the partition at the
  expense of latency, as data will be copied in multiple iterations. *Note:*
  when data is copied in multiple iterations, the atomicity property of the
  filesystem is lost in the case of an asynchronous power failure.

--------------

*Copyright (c) 2019-2021, Arm Limited. All rights reserved.*
*Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.*
