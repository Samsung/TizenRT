#######################################
Trusted Firmware-M Profile Large Design
#######################################

:Author: David Hu
:Organization: Arm Limited
:Contact: david.hu@arm.com

************
Introduction
************

TF-M Profiles defines 3 profiles: Profile Small, Profile Medium and Profile
Large. Each profile provides a predefined list of TF-M configurations to meet
the security requirement of typical use cases with device hardware constraints.
TF-M Profiles align with PSA specifications and certification requirements.

As one of TF-M Profiles, Profile Large protects less resource-constrained Arm
Cortex-M devices.

Compared to Profile Small [1]_ and Profile Medium [2]_, Profile Large aims to
enable more secure features to support higher level of security required in more
complex usage scenarios.

    - Isolation level 3 enables additional isolation between
      :term:`Application RoT` (App RoT) services.
    - More crypto algorithms and cipher suites are selected to securely connect
      devices to remote services offered by various major Cloud Service
      Providers (CSP)
    - Basic software countermeasures against physical attacks can be enabled.

Profile Large can be aligned as a reference implementation with the requirements
defined in PSA Certified Level 3 Lightweight Protection Profile [3]_.

**************
Overall design
**************

TF-M Profile Large defines the following feature set:

    - Firmware Framework

        - Inter-Process Communication (IPC) model [4]_
        - Isolation level 3 [4]_

    - Internal Trusted Storage (ITS)

    - Crypto

        - Support both symmetric ciphers and asymmetric ciphers
        - Asymmetric key based cipher suites defined in TLS 1.2 [5]_ to support
          direct secure connection to major CSPs, including

            - Authenticated Encryption with Associated Data (AEAD) algorithm
            - Asymmetric key algorithm based signature and verification
            - Public-key cryptography based key exchange
            - Hash function
            - HMAC for default Pseudorandom Function (PRF)

        - Asymmetric digital signature and verification for Initial Attestation
          Token (IAT)
        - Asymmetric algorithms for firmware image signature verification
        - Key derivation

    - Initial Attestation

        - Asymmetric key algorithm based Initial Attestation

    - Secure boot

        - Anti-rollback protection
        - Multiple image boot

    - Protected Storage (PS) if off-chip storage device is integrated

        - Data confidentiality
        - Data integrity
        - Rollback protection

    - Software countermeasures against physical attacks

**************
Design details
**************

More details of TF-M Profile Large design are described in following sections.

Firmware framework
==================

Profile Large selects IPC model and isolation level 3 by default.

Isolation level 3 supports additional isolation between App RoT services,
compared to isolation level 2. It can protect :term:`RoT` services from each
other when their vendors don't trust each other.

Crypto service
==============

Profile Large supports direct connection to Cloud services via common protocols,
such as TLS 1.2.

In some usage scenarios, PSA RoT can be managed by device manufacturer or other
vendors and is out of control of application developers.
Profile Large selects alternative crypto algorithms for each crypto function to
support multiple common cipher suites required by various major CSPs. Therefore,
application developers can support services for diverse CSPs on same devices
with Profile Large, without relying on PSA RoT upgrades of crypto.

Devices meeting Profile Large should be in a position to offer at least two
alternatives to every cryptographic primitive for symmetric, asymmetric and
hash, and be able to use them for encryption, AEAD, signature and verification.

It will cost more resource in Profile Large to support more crypto algorithms
and cipher suites, compared to Profile Medium [2]_.

Boot loader
===========

BL2 implementation can be device specific. Devices may implement diverse
boot processes with different features and configurations.
However, the boot loader must support anti-rollback protection. Boot loader must
be able to prevent unauthorized rollback, to protect devices from being
downgraded to earlier versions with known vulnerabilities.

MCUBoot in TF-M is configured as multiple image boot by default in Profile
Large. In multiple image boot, secure and non-secure images can be signed
independently with different keys and they can be updated separately. It can
support multiple vendors scenarios, in which non-secure and secure images are
generated and updated by different vendors.
Multiple image boot may cost larger memory footprint compared with single image
boot.

Boot loader can implement software countermeasures to mitigate physical attacks.

Protected Storage
=================

PS service is required if an off-chip storage device is integrated and used on
the platform.

Anti-rollback protection in PS relies on non-volatile counter(s) provided by
TF-M Platform :term:`Secure Partition` (SP).

TF-M audit logging service
==========================

TF-M audit logging service allows secure services in the system to log TF-M
events and information.

TF-M audit logging service is not enabled in Profile Large since its IPC model
dedicated interface is not ready yet.

.. note ::

    **Implementation note**

    Please note that there is no dedicated PSA specification for Audit Logging
    yet.
    The design, interfaces and implementation of TF-M audit logging service may
    change.

Software countermeasures against physical attacks
=================================================

