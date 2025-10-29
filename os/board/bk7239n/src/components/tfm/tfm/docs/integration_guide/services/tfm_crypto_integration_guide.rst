################################
Crypto Service Integration Guide
################################

************
Introduction
************
TF-M Crypto service allows application to use cryptography primitives such as
symmetric and asymmetric ciphers, hash, message authentication codes (MACs) and
authenticated encryption with associated data (AEAD).

**************
Code structure
**************
The PSA interfaces for the Crypto service are located in ``interface/include``.
The only header to be included by applications that want to use functions from
the PSA API is ``psa/crypto.h``.
The TF-M Crypto service source files are located in
``secure_fw/partitions/crypto``.

PSA interfaces
==============
The TF-M Crypto service exposes the PSA interfaces detailed in the header
``psa/crypto.h``. This header, in turn, includes several other headers which
are not meant to be included directly by user applications. For a detailed
description of the PSA API interface, please refer to the comments in the
``psa/crypto.h`` header itself.

Service source files
====================
- ``crypto_cipher.c`` : This module handles requests for symmetric cipher
  operations
- ``crypto_hash.c`` : This module handles requests for hashing operations
- ``crypto_mac.c`` : This module handles requests for MAC operations
- ``crypto_aead.c`` : This module handles requests for AEAD operations
- ``crypto_key_derivation.c`` : This module handles requests for key derivation
  related operations
- ``crypto_key_management.c`` : This module handles requests for key management
  related operations
- ``crypto_key.c`` : This module handles requests for key backend operations,
  including key attributes switch between caller and service.
- ``crypto_asymmetric.c`` : This module handles requests for asymmetric
  cryptographic operations
- ``crypto_init.c`` : This module provides basic functions to initialise the
  secure service during TF-M boot. When the service is built for IPC mode
  compatibility, this layer handles as well the connection requests and the
  proper dispatching of requests to the corresponding functions, and it holds
  the internal buffer used to allocate temporarily the IOVECs needed. The size
  of this buffer is controlled by the ``TFM_CRYPTO_IOVEC_BUFFER_SIZE`` define.
  This module also provides a static buffer which is used by the Mbed Crypto
  library for its own allocations. The size of this buffer is controlled by
  the ``TFM_CRYPTO_ENGINE_BUF_SIZE`` define
- ``crypto_alloc.c`` : This module is required for the allocation and release of
  crypto operation contexts in the SPE. The ``TFM_CRYPTO_CONC_OPER_NUM``,
  defined in this file, determines how many concurrent contexts are supported
  for multipart operations (8 for the current implementation). For multipart
  cipher/hash/MAC/generator operations, a context is associated to the handle
  provided during the setup phase, and is explicitly cleared only following a
  termination or an abort
- ``tfm_crypto_secure_api.c`` : This module implements the PSA Crypto API
  client interface exposed to the Secure Processing Environment
- ``tfm_crypto_api.c`` :  This module is contained in ``interface/src`` and
  implements the PSA Crypto API client interface exposed to the  Non-Secure
  Processing Environment.
- ``tfm_mbedcrypto_alt.c`` : This module contains alternative implementations of
  Mbed Crypto functions. Decryption code is skipped in AES CCM mode in Profile
  Small by default.

****************************
Crypto Backend configuration
****************************

The Crypto service can use either a hardware crypto accelerator backend like
CC-312 or a software crypto library which by default is MbedTLS.

If using MbedTLS as backend, then the library configuration is supplied using
the ``TFM_MBEDCRYPTO_CONFIG_PATH`` cmake option.

Platforms can specify an extra config file by setting the
``TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH`` variable (which is a wrapper
around the ``MBEDTLS_USER_CONFIG_FILE`` option).  This is preferred for platform
configuration over ``TFM_MBEDCRYPTO_CONFIG_PATH`` as it does not interfere with
config changes due to TFM Profile.

.. Note::

    The default entropy source configured for MbedTLS is
    MBEDTLS_ENTROPY_NV_SEED with unique seed. For production devices,
    it can also select a hardware entropy source via
    MBEDTLS_ENTROPY_HARDWARE_ALT

**************************
Crypto service integration
**************************
In this section, a brief description of the required flow of operation for the
functionalities exported by the PSA Crypto interface is given, with particular
focus on the TF-M Crypto service specific operations. For the details of the
generic PSA Crypto interface operations, please refer directly to the header
``psa/crypto.h``.

Most of the PSA Crypto multipart APIs require an operation context to be
allocated by the application and then to be passed as a pointer during the
following API calls. These operation contexts are of four main types described
below:

- ``psa_key_derivation_operation_t`` - Operation context for key derivation
- ``psa_hash_operation_t`` - Operation context for multipart hash operations
- ``psa_mac_operation_t`` - Operation context for multipart MAC operations
- ``psa_cipher_operation_t`` - Operation context for multipart cipher operations

The user applications are not allowed to make any assumption about the original
types behind these typedefs, which are defined inside ``psa/crypto.h``.
In the scope of the TF-M Crypto service, these types are regarded as handles to
the corresponding implementation defined structures which are stored in the
Secure world.

--------------

*Copyright (c) 2018-2021, Arm Limited. All rights reserved.*
