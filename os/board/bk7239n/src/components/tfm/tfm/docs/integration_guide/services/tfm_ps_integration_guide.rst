###########################################
Protected Storage Service Integration Guide
###########################################

************
Introduction
************
TF-M Protected Storage (PS) service implements PSA Protected Storage APIs.

The service is usually backed by hardware isolation of the flash
access domain and, in the current version, relies on hardware to
isolate the flash area from non-secure access. In absence of hardware
isolation, the secrecy and integrity of data is still maintained.

The PS service implements an AES-GCM based AEAD encryption policy, as a
reference, to protect data integrity and authenticity.

The PS reuses the non-hierarchical filesystem provided by the TF-M
Internal Trusted Storage service to store encrypted, authenticated
objects.

The design addresses the following high level requirements as well:

- **Confidentiality** - Resistance to unauthorised accesses through
  hardware/software attacks.

- **Access Authentication** - Mechanism to establish requester's identity (a
  non-secure entity, secure entity, or a remote server).

- **Integrity** - Resistant to tampering by either the normal users of a product,
  package, or system or others with physical access to it. If the content of the
  protected storage is changed maliciously, the service is able to detect it.

- **Reliability** - Resistant to power failure scenarios and incomplete write
  cycles.

- **Configurability** - High level configurability to scale up/down memory
  footprint to cater for a variety of devices with varying security
  requirements.

- **Performance** - Optimized to be used for resource constrained devices with
  very small silicon footprint, the PPA (power, performance, area) should be
  optimal.

******************************
Current PS Service Limitations
******************************
- **Asset size limitation** - An asset is stored in a contiguous space in a
  block/sector. Hence, the maximum asset size can be up-to the size of the
  data block/sector. Detailed information about the maximum asset size can be
  found in the section `Maximum asset size` below.

- **Fragmentation** - The current design does not support fragmentation, as an
  asset is stored in a contiguous space in a block.
  Each block can potentially store multiple assets.
  A delete operation implicitly moves all the assets towards the top of the block
  to avoid fragmentation within block. However, this may also result in
  unutilized space at the end of each block.

- **Non-hierarchical storage model** - The current design uses a
  non-hierarchical storage model, as a filesystem, where all the assets are
  managed by a linearly indexed list of metadata. This model locates the
  metadata in blocks which are always stored in the same flash location. That
  increases the number of writes in a specific flash location as every change in
  the storage area requires a metadata update.

- **PSA internal trusted storage API** - In the current design, the service does
  not use the PSA Internal Trusted Storage API to write the rollback protection
  values stored in the internal storage.

- **Protection against physical storage medium failure** - Complete handling of
  inherent failures of storage mediums (e.g. bad blocks in a NAND based device)
  is not supported by the current design.

- **Key diversification** - In a more robust design, each asset would be
  encrypted through a different key.

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
Protected storage service code is located in
``secure_fw/partitions/protected_storage/`` and is divided as follows:

    - Core files
    - Cryptographic interfaces
    - Non-volatile (NV) counters interfaces

The PSA PS interfaces for PS service are located in ``interface/include/psa``

PSA Protected Storage Interfaces
================================

The PS service exposes the following mandatory PSA PS interfaces, version 1.0:

.. code-block:: c

    psa_status_t psa_ps_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags);
    psa_status_t psa_ps_get(psa_storage_uid_t uid, size_t data_offset, size_t data_size, void *p_data, size_t *p_data_length);
    psa_status_t psa_ps_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info);
    psa_status_t psa_ps_remove(psa_storage_uid_t uid);
    uint32_t psa_ps_get_support(void);

For the moment, it does not support the extended version of those APIs.

These PSA PS interfaces and PS TF-M types are defined and documented in
``interface/include/psa/protected_storage.h``,
``interface/include/psa/storage_common.h`` and
``interface/include/tfm_ps_defs.h``