TF-M Profile Large enables TF-M Fault Injection Hardening (FIH) library Profile
Medium by default. It enables the following countermeasure techniques:

    - Control flow monitor
    - Failure loop hardening
    - Complex constants
    - Redundant variables and condition checks

Refer to TF-M physical attack mitigation design document [6]_ for FIH library
details.

.. note ::

    **TF-M FIH library is still under development**.

    TF-M FIH library hardens TF-M critical execution steps to make physical
    attacks more difficult, together with device hardware countermeasures.
    It is not guaranteed that TF-M FIH library is able to mitigate all kinds of
    physical attacks.

.. note ::

    **Implementation note**

    TF-M FIH library doesn't cover platform specific critical configurations.
    Platforms shall implement software countermeasures against physical attacks
    to protect platform specific implementation.

**************
Implementation
**************

Overview
========

The basic idea is to add dedicated profile CMake configuration files under
folder ``config/profile`` for TF-M Profile Large default configuration, the
same as other TF-M Profiles do.

The top-level Profile Large config file collects all the necessary configuration
flags and set them to default values, to explicitly enable the features required
in Profile Large and disable the unnecessary ones, during TF-M build.

A platform/use case can provide a configuration extension file to overwrite
Profile Large default setting and append other configurations.
This configuration extension file can be added via parameter
``TFM_EXTRA_CONFIG_PATH`` in build command line.

The behaviour of the Profile Large build flow (particularly the order of
configuration loading and overriding) can be found at
:ref:`tfm_cmake_configuration`

The details of configurations will be covered in each module in
`Implementation details`_.

Implementation details
======================

This section discusses the details of Profile Large implementation.

Top-level configuration files
-----------------------------

The firmware framework configurations in ``config/profile/profile_large`` are
shown below.

