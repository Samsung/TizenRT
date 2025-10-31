#######################################
Trusted Firmware-M Profile Small Design
#######################################

:Author: David Hu
:Organization: Arm Limited
:Contact: david.hu@arm.com

************
Introduction
************

The capabilities and resources may dramatically vary on different IoT devices.
Some IoT devices may have very limited memory resource. The program on those
devices should keep small memory footprint and basic functionalities.
On the other hand, some devices may consist of more memory and extended storage,
to support stronger software capabilities.

Diverse IoT use cases also require different levels of security and requirements
on device resource. For example, use cases require different cipher
capabilities. Selecting cipher suites can be sensitive to memory footprint on
devices with constrained resource.

Trusted Firmware-M (TF-M) defines 3 general profiles, Profile Small,
Profile Medium and Profile Large, to provide different levels of security to fit
diverse device capabilities and use cases.
Each profile specifies a predefined list of features, targeting typical use
cases with specific hardware constraints. Profiles can serve as reference
designs, based on which developers can continue further development and
configurations, according to use case.

As one of the TF-M Profiles, TF-M Profile Small (Profile S) consists of
lightweight TF-M framework and basic Secure Services to keep smallest memory
footprint, supporting fundamental security features on devices with ultra
constrained resource.

This profile enables connecting with Edge Gateways and IoT Cloud Services
supporting secure connection based solely on symmetric cryptography.

This document summarizes and discusses the features specified in TF-M Profile
Small.

**************
Overall design
**************

TF-M Profile Small defines the following features:

    - Lightweight framework

        - Library model or Secure Function (SFN) model [2]_
        - Level 1 isolation
        - Buffer sharing allowed
        - Single secure context

    - Crypto

        - Symmetric cipher only
        - Cipher suite for symmetric-key algorithms based protocols, such as
          cipher suites defined in TLS pre-shared key (TLS-PSK) [1]_.

            - Advanced Encryption Standard (AES) as symmetric crypto algorithm
            - SHA256 as Hash function
            - HMAC as Message Authentication Code algorithm

    - Internal Trusted Storage (ITS)

        - No encryption
        - No rollback protection
        - Decrease internal transient buffer size

    - Initial Attestation

        - Based on symmetric key algorithms

    - Lightweight boot

        - Single image boot
        - Anti-rollback protection is enabled


Protected Storage, audit logging and other Secure Services provided by TF-M are
disabled by default.

**************
Design details
**************

More details of TF-M Profile Small design are discussed in following sections.

Lightweight framework
=====================

TF-M framework model
--------------------

Library model is selected by default in Profile Small implementation.
Library model implements secure function calls, via which clients directly call
secure services. It provides a more simple implementation of TF-M framework and
may reduce memory footprint, compared with Inter-Process Communication (IPC)
model [3]_.

As Library model is TF-M specific implementation, please check some of its
dedicated implementation details as described in `Appendix`_, before adopting
Library model on your platforms.

You can select SFN model instead of Library model in Profile Small.
SFN model is defined in FF-M 1.1 extensions [2]_. It is a more simple
implementation of TF-M framework and may also reduce memory footprint, compared
with Inter-Process Communication (IPC) model [3]_.

Level 1 isolation
-----------------

PSA Security Model [4]_ defines 3 levels of isolation.

  - Level 1 isolation isolates Secure Processing Environment (SPE) from
    Non-secure Processing Environment (NSPE).
  - PSA Root of Trust (PSA RoT) and Application Root of Trust (ARoT) are
    isolated from each other in level 2 isolation.
  - Individual secure partitions are isolated from each other even within a
    particular security domain (PSA RoT, ARoT), in level 3 isolation.

Profile Small dedicated use cases with simple service model may not require
level 2 or level 3 isolation. Devices which Profile Small aims at may be unable
to implement stricter isolation, limited by hardware capabilities.

Level 1 isolation reduces requirements enforced by hardware isolation and cost
of software for management.

.. note ::

    **Security note**

    If a device or a use case enforces level 2 or level 3 isolation, it is
    suggested to apply other configurations, other than TF-M Profile Small.

Crypto service
==============

TF-M Profile Small only requires symmetric crypto since symmetric algorithms
require shorter keys and less computational burden, compared with asymmetric
crypto.