Core Files
==========
- ``tfm_ps_req_mngr.c`` - Contains the PS request manager implementation which
  handles all requests which arrive to the service. This layer extracts the
  arguments from the input and output vectors, and it calls the protected
  storage layer with the provided parameters.

- ``tfm_protected_storage.c`` - Contains the TF-M protected storage API
  implementations which are the entry points to the PS service.

- ``ps_object_system.c`` - Contains the object system implementation to manage
  all objects in PS area.

- ``ps_object_table.c`` - Contains the object system table implementation which
  complements the object system to manage all object in the PS area.
  The object table has an entry for each object stored in the object system
  and keeps track of its version and owner.

- ``ps_encrypted_object.c`` - Contains an implementation to manipulate
  encrypted objects in the PS object system.

- ``ps_utils.c`` - Contains common and basic functionalities used across the
  PS service code.

Flash Filesystem and Flash Interfaces
=====================================
The PS service reuses the non-hierarchical filesystem and flash
interfaces provided by the TF-M Internal Trusted Storage service. It
stores encrypted, authenticated objects by making service calls to the
ITS service. When the ITS service receives requests from the PS
partition, it handles the request by using a separate filesystem
context.

The ITS filesystem and flash interfaces and their implementation can be found in
``secure_fw/partitions/internal_trusted_storage/flash_fs`` and
``secure_fw/partitions/internal_trusted_storage/flash`` respectively. More
information about the filesystem and flash interfaces can be found in the
:doc:`ITS integration guide
</integration_guide/services/tfm_its_integration_guide>`.

The ITS service implementation in
``secure_fw/partitions/internal_trusted_storage/tfm_internal_trusted_storage.c``,
constructs a filesystem configuration for Protected Storage based on
target-specific definitions from the Protected Storage HAL. Please see the
`Protected Storage Service HAL` section for details of these.

Cryptographic Interface
=======================
- ``crypto/ps_crypto_interface.h`` - Abstracts the cryptographic operations for
  the protected storage service.

- ``crypto/ps_crypto_interface.c`` - Implements the PS service cryptographic
  operations with calls to the TF-M Crypto service.

Non-volatile (NV) Counters Interface
====================================
The current PS service provides rollback protection based on NV
counters.
PS defines and implements the following NV counters functionalities:

- ``nv_counters/ps_nv_counters.h`` - Abstracts PS non-volatile
  counters operations. This API detaches the use of NV counters from the TF-M NV
  counters implementation, provided by the platform, and provides a mechanism to
  compile in a different API implementation for test purposes. A PS test suite
  **may** provide its own custom implementation to be able to test different PS
  service use cases.

- ``nv_counters/ps_nv_counters.c`` - Implements the PS NV counters interfaces
  based on TF-M NV counters implementation provided by the platform.

****************************
PS Service Integration Guide
****************************
This section describes mandatory (i.e. **must** implement) or optional
(i.e. **may** implement) interfaces which the system integrator have to take
in to account in order to integrate the protected storage service in a new
platform.

Maximum Asset Size
==================
An asset is stored in a contiguous space in a block/sector. The maximum
size of an asset can be up-to the size of the data block/sector minus the object
header size (``PS_OBJECT_HEADER_SIZE``) which is defined in
``ps_object_defs.h``. The ``PS_OBJECT_HEADER_SIZE`` changes based on the
``PS_ENCRYPTION`` flag status.

Protected Storage Service HAL
=============================
The PS service requires the platform to implement the PS HAL, defined in
``platform/include/tfm_hal_ps.h``.

The following C definitions in the HAL are mandatory, and must be defined by the
platform in a header named ``flash_layout.h``:

- ``TFM_HAL_PS_FLASH_DRIVER`` - Defines the identifier of the CMSIS Flash
  ARM_DRIVER_FLASH object to use for PS. It must have been allocated by the
  platform and will be declared extern in the HAL header.

