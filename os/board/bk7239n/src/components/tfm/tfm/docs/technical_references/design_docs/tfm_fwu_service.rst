#######################
Firmware Update Service
#######################

:Author: Sherry Zhang
:Organization: Arm Limited
:Contact: Sherry Zhang <Sherry.Zhang2@arm.com>

.. contents:: Table of Contents

***************************************
Introduction of Firmware Update service
***************************************
The Firmware Update(FWU) service provides the functionality of updating firmware
images. It provides a standard interface for updating firmware and it is
platform independent. TF-M defines a shim layer to support cooperation between
bootloader and FWU service.

This partition supports the following features:

- Query image information
  Fetch information about the firmware images on the device. This covers the
  images in the running area and also the staging area.
- Store image
  Write a candidate image to its staging area.
- Validate image
  Starts the validation of the image.
- Trigger reboot
  Trigger a reboot to restart the platform.

**********
Components
**********
The structure of the TF-M Firmware Update service is listed below:
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | **Component name**          | **Description**                                               | **Location**                                                                          |
   +=============================+===============================================================+=======================================================================================+
   | SPE client API interface    | This module exports the client API of PSA Firmware Update to  | ``./secure_fw/partitions/firmware_update/tfm_fwu_secure_api.c``                       |
   |                             | the other services available in TF-M.                         |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | NSPE client API interface   | This module exports the client API of PSA Firmware Update to  | ``./interface/src/tfm_firmware_update_func_api.c``                                    |
   |                             | the NSPE(i.e. to the applications).                           | ``./interface/src/tfm_firmware_update_ipc_api.c``                                     |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | Manifest                    | The manifest file is a description of the service components  | ``./secure_fw/partitions/firmware_update/tfm_firmware_update.yaml``                   |
   |                             | for both library mode and IPC mode.                           |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | Secure functions and IPC    | This module handles all the secure function requests in       | ``./secure_fw/partitions/firmware_update/tfm_fwu_req_mngr.c``                         |
   | request handlers            | library model and all the service requests in IPC model.      |                                                                                       |
   |                             | It maitains the image state context and calls the image ID    |                                                                                       |
   |                             | converter to achieve the firmware update functionalities.     |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | Image ID Converter          | This module converts the image ID between psa_image_id_t,     | ``./secure_fw/partitions/firmware_update/tfm_fwu_internal.c``                         |
   |                             | which is the image ID structure in user interfaces, and       |                                                                                       |
   |                             | bl_image_id_t which is the image ID structure in bootloader.  |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | Shim layer between FWU and  | This module provides the APIs with the functionality of       | ``./secure_fw/partitions/firmware_update/bootloader/tfm_bootloader_fwu_abstraction.h``|
   | bootloader                  | operating the bootloader to cooperate with the Firmware Update|                                                                                       |
   |                             | service                                                       |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+
   | Shim layer example based on | This module is the implementation of the shim layer between   | ``./secure_fw/partitions/firmware_update/bootloader/mcuboot/tfm_mcuboot_fwu.c``       |
   | MCUboot                     | FWU and bootloader based on MCUboot.                          |                                                                                       |
   |                             |                                                               |                                                                                       |
   +-----------------------------+---------------------------------------------------------------+---------------------------------------------------------------------------------------+

***********************
Service API description
***********************
This service follows the PSA Firmware Update API spec of version 0.7 [1]_.
It implements the mandatory interface functions listed in section 5.1 and the
optional interface ``psa_fwu_accept()``. Please refer to Firmware Update spec
for the detailed description.

*************************************
Shim Layer between FWU and bootloader
*************************************
The firmware update operations are achieved by calling the shim layer APIs
between bootloader and FWU.

Shim layer introduction
=======================
This shim layer provides the APIs with the functionality of operating the
bootloader to cooperate with the Firmware Update service. This shim layer
is decoupled from bootloader implementation. Users can specify a specific
bootloader by setting ``TFM_FWU_BOOTLOADER_LIB`` build configuration and
adding the specific build scripts into that file. By default, the MCUboot
is chosen as the bootloader.

Interfaces of the shim Layer
============================

fwu_bootloader_init(function)
-----------------------------
Prototype
^^^^^^^^^
.. code-block:: c

    psa_status_t fwu_bootloader_init(void);

Description
^^^^^^^^^^^
Bootloader related initialization for the firmware update, such as reading
some necessary shared data from the memory if needed.

Parameters
^^^^^^^^^^
    N/A

fwu_bootloader_staging_area_init(function)
------------------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_staging_area_init(bl_image_id_t bootloader_image_id);

**Description**

Prepare the staging area of the image with the given ID for image download.
For example, initialize the staging area, open the flash area, and so on.
The image will be written into the staging area later.

**Parameters**

- ``bootloader_image_id``: The identifier of the target image in bootloader.

fwu_bootloader_load_image(function)
-----------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_load_image(bl_image_id_t bootloader_image_id,
                                           size_t        image_offset,
                                           const void    *block,
                                           size_t        block_size);