By default, TF-M Profile Small requires the same capabilities as defined in
TLS-PSK, to support symmetric key algorithms based protocols.

.. note ::

    **Implementation note**

    Please note that TF-M Profile Small doesn't require that TLS-PSK is
    mandatory in applications. Instead, Profile Small only requires the same
    capabilities as defined in TLS-PSK, such as one symmetric cipher algorithm
    and one hash function.

TF-M Profile Small selects TLS-PSK cipher suite TLS_PSK_WITH_AES_128_CCM [6]_
as reference, which requires:

    - AES-128-CCM (AES CCM mode with 128-bit key) as symmetric crypto algorithm
    - SHA256 as Hash function
    - HMAC as Message Authentication Code algorithm

TLS_PSK_WITH_AES_128_CCM is selected since it requires small key length and less
hardware capabilities, while keeping enough level of security.

.. note ::

    **Implementation note**

    Developers can replace default algorithms with others or implement more
    algorithms.

    Proper symmetric key algorithms and cipher suites should be selected
    according to device capabilities, the use case and the requirement of peers
    in connection.

    Refer to `Crypto service configuration`_ for implementation details of
    configuring algorithms and cipher suites.

.. note ::

    **Security note**

    It is recommended not to use MD5 or SHA-1 for message digests as they are
    subject to collision attacks [7]_ [8]_.

Secure Storage
==============

TF-M Profile Small assumes that extremely constrained devices only contain basic
on-chip storage, without external or removable storage.
As a result, TF-M Profile Small includes ITS service and disables Protected
Storage service.

Encryption and rollback protection
----------------------------------

Neither encryption nor rollback protection is enabled in current ITS
implementation.

It is expected that ITS relies solely on the physical inaccessibility property
of on-chip storage, together with PSA isolation, without requiring additional
cryptographic protection.

Internal transient buffer
-------------------------

ITS implements a internal transient buffer [9]_ to hold the data read
from/written to storage, especially for flash, to solve the alignment and
security issues.

The internal transient buffer is aligned to the flash device’s program unit.
Copying data to it from the caller can align all write requests to the flash
device’s program unit.
The internal transient buffer can help protect Flash access from some attacks,
such as TOCTOU attack.

Although removing this internal buffer can save some memory consumption,
typically 512 bytes, it may bring alignment or security issues.
Therefore, to achieve a better trade-off between memory footprint and security,
TF-M Profile Small optimizes the internal buffer size to 32 bytes by default.

As discussed in `Crypto service`_, TF-M Profile Small requires AES-128 and
SHA-256, which use 128-bit key and 256-bit key respectively.
Besides, either long public/private keys or PKI-based certificates should be
very rare as asymmetric crypto is not supported in Profile Small.
Therefore, a 32-byte internal buffer should cover the assets in TF-M Profile
Small use cases.

The buffer size can be adjusted according to use case and device Flash
attributes. Refer to `Internal Trusted Storage configurations`_ for more
details.

Initial Attestation
===================

Profile Small requires an Initial Attestation secure service based on symmetric
key algorithms. Refer to PSA Attestation API document [10]_ for details of
Initial Attestation based on symmetric key algorithms.

It can heavily increase memory footprint to support Initial Attestation based on
asymmetric key algorithms, due to asymmetric ciphers and related PKI modules.

.. note ::

    **Implementation note**

    As pointed out by PSA Attestation API document [10]_, the use cases of
    Initial Attestation based on symmetric key algorithms can be limited due to
    the associated infrastructure costs for key management and operational
    complexities. It may also restrict the ability to interoperate with
    scenarios that involve third parties.

    If asymmetric key algorithms based Initial Attestation is required in use
    scenarios, it is recommended to select other TF-M Profiles which support
    asymmetric key algorithms.

.. note ::

    **Implementation note**

    It is recommended to utilize the same MAC algorithm supported in Crypto
    service to complete the signing in ``COSE_Mac0``, to minimize memory
    footprint.

Lightweight boot
================

If MCUBoot provided by TF-M is enabled, single image boot [11]_ is selected by
default in Profile Small.
In case of single image boot, secure and non-secure images are handled as a
single blob and signed together during image generation.

However, secure and non-secure images must be updated together in single image
boot. It may decrease the flexibility of image update and cost longer update
process. Since the image sizes should usually be small with limited
functionalities in Profile Small dedicated use case, the cost may still be
reasonable.