- ``TFM_HAL_PS_PROGRAM_UNIT`` - Defines the size of the PS flash device's
  physical program unit (the smallest unit of data that can be individually
  programmed to flash). It must be equal to
  ``TFM_HAL_PS_FLASH_DRIVER.GetInfo()->program_unit``, but made available at
  compile time so that filesystem structures can be statically sized. Valid
  values are powers of two between 1 and the flash sector size, inclusive.

The following C definitions in the HAL may optionally be defined by the platform
in the ``flash_layout.h`` header:

- ``TFM_HAL_PS_FLASH_AREA_ADDR`` - Defines the base address of the dedicated
  flash area for PS.

- ``TFM_HAL_PS_FLASH_AREA_SIZE`` - Defines the size of the dedicated flash area
  for PS in bytes.

- ``TFM_HAL_PS_SECTORS_PER_BLOCK`` - Defines the number of contiguous physical
  flash erase sectors that form a logical erase block in the filesystem. The
  typical value is ``1``, but it may be increased so that the maximum required
  asset size will fit in one logical block.

If any of the above definitions are not provided by the platform, then the
``tfm_hal_ps_fs_info()`` HAL API must be implemented instead. This function is
documented in ``tfm_hal_ps.h``.

The sectors reserved to be used for Protected Storage **must** be contiguous
sectors starting at ``TFM_HAL_PS_FLASH_AREA_ADDR``.

The design requires either 2 blocks, or any number of blocks greater than or
equal to 4. Total number of blocks can not be 0, 1 or 3. This is a design choice
limitation to provide power failure safe update operations.

Protected Storage Service Optional Platform Definitions
=======================================================
The following optional platform definitions may be defined in
``flash_layout.h``:

- ``PS_RAM_FS_SIZE`` - Defines the size of the RAM FS buffer when using the
  RAM FS emulated flash implementation. The buffer must be at least as large as
  the area earmarked for the filesystem by the HAL.
- ``PS_FLASH_NAND_BUF_SIZE`` - Defines the size of the write buffer when using
  the NAND flash implementation. The buffer must be at least as large as a
  logical filesystem block.

More information about the ``flash_layout.h`` content, not PS related, is
available in :doc:`platform readme </platform/ext/readme>` along with other
platform information.

TF-M NV Counter Interface
=========================
To have a platform independent way to access the NV counters, TF-M defines a
platform NV counter interface. For API specification, please check:
``platform/include/tfm_plat_nv_counters.h``

The system integrators **may** implement this interface based on the target
capabilities and set the ``PS_ROLLBACK_PROTECTION`` flag to compile in
the rollback protection code.

Secret Platform Unique Key
==========================
The encryption policy relies on a secret hardware unique key (HUK) per device.
It is system integrator's responsibility to provide an implementation which
**must** be a non-mutable target implementation.
For API specification, please check:
``platform/include/tfm_plat_crypto_keys.h``

A stub implementation is provided in
``platform/ext/common/template/crypto_keys.c``

Non-Secure Identity Manager
===========================
TF-M core tracks the current client IDs running in the secure or non-secure
processing environment. It provides a dedicated API to retrieve the client ID
which performs the service request.

:doc:`Non-secure Client Extension Integration Guide </integration_guide/non-secure_client_extension_integration_guide>`
provides further details on how client identification works.

PS service uses that TF-M core API to retrieve the client ID and associate it
as the owner of an asset. Only the owner can read, write or delete that asset
based on the creation flags.

The :doc:`integration guide </integration_guide/index>`
provides further details of non-secure implementation requirements for TF-M.

Cryptographic Interface
=======================
The reference encryption policy is built on AES-GCM, and it **may** be replaced
by a vendor specific implementation.

The PS service abstracts all the cryptographic requirements and specifies the
required cryptographic interface in
``secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.h``

The PS service cryptographic operations are implemented in
``secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.c``, using
calls to the TF-M Crypto service.