**Description**

Load the image to its staging area.

**Parameters**

- ``bootloader_image_id``: The identifier of the target image in bootloader.
- ``image_offset``: The offset of the image being passed into block, in bytes.
- ``block``: A buffer containing a block of image data. This might be a complete image or a subset.
- ``block_size``: Size of block.

fwu_bootloader_install_image(function)
---------------------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_install_image(bl_image_id_t bootloader_image_id,
                                              bl_image_id_t       *dependency,
                                              psa_image_version_t *dependency_version);

**Description**

Check the authenticity and integrity of the image. If a reboot is required to
complete the check, then mark this image as a candidate so that the next time
bootloader runs it will take this image as a candidate one to bootup. Return
the error code PSA_SUCCESS_REBOOT.

**Parameters**

- ``bootloader_image_id``: The identifier of the target image in bootloader.
- ``dependency``: Bootloader image ID of dependency if needed.
- ``dependency_version``: Bootloader image version of dependency if needed.

fwu_bootloader_mark_image_accepted(function)
--------------------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_mark_image_accepted(bl_image_id_t bootloader_image_id);

**Description**

Call this API to mark the running images as permanent/accepted to avoid
revert when next time bootup. Usually, this API is called after the running
images have been verified as valid.

**Parameters**

    N/A

fwu_bootloader_abort(function)
------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_abort(void);

**Description**

Abort the current image download process.

**Parameters**

    N/A

fwu_bootloader_get_image_info(function)
---------------------------------------
**Prototype**

.. code-block:: c

    psa_status_t fwu_bootloader_get_image_info(bl_image_id_t    bootloader_image_id,
                                               bool             staging_area,
                                               tfm_image_info_t *info);

**Description**

Get the image information of the given bootloader_image_id in the staging area
or the running area.

**Parameters**

    - ``bootloader_image_id``: The identifier of the target image in bootloader.
    - ``active_image``: Indicates image location.

        - ``True``: the running image.
        - ``False``: the image in the passive(or staging) slot.

    - ``info``: Buffer containing the image information.

******************************************
Additional shared data between BL2 and SPE
******************************************
An additional TLV area "image version" is added into the shared memory between
BL2 and TF-M. So that the firmware update partition can get the image version.
Even though the image version information is also included in the ``BOOT RECORD``
TLV area which is encoded by CBOR, adding a dedicated ``image version`` TLV area
is preferred to avoid involving the CBOR encoder which can increase the code
size. The FWU partition will read the shared data at the partition
initialization.

******************
Image ID structure
******************
The structure of image ID is:
    image_id[7:0]: slot.
    image_id[15:8]: image type.
    image_id[31:16]: specific image ID.

Three image types are defined in this partition.
- FWU_IMAGE_TYPE_NONSECURE: the non_secure image
- FWU_IMAGE_TYPE_SECURE: the secure image
- FWU_IMAGE_TYPE_FULL: the secure + non_secure image

Macros **FWU_CALCULATE_IMAGE_ID**, **FWU_IMAGE_ID_GET_TYPE** and
**FWU_IMAGE_ID_GET_SLOT** are dedicated to converting the image id, type, and
slot. The service users can call these macros to get the image ID.

.. Note::

    The image ID structure, as well as the macros listed here, is TF-M specific implementation.

***********************************
Benefits Analysis on this Partition
***********************************

Implement the FWU functionality in the non-secure side
======================================================
The APIs listed in PSA Firmware Update API spec [1]_ can also be implemented in
the non-secure side. The library model implementation can be referred to for the
non-secure side implementation.

Pros and Cons for Implementing FWU APIs in Secure Side
======================================================

Pros
----
- It protects the image in the passive or staging area from being tampered with
  by the NSPE. Otherwise, a malicious actor from NSPE can tamper the image
  stored in the non-secure area to break image update.

- It protects secure image information from disclosure. In some cases, the
  non-secure side shall not be permitted to get secure image information.

- It protects the active image from being manipulated by NSPE. Some bootloader
  supports testing the image. After the image is successfully installed and
  starts to run, the user should set the image as permanent image if the image
  passes the test. To achieve this, the area of the active image needs to be
  accessed. In this case, implementing FWU service in SPE can prevent NSPE
  from manipulating the active image area.

- On some devices, such as the Arm Musca-B1 board, the passive or staging area
  is restricted as secure access only. In this case, the FWU partition should
  be implemented in the secure side.

Cons
----
- It increases the image size of the secure image.
- It increases the execution latency and footprint. Compared to implementing
  FWU in NSPE directly, calling the Firmware Update APIs which are implemented
  in the secure side increases the execution latency and footprint.
- It can increase the attack surface of the secure runtime.

Users can decide whether to call the FWU service in TF-M directly or implement
the Firmware Update APIs in the non-secure side based on the pros and cons
analysis above.

*********
Reference
*********

.. [1] `PSA Firwmare Update API <https://developer.arm.com/documentation/ihi0093/0000/>`_

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
