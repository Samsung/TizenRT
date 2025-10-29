########################
BL1 Immutable bootloader
########################

:Author: Raef Coles
:Organization: Arm Limited
:Contact: raef.coles@arm.com

************
Introduction
************

Some devices that use TF-M will require initial boot code that is stored in ROM.
There are a variety of reasons that this might happen:

- The device cannot access flash memory without a driver, so needs some setup
  to be done before main images on flash can be booted.
- The device has no on-chip secure flash, and therefore cannot otherwise
  maintain a tamper-resistant root of trust.
- The device has a security model that requires an immutable root of trust

Henceforth any bootloader stored in ROM will be referred to as BL1, as it would
necessarily be the first stage in the boot chain.

TF-M provides a reference second-stage flash bootloader BL2, in order to allow
easier integration. This bootloader implements all secure boot functionality
needed to provide a secure chain of trust.

A reference ROM bootloader BL1 has now being added with the same motivation -
allowing easier integration of TF-M for platforms that do not have their own
BL1 and require one.

****************************
BL1 Features and Motivations
****************************

The reference ROM bootloader provides the following features:

- A split between code being stored in ROM and in other non-volatile memory.

  - This can allow significant cost reduction in fixing bugs compared to
    ROM-only bootloaders.

- A secure boot mechanism that allows upgrading the next boot stage (which
  would usually be BL2).

  - This allows for the fixing of any bugs in the BL2 image.
  - Alternately, this could allow the removal of BL2 in some devices that are
    constrained in flash space but have ROM.

- A post-quantum resistant asymmetric signature scheme for verifying the next
  boot stage image.

  - This can allow devices to be securely updated even if attacks
    involving quantum computers become viable. This could extend the lifespans
    of devices that might be deployed in the field for many years.

- A mechanism for passing boot measurements to the TF-M runtime so that they
  can be attested.
- Tooling to create and sign images.
- Fault Injection (FI) and Differential Power Analysis (DPA) mitigations.

*********************************
BL1_1 and BL1_2 split bootloaders
*********************************

BL1 is split into two distinct boot stages, BL1_1 which is stored in ROM and
BL1_2 which is stored in other non-volatile storage. This would usually be
either trusted or untrusted flash, but on platforms without flash memory can be
OTP. As BL1_2 is verified against a hash stored in OTP, it is immutable after
provisioning even if stored in mutable storage.

Bugs in ROM bootloaders usually cannot be fixed once a device is provisioned /
in the field, as ROM code is immutable the only option is fixing the bug in
newly manufactured devices.

However, it can be very expensive to change the ROM code of devices once
manufacturing has begun, as it requires changes to the photolithography masks
that are used to create the device. This cost varies depending on the complexity
of the device and of the process node that it is being fabricated on, but can be
large, both in engineering time and material/process costs.

By placing the majority of the immutable bootloader in other storage, we can
mitigate the costs associated with changing ROM code, as a new BL1_2 image can
be used at provisioning time with minimal changeover cost. BL1_1 contains a
minimal codebase responsible mainly for the verification of the BL1_2 image.

The bootflow is as follows. For simplicity this assumes that the boot stage
after BL1 is BL2, though this is not necessarily the case:

1) BL1_1 begins executing in place from ROM
2) BL1_1 copies BL1_2 into RAM
3) BL1_1 verifies BL1_2 against the hash stored in OTP
4) BL1_1 jumps to BL1_2, if the hash verification has succeeded
5) BL1_2 copies the primary BL2 image from flash into RAM
6) BL1_2 verifies the BL2 image using asymmetric cryptography
7) If verification fails, BL1_2 repeats 5 and 6 with the secondary BL2 image
8) BL1_2 jumps to BL2, if either image has successfully verified

.. Note::
    The BL1_2 image is not encrypted, so if it is placed in untrusted flash it
    will be possible to read the data in the image.

Some optimizations have been made specifically for the case where BL1_2 has been
stored in OTP:

OTP can be very expensive in terms of chip area, though new technologies like
antifuse OTP decrease this cost. Because of this, the code size of BL1_2 has
been minimized. Code-sharing has been configured so that BL1_2 can call
functions stored in ROM. Care should be taken that OTP is sized such that it is
possible to include versions of the functions used via code-sharing, in case the
ROM functions contain bugs, though less space is needed than if all code is
duplicated as it is assumed that most functions will not contain bugs.

As OTP memory frequently has low performance, BL1_2 is copied into RAM before it
it is executed. It also copies the next image stage into RAM before
authenticating it, which allows the next stage to be stored in untrusted flash.
This requires that the device have sufficient RAM to contain both the BL1_2
image and the next stage image at the same time. Note that this is done even if
BL1_2 is located in XIP-capable flash, as it both allows the use of untrusted
flash and simplifies the image upgrade logic.

.. Note::
   BL1_2 enables TF-M to be used on devices that contain no secure flash, though
   the ITS service will not be available. Other services that depend on ITS will
   not be available without modification.

*************************************
Secure boot / Image upgrade mechanism
*************************************

BL1_2 verifies the authenticity of the next stage image via asymmetric
cryptography, using a public key that is provisioned into OTP.

BL1_2 implements a rollback protection counter in OTP, which is used to prevent
the next stage image being downgraded to a less secure version.

BL1_2 has two image slots, which allows image upgrades to be performed. The
primary slot is always booted first, and then if verification of this fails
(either due to an invalid signature or due to a version lower than the rollback
protection counter) the secondary slot is then booted (subject to the same
checks).

BL1_2 contains no image upgrade logic, in order for OTA of the next stage image
to be implemented, a later stage in the system must handle downloading new
images and placing them in the required slot.

********************************************
Post-Quantum signature verification in BL1_2
********************************************