PS Service Build Definitions
============================
The PS service uses a set of C definitions to compile in/out certain features,
as well as to configure certain service parameters. When using the TF-M build
system, these definitions are controlled by build flags of the same name. The
``config/config_default.cmake`` file sets the default values of those flags, but
they can be overwritten based on platform capabilities by setting them in
``platform/ext/target/<TARGET_NAME>/config.cmake``. The list of PS service build
definitions is:

- ``PS_ENCRYPTION``- this flag allows to enable/disable encryption
  option to encrypt the protected storage data.
- ``PS_CREATE_FLASH_LAYOUT``- this flag indicates that it is required
  to create a PS flash layout. If this flag is set, PS service will
  generate an empty and valid PS flash layout to store assets. It will
  erase all data located in the assigned PS memory area before generating
  the PS layout.  This flag is required to be set if the PS memory area
  is located in a non-persistent memory.  This flag can be set if the PS
  memory area is located in a persistent memory without a valid PS flash
  layout in it. That is the case when it is the first time in the device
  life that the PS service is executed.
- ``PS_VALIDATE_METADATA_FROM_FLASH``- this flag allows to
  enable/disable the validation mechanism to check the metadata store in flash
  every time the flash data is read from flash. This validation is required
  if the flash is not hardware protected against malicious writes. In case
  the flash is protected against malicious writes (i.e embedded flash, etc),
  this validation can be disabled in order to reduce the validation overhead.
- ``PS_ROLLBACK_PROTECTION``- this flag allows to enable/disable
  rollback protection in protected storage service. This flag takes effect only
  if the target has non-volatile counters and ``PS_ENCRYPTION`` flag is on.
- ``PS_RAM_FS``- setting this flag to ``ON`` enables the use of RAM instead of
  the persistent storage device to store the FS in the Protected Storage
  service. This flag is ``OFF`` by default. The PS regression tests write/erase
  storage multiple time, so enabling this flag can increase the life of flash
  memory when testing.
  If this flag is set to ``ON``, PS_RAM_FS_SIZE must also be provided. This
  specifies the size of the block of RAM to be used to simulate the flash.

  .. Note::
    If this flag is disabled when running the regression tests, then it is
    recommended that the persistent storage area is erased before running the
    tests to ensure that all tests can run to completion. The type of persistent
    storage area is platform specific (eFlash, MRAM, etc.) and it is described
    in corresponding flash_layout.h

- ``PS_MAX_ASSET_SIZE`` - Defines the maximum asset size to be stored in the
  PS area. This size is used to define the temporary buffers used by PS to
  read/write the asset content from/to flash. The memory used by the temporary
  buffers is allocated statically as PS does not use dynamic memory allocation.
- ``PS_NUM_ASSETS`` - Defines the maximum number of assets to be stored in the
  PS area. This number is used to dimension statically the object table size in
  RAM (fast access) and flash (persistent storage). The memory used by the
  object table is allocated statically as PS does not use dynamic memory
  allocation.
- ``PS_TEST_NV_COUNTERS``- this flag enables the virtual implementation of the
  PS NV counters interface in ``test/secure_fw/suites/ps/secure/nv_counters`` of
  the ``tf-m-tests`` repo, which emulates NV counters in
  RAM, and disables the hardware implementation of NV counters provided by
  the secure service. This flag is enabled by default, but has no effect when
  the secure regression test is disabled. This flag can be
  overridden to ``OFF`` when building the regression tests. In this case,
  the PS rollback protection test suite will not be built, as it relies
  on extra functionality provided by the virtual NV counters to simulate
  different rollback scenarios. The remainder of the PS test suites will
  run using the hardware NV counters. Please note that running the tests in
  this configuration will quickly increase the hardware NV counter values,
  which cannot be decreased again.
  Overriding this flag from its default value of ``OFF`` when not
  building the regression tests is not currently supported.

--------------

*Copyright (c) 2018-2022, Arm Limited. All rights reserved.*
*Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.*