BL2 implementation can be device specific. Devices may implement diverse
boot processes with different features and configurations.
However, anti-rollback protection is required as a mandatory feature of boot
loader. Boot loader should be able to prevent unauthorized rollback, to protect
devices from being downgraded to earlier versions with known vulnerabilities.

**************
Implementation
**************

Overview
========

The basic idea is to add dedicated profile CMake configuration files under
folder ``config/profile`` for TF-M Profile Small default configuration.

The top-level Profile Small config file collects all the necessary
configuration flags and set them to default values, to explicitly enable the
features required in Profile Small and disable the unnecessary ones, during
TF-M build.

A platform/use case can provide a configuration extension file to overwrite
Profile Small default setting and append other configurations.
This configuration extension file can be added via parameter
``TFM_EXTRA_CONFIG_PATH`` in build command line.

The behavior of the Profile Small build flow (particularly the order of
configuration loading and overriding) can be found at
:ref:`tfm_cmake_configuration`

The details of configurations will be covered in each module in
`Implementation details`_.

Implementation details
======================

This section discusses the details of Profile Small implementation.

Top-level configuration files
-----------------------------

The firmware framework configurations in ``config/profile/profile_small`` are
shown below.

.. table:: TFM options in Profile Small top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | Configs                                    | Default value                                                                                       | Descriptions                        |
   +============================================+=====================================================================================================+=====================================+
   | ``TFM_ISOLATION_LEVEL``                    | ``1``                                                                                               | Select level 2 isolation            |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_LIB_MODEL``                          | ``ON``                                                                                              | Select Library model                |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INTERNAL_TRUSTED_STORAGE`` | ``ON``                                                                                              | Enable ITS SP                       |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``ITS_BUF_SIZE``                           | ``32``                                                                                              | ITS internal transient buffer size  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_CRYPTO``                   | ``ON``                                                                                              | Enable Crypto service               |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_MBEDCRYPTO_CONFIG_PATH``             | ``${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_small.h``  | Mbed Crypto config file path        |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``CRYPTO_ASYM_SIGN_MODULE_DISABLED``       | ``ON``                                                                                              | Disable asymmetric signature        |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED``    | ``ON``                                                                                              | Disable asymmetric encryption       |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INITIAL_ATTESTATION``      | ``ON``                                                                                              | Enable Initial Attestation service  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``SYMMETRIC_INITIAL_ATTESTATION``          | ``ON``                                                                                              | Enable symmetric attestation        |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PROTECTED_STORAGE``        | ``OFF``                                                                                             | Enable PS service                   |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PLATFORM``                 | ``OFF``                                                                                             | Enable TF-M Platform SP             |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_AUDIT_LOG``                | ``OFF``                                                                                             | Disable TF-M audit logging service  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+

.. note ::

    **Implementation note**

    The following sections focus on the feature selection via configuration
    setting.
    Dedicated optimization on memory footprint is not covered in this document.

Device configuration extension
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To change default configurations and add platform specific configurations,
a platform can add a platform configuration file at
``platform/ext<TFM_PLATFORM>/config.cmake``

TF-M framework setting
----------------------

The top-level Profile Small CMake config file selects Library model and level 1
isolation.

Users can set ``-DCONFIG_TFM_SPM_BACKEND=SFN`` in build command to select SFN
model instead.

Crypto service configuration
----------------------------

Crypto Secure Partition
^^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Small enables Crypto Secure Partition (SP) in its top-level CMake
config file. Crypto SP modules not supported in TF-M Profile Small are disabled.
The disabled modules are shown below.

    - Disable asymmetric cipher

Other modules and configurations [12]_ are kept as default values.

Additional configuration flags with more fine granularity can be added to
control building of specific crypto algorithms and corresponding test cases.

Mbed Crypto configurations
^^^^^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Small adds a dedicated Mbed Crypto config file
``tfm_mbedcrypto_config_profile_small.h`` at
``/lib/ext/mbedcrypto/mbedcrypto_config``
file, instead of the common one ``tfm_mbedcrypto_config_default.h`` [12]_.

Major Mbed Crypto configurations are set as listed below:

    - Enable SHA256
    - Enable generic message digest wrappers
    - Enable AES
    - Enable CCM mode for symmetric ciphers
    - Disable other modes for symmetric ciphers
    - Disable asymmetric ciphers
    - Disable HMAC-based key derivation function (HKDF)

Other configurations can be selected to optimize the memory footprint of Crypto
module.

A device/use case can append an extra config header to the  Profile Small
default Mbed Crypto config file. This can be done by setting the
``TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH`` cmake variable in the platform
config file ``platform/ext<TFM_PLATFORM>/config.cmake``. This cmake variable is
a wrapper around the ``MBEDTLS_USER_CONFIG_FILE`` options, but is preferred as
it keeps all configuration in cmake.

Internal Trusted Storage configurations
---------------------------------------

ITS service is enabled in top-level Profile Small CMake config file.

The internal transient buffer size ``ITS_BUF_SIZE`` [9]_ is set to 32 bytes by
default. A platform/use case can overwrite the buffer size in its specific
configuration extension according to its actual requirement of assets and Flash
attributes.

Profile Small CMake config file won't touch the configurations of device
specific Flash hardware attributes [9]_.

Initial Attestation secure service
----------------------------------

TF-M Profile Small provides a reference implementation of symmetric key
algorithms based Initial Attestation, using HMAC SHA-256 as MAC algorithm in
``COSE_Mac0`` structure. The implementation follows PSA Attestation API document
[10]_.

Profile Small top-level config file enables Initial Attestation secure service
and selects symmetric key algorithms based Initial Attestation by default.

    - Set ``TFM_PARTITION_INITIAL_ATTESTATION`` to ``ON``
    - Set ``SYMMETRIC_INITIAL_ATTESTATION`` to ``ON``

Symmetric and asymmetric key algorithms based Initial Attestation can share the
same generations of token claims, except Instance ID claim.

Profile Small may implement the procedure or rely on a 3rd-party tool to
construct and sign ``COSE_Mac0`` structure.

Details of symmetric key algorithms based Initial Attestation design will be
covered in a dedicated document.

Disabled secure services
------------------------

Audit logging, Protected Storage, and Platform Service are disabled by default
in Profile Small top-level CMake config file.

Test configuration
------------------

Some cryptography tests are disabled due to the reduced Mbed Crypto config.
Some of them are shown in the table below.

.. table:: TFM options in Profile Small top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | Configs                                    | Default value                                                                                       | Descriptions                        |
   +============================================+=====================================================================================================+=====================================+
   | ``TFM_CRYPTO_TEST_ALG_CBC``                | ``OFF``                                                                                             | Test CBC cryptography mode          |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CCM``                | ``ON``                                                                                              | Test CCM cryptography mode          |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CFB``                | ``OFF``                                                                                             | Test CFB cryptography mode          |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CTR``                | ``OFF``                                                                                             | Test CTR cryptography mode          |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_GCM``                | ``OFF``                                                                                             | Test GCM cryptography mode          |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_SHA_512``            | ``OFF``                                                                                             | Test SHA-512 cryptography algorithm |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_HKDF``                   | ``OFF``                                                                                             | Test HKDF key derivation algorithm  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_CRYPTO_TEST_ECDH``                   | ``OFF``                                                                                             | Test ECDH key agreement algorithm   |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+

BL2 setting
-----------

Profile Small enables MCUBoot provided by TF-M by default. A platform can
overwrite this configuration by disabling MCUBoot in its configuration extension
file ``platform/ext<TFM_PLATFORM>/config.cmake``.

If MCUBoot provided by TF-M is enabled, single image boot is selected in TF-M
Profile Small top-level CMake config file.

If a device implements its own boot loader, the configurations are
implementation defined.

.. table:: BL2 options in Profile Small top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | Configs                                    | Default value                                                                                       | Descriptions                        |
   +============================================+=====================================================================================================+=====================================+
   | ``BL2``                                    | ``ON``                                                                                              | Enable MCUBoot bootloader           |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``MCUBOOT_IMAGE_NUMBER``                   | ``1``                                                                                               | Combine S and NS images             |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+

****************
Platform support
****************

Building Profile Small
======================

To build Profile Small, argument ``TFM_PROFILE`` in build command line should be
set to ``profile_small``.

Take AN521 as an example.

The following commands build Profile Small without test cases on **AN521** with
build type **MinSizeRel**, built by **Armclang**.
Library model is selected by default.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=arm/mps2/an521 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_small \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         ../
   cmake --build ./ -- install

The following commands build Profile Small with regression test cases on
**AN521** with build type **MinSizeRel**, built by **Armclang**.
Library model is selected by default.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=arm/mps2/an521 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_small \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         -DTEST_NS=ON \
         ../
   cmake --build ./ -- install

.. Note::

 - For devices with more constrained memory and flash requirements, it is
   possible to build with either only TEST_S enabled or only TEST_NS enabled.
   This will decrease the size of the test images. Note that both test suites
   must still be run to ensure correct operation.

The following commands build Profile Small with SFN model on **AN521** with
build type **MinSizeRel**, built by **GNU Arm compiler**.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=arm/mps2/an521 \
         -DTFM_PROFILE=profile_small \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         -DCONFIG_TFM_SPM_BACKEND=SFN \
         ../
   cmake --build ./ -- install

More details of building instructions and parameters can be found TF-M build
instruction guide [13]_.

********
Appendix
********

TF-M Library model implementation details
=========================================

.. note ::

    **Implementation note**

    Please note that there is no public dedicated specification for Library
    model.
    The design, interfaces and implementation of Library model in TF-M may
    change.

Buffer sharing allowed
----------------------

To simplify interface and reduce memory footprint, TF-M Library model directly
handles client call input vectors from non-secure client buffers and later
writes results back to those buffers, without keeping a copy in a transient
buffer inside TF-M.

.. note ::

    **Security note**

    There can be security vulnerabilities if non-secure client buffers are
    directly shared between NSPE and SPE, such as Time-of-check to time-of-use
    (TOCTOU) attack.

    Developers need to check if this can meet the Security Functional
    Requirements (SFR) of the integration of their devices.
    Some SFRs are listed in a set of example Threat Models and Security Analyses
    (TMSA) offered by PSA for common IoT use cases. [5]_

Single secure context
---------------------

TF-M Library model only supports single secure context.

It cannot support multiple contexts or the scheduling implemented in IPC model.
It neither can support multiple outstanding PSA client calls.

But correspondingly, it can save memory footprint and runtime complexity in
context management and scheduling.

.. note ::

    **Security note**

    Non-secure software should prevent triggering multiple outstanding PSA
    client calls concurrently. Otherwise, it may crash current running secure
    context.

*********
Reference
*********

.. [1] `Pre-Shared Key Ciphersuites for Transport Layer Security (TLS) <https://tools.ietf.org/html/rfc4279>`_

.. [2] `Arm Firmware Framework for M 1.1 Extensions <https://developer.arm.com/documentation/aes0039/latest>`_

.. [3] `Arm Platform Security Architecture Firmware Framework 1.0 <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4&hash=3BFD6F3E687F324672F18E5BE9F08EDC48087C93>`_

.. [4] `Platform Security Model 1.1 <https://developer.arm.com/documentation/den0128/latest>`_

.. [5] `PSA analyze stage <https://developer.arm.com/architectures/security-architectures/platform-security-architecture#analyze>`_

.. [6] `AES-CCM Cipher Suites for Transport Layer Security (TLS) <https://tools.ietf.org/html/rfc6655>`_

.. [7] `Updated Security Considerations for the MD5 Message-Digest and the HMAC-MD5 Algorithms <https://tools.ietf.org/html/rfc6151>`_

.. [8] `Transitioning the Use of Cryptographic Algorithms and Key Lengths <https://www.nist.gov/publications/transitioning-use-cryptographic-algorithms-and-key-lengths>`_

.. [9] :doc:`ITS integration guide </integration_guide/services/tfm_its_integration_guide>`

.. [10] `PSA Attestation API 1.0 (ARM IHI 0085) <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Implement/IHI0085-PSA_Attestation_API-1.0.2.pdf?revision=eef78753-c77e-4b24-bcf0-65596213b4c1&la=en&hash=E5E0353D612077AFDCE3F2F3708A50C77A74B2A3>`_

.. [11] :doc:`Secure boot </technical_references/design_docs/tfm_secure_boot>`

.. [12] :doc:`Crypto design </technical_references/design_docs/tfm_crypto_design>`

.. [13] :doc:`TF-M build instruction </technical_references/instructions/tfm_build_instruction>`

--------------

*Copyright (c) 2020-2022, Arm Limited. All rights reserved.*