BL1_2 uses a post-quantum asymmetric signature scheme to verify the next stage.
The scheme used is Leighton-Michaeli Signatures (henceforth LMS). LMS is
standardised in `NIST SP800-208
<https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-208.pdf>`_
and `IETF RFC8554. <https://datatracker.ietf.org/doc/html/rfc8554>`_

LMS is a stateful-hash signature scheme, meaning that:

 1) It is constructed from a cryptographic hash function, in this case SHA256.

    - This function can be accelerated by existing hardware accelerators, which
      can make LMS verification relatively fast compared to other post-quantum
      signature schemes that cannot be accelerated in hardware yet.

 2) Each private key can only be used to sign a certain number of images.

    - BL1_2 uses the SHA256_H10 parameter set, meaning each key can sign 1024
      images.

The main downside, the limited amount of possible signatures, can be mitigated
by limiting the amount of image upgrades that are done. As BL2 is often
currently not upgradable, it is not anticipated that this limit will be
problematic. If BL1 is being used to directly boot a TF-M/NS combined image, the
limit is more likely to be problematic, and care should be taken to examine the
likely update amount.

LMS public keys are 32 bytes in size, and LMS signatures are 1912 bytes in size.
The signature size is larger than some asymmetric schemes, though most devices
should have enough space in flash to accommodate this.

The main upside of LMS, aside from the security against attacks involving
quantum computers, is that it is relatively simple to implement. The software
implementation that is used by BL1 is ~3KiB in size, which is considerably
smaller than the corresponding RSA implementation which is at least 6.5K. This
simplicity of implementation is useful to avoid bugs.

BL1 will use MbedTLS as the source for its implementation of LMS.

.. Note::
   As of the time of writing, the LMS code is still in the process of being
   merged into MbedTLS, so BL1 currently does not support asymmetric
   verification of the next boot stage. Currently, the next boot stage is
   hash-locked, so cannot be upgraded.

   The Github pull request for LMS can be found `here
   <https://github.com/ARMmbed/mbedtls/pull/4826>`_

*********************
BL1 boot measurements
*********************

BL1 outputs boot measurements in the same format as BL2, utilising the same
shared memory area. These measurements can then be included in the attestation
token, allowing the attestation of the version of the boot stage after BL1.

***********
BL1 tooling
***********

Image signing scripts are provided for BL1_1 and BL1_2. While the script is
named ``create_bl2_img.py``, it can be used for any next stage image.

- ``bl1/bl1_1/scripts/create_bl1_2_img.py``
- ``bl1/bl1_2/scripts/create_bl2_img.py``

These sign (and encrypt in the case of ``create_bl2_img.py``) a given image file
and append the required headers.

**************************
BL1 FI and DPA mitigations
**************************

BL1 reuses the FI countermeasures used in the TF-M runtime, which are found in
``lib/fih/``.

BL1 implements countermeasures against DPA, which are primarily targeted
towards being able to handle cryptographic material without leaking its
contents. The functions with these countermeasures are found in
``bl1/bl1_1/shared_lib/util.c``

``bl_secure_memeql`` tests if memory regions have the same value

- It does not perform early exits to prevent timing attacks.
- It compares chunks in random orders to prevent DPA trace correlation analysis
- It inserts random delays to prevent DPA trace correlation analysis
- It performs loop integrity checks
- It uses FIH constructs

``bl_secure_memcpy`` copies memory regions

- It copies chunks in random orders to prevent DPA trace correlation analysis
- It inserts random delays to prevent DPA trace correlation analysis
- It performs loop integrity checks
- It uses FIH constructs

**************************
Using BL1 on new platforms
**************************

New platforms must define the following macros in their ``region_defs.h``:

- ``BL1_1_HEAP_SIZE``
- ``BL1_1_STACK_SIZE``
- ``BL1_2_HEAP_SIZE``
- ``BL1_2_STACK_SIZE``
- ``BL1_1_CODE_START``
- ``BL1_1_CODE_LIMIT``
- ``BL1_1_CODE_SIZE``
- ``BL1_2_CODE_START``
- ``BL1_2_CODE_LIMIT``
- ``BL1_2_CODE_SIZE``
- ``PROVISIONING_DATA_START``
- ``PROVISIONING_DATA_LIMIT``
- ``PROVISIONING_DATA_SIZE``

The ``PROVISIONING_DATA_*`` defines are used to locate where the data to be
provisioned into OTP can be found. These are required as the provisioning bundle
needs to contain the entire BL1_2 image, usually >= 8KiB in size, which is too
large to be placed in the static data area as is done for all other dummy
provisioning data. On development platforms with reprogrammable ROM, this is
often placed in unused ROM. On production platforms, this should be located in
RAM and then filled with provisioning data. The format of the provisioning data
that should be located in the ``PROVISIONING_DATA_*`` region can be found in
``bl1/bl1_1/lib/provisioning.c`` in the struct
``bl1_assembly_and_test_provisioning_data_t``

If the platform is storing BL1_2 in flash, it must set
``BL1_2_IMAGE_FLASH_OFFSET`` to the flash offset of the start of BL1_2.

The platform must also implement the HAL functions defined in the following
headers:

- ``bl1/bl1_1/shared_lib/interface/trng.h``
- ``bl1/bl1_1/shared_lib/interface/crypto.h``
- ``bl1/bl1_1/shared_lib/interface/otp.h``

If the platform integrates a CryptoCell-312, then it can reuse the existing
implementation.

***********
BL1 Testing
***********

New tests have been written to test both the HAL implementation, and the
integration of those functions for verifying images. These tests are stored in
the ``tf-m-tests`` repository, under the ``test/bl1/`` directory, and further
subdivided into BL1_1 and BL1_2 tests.

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