.. table:: Config flags in Profile Large top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | Configs                                    | Descriptions                       | Default value                                                                                      |
   +============================================+====================================+====================================================================================================+
   | ``TFM_ISOLATION_LEVEL``                    | Select level 3 isolation           | ``3``                                                                                              |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_LIB_MODEL``                          | Select IPC model                   | ``OFF``                                                                                            |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_INTERNAL_TRUSTED_STORAGE`` | Enable ITS SP                      | ``ON``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``ITS_BUF_SIZE``                           | ITS internal transient buffer size | ``64``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_CRYPTO``                   | Enable Crypto service              | ``ON``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_MBEDCRYPTO_CONFIG_PATH``             | MbedTLS config file path           | ``${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_large.h`` |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_INITIAL_ATTESTATION``      | Enable Initial Attestation service | ``ON``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_PROTECTED_STORAGE`` [a]_   | Enable PS service                  | ``ON``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_PLATFORM``                 | Enable TF-M Platform SP            | ``ON``                                                                                             |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+
   | ``TFM_PARTITION_AUDIT_LOG``                | Disable TF-M audit logging service | ``OFF``                                                                                            |
   +--------------------------------------------+------------------------------------+----------------------------------------------------------------------------------------------------+

.. [a] PS service is enabled by default. Platforms without off-chip storage
       devices can turn off ``TFM_PARTITION_PROTECTED_STORAGE`` to disable PS
       service. See `Protected Storage Secure Partition`_ for details.

Crypto service configurations
-----------------------------

Crypto Secure Partition
^^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Large enables Crypto SP in top-level CMake config file and selects
all the Crypto modules.

MbedTLS configurations
^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Large adds a dedicated MbedTLS config file
``tfm_mbedcrypto_config_profile_large.h`` under
``/lib/ext/mbedcrypto/mbedcrypto_config`` folder, instead of the common one
``tfm_mbedcrypto_config_default.h`` [7]_.

Major MbedTLS configurations are set as listed below:

    - Enable SHA256 and SHA512
    - Enable generic message digest wrappers
    - Enable AES
    - Enable CCM mode, GCM mode and CBC mode for symmetric ciphers
    - Disable other modes for symmetric ciphers
    - Enable ECDH
    - Enable ECDSA
    - Enable RSA
    - Select ECC curve ``secp256r1`` and ``secp384r1``
    - Enable HMAC-based key derivation function
    - Other configurations required by selected option above

A device/use case can append an extra config header to the Profile Large default
MbedTLS config file to override the default settings. This can be done by
setting the ``TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH`` cmake variable in the
platform config file ``platform/ext<TFM_PLATFORM>/config.cmake``.
This cmake variable is a wrapper around the ``MBEDTLS_USER_CONFIG_FILE``
options, but is preferred as it keeps all configuration in cmake.

Internal Trusted Storage configurations
---------------------------------------

ITS service is enabled in top-level Profile Large CMake config file by default.

The internal transient buffer size ``ITS_BUF_SIZE`` [8]_ is set to 64 bytes by
default. A platform/use case can overwrite the buffer size in its specific
configuration extension according to its actual requirement of assets and Flash
attributes.

Profile Large CMake config file won't touch the configurations of device
specific Flash hardware attributes.

Protected Storage Secure Partition
----------------------------------

Data confidentiality, integrity and anti-rollback protection are enabled by
default in PS.

If PS is selected, AES-CCM is used as AEAD algorithm by default. If platform
hardware crypto accelerator supports the AEAD algorithm, the AEAD operations can
be executed in hardware crypto accelerator.

If platforms don't integrate any off-chip storage device, platforms can disable
PS in platform specific configuration extension file via
``platform/ext<TFM_PLATFORM>/config.cmake``.

BL2 setting
-----------

Profile Large enables MCUBoot provided by TF-M by default. A platform can
overwrite this configuration by disabling MCUBoot in its configuration extension
file ``platform/ext<TFM_PLATFORM>/config.cmake``.

If MCUBoot provided by TF-M is enabled, multiple image boot is selected by
default.

If a device implements its own boot loader, the configurations are
implementation defined.

Software countermeasure against physical attacks
------------------------------------------------

Profile Large selects TF-M FIH library Profile Medium by specifying
``-DTFM_FIH_PROFILE=MEDIUM`` in top-level CMake config file.

System integrators shall implement software countermeasures in platform specific
implementations.

Device configuration extension
------------------------------

To change default configurations and add platform specific configurations,
a platform can add a platform configuration file at
``platform/ext<TFM_PLATFORM>/config.cmake``

Test configuration
------------------

Some cryptography tests are disabled due to the reduced MbedTLS config.
Profile Large specific test configurations are also specified in Profile Large
top-level CMake config file ``config/profile/profile_large``.

.. table:: Profile Large crypto test configuration
   :widths: auto
   :align: center

   +--------------------------------------------+---------------+-----------------------------------------+
   | Configs                                    | Default value | Descriptions                            |
   +============================================+===============+=========================================+
   | ``TFM_CRYPTO_TEST_ALG_CBC``                | ``ON``        | Test CBC cryptography mode              |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CCM``                | ``ON``        | Test CCM cryptography mode              |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_GCM``                | ``ON``        | Test GCM cryptography mode              |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_SHA_512``            | ``ON``        | Test SHA-512 cryptography algorithm     |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_HKDF``                   | ``ON``        | Test HMAC-based key derivation function |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CFB``                | ``OFF``       | Test CFB cryptography mode              |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ALG_CTR``                | ``OFF``       | Test CTR cryptography mode              |
   +--------------------------------------------+---------------+-----------------------------------------+
   | ``TFM_CRYPTO_TEST_ECDH``                   | ``ON``        | Test ECDH key agreement algorithm       |
   +--------------------------------------------+---------------+-----------------------------------------+

****************
Platform support
****************

To enable Profile Large on a platform, the platform specific CMake file should
be added into the platform support list in top-level Profile Large CMake config
file.

Building Profile Large
======================

To build Profile Large, argument ``TFM_PROFILE`` in build command line should be
set to ``profile_large``.

Take AN521 as an example:

The following commands build Profile Large without test cases on **AN521** with
build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=arm/mps2/an521 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_large \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         ../
   cmake --build ./ -- install

The following commands build Profile Large with regression test cases on
**AN521** with build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=arm/mps2/an521 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_large \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         -DTEST_S=ON -DTEST_NS=ON \
         ../
   cmake --build ./ -- install

More details of building instructions and parameters can be found TF-M build
instruction guide [9]_.

*********
Reference
*********

.. [1] :doc:`Trusted Firmware-M Profile Small Design </technical_references/design_docs/profiles/tfm_profile_small>`

.. [2] :doc:`Trusted Firmware-M Profile Medium Design </technical_references/design_docs/profiles/tfm_profile_medium>`

.. [3] `PSA Certified Level 3 Lightweight Protection Profile <https://www.psacertified.org/app/uploads/2020/12/JSADEN009-PSA_Certified_Level_3_LW_PP-1.0-BET02.pdf>`_

.. [4] `Arm Platform Security Architecture Firmware Framework 1.0 <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`_

.. [5] `The Transport Layer Security (TLS) Protocol Version 1.2 <https://tools.ietf.org/html/rfc5246>`_

.. [6] :doc:`Physical attack mitigation in Trusted Firmware-M </technical_references/design_docs/tfm_physical_attack_mitigation>`

.. [7] :doc:`Crypto design </technical_references/design_docs/tfm_crypto_design>`

.. [8] :doc:`ITS integration guide </integration_guide/services/tfm_its_integration_guide>`

.. [9] :doc:`TF-M build instruction </technical_references/instructions/tfm_build_instruction>`

--------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
