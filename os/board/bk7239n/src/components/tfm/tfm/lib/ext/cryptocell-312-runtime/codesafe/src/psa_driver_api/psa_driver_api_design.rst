###############################################
PSA Cryptoprocessor Driver Interface for CC-312
###############################################

This document describes the high level design of the driver interface to the
CC-312 Cryptoprocessor based on PSA interfaces. The driver interface is named
as ``CC-3xx`` to indicate that it should be generic enough to easily support
multiple versions of the Cryptocell 3XX lineup.

.. note::
  The original specification is available here:
  `PSA Cryptoprocessor Driver Interface <https://github.com/ARMmbed/mbedtls/blob/development/docs/proposed/psa-driver-interface.md>`_

************
Introduction
************
The PSA Cryptoprocessor Driver interface describes a way to uniformly interface
a compliant PSA Crypto implementation to a cryptographic accelerator, that can
be either a _transparent_ accelerator, i.e. which does not handle key materials
in a separated environment, or an _opaque_ accelerator, i.e. a secure element
which is able to store keys in a separate, protected domain from the rest of
the driver core.

Due to the nature of the CC-312 processor, the natural choice is to implement
the PSA Cryptoprocessor Driver Interface as a transparent driver.
Implementations with more complex requirements can extend the driver interface
to provide support for a dedicated key management subsystem or add support for
the hardware based (OTP) key storage offered by CC-312, thus implementing
interfaces which are specific to an _opaque_ driver as described by the
specification. The high level design described below behaves as a _transparent_
driver.


*********
Structure
*********
Overall, the driver is a high-level interface between the PSA Crypto driver
core which must be provided by a PSA compliant implementation, i.e. mbedTLS,
and the rest of the driver stack of the CC-312. The driver exports a set of
`Public APIs`_, which are the public functions that are available for the
crypto core to call functionalities of the driver, and a set of `Internal APIs`_,
functions which are meant to be called by other modules of the driver, for
example utility functions, but that are not implementing any *driver entry
point* as described by the specification.

In addition to this, the driver provides a main inclusion point, ``cc3xx.h``,
and type declaration and definition headers, i.e. ``cc3xx_crypto_primitives.h``
and ``cc3xx_crypto_primitives_private.h``. The main inclusion point contains
the declarations of all the driver entry points and needs to be included by the
PSA Crypto driver core. The `Types`_ declaration and definition headers
contain the types associated to the driver. Any module that does not need to
access internal details of the type can include the public header only and
treat each object as an opaque pointer, while any module that needs access to
internals of the type needs to include the private header. In particular, the
PSA Crypto driver core needs to get access to type internals, hence
``cc3xx_crypto_primitives_private.h`` is included by the PSA Crypto driver core
as well. It's worth to note that when the driver interface is enabled in the
core, i.e. when ``PSA_CRYPTO_DRIVER_CC3XX`` is defined, the type definitions
are visible through inclusion of the main PSA Crypto header file, i.e.
``psa/crypto.h``.


Public APIs
===========
The source and header files which are prepended with ``cc3xx_psa_*`` implement
a family of driver entry points. The families implemented by the driver are as
follows:

* ``aead``: Authenticated Encryption with Associated Data
* ``asymmetric_encryption``: Public key encryption algorithms
* ``asymmetric_signature``: Public key signature algorithms
* ``cipher``: Symmetric cipher encryption and decryption algorithms
* ``entropy``: Driver entry point to sample the CC-312 TRNG to extract entropy,
  for example to seed a determinist random binary generator
* ``hash``: Hashing algorithms
* ``key_agreement``: Driver entry point to perform raw key agreement based on
  Ellipctic Curve Diffie Hellman (ECDH)
* ``key_generation``: Driver entry points to generate random keys and to
  accelerate public key extraction from RSA/ECDSA private keys
* ``mac``: Algorithm for MAC generation and verification


Internal APIs
=============
The source and header files which are prepended with ``cc3xx_internal_*``
implement public functions which are not meant to be called by the PSA Crypto
driver core, i.e. they don't implement a driver entry point, but they need to
be called between modules, as they are usually utility functions or algorithm-
specific implementations that are called by the main entry points. The internal
modules available are as follows:

* ``aes``: AES based symmetric encryption and decryption
* ``asn1_util``: Utility functions for ASN-1 parsing
* ``ccm``: AES-CCM based AEAD
* ``chacha20-poly1305``: Chacha20-Poly1305 based AEAD
* ``chacha20``: Chacha20 based symmetric encryption and decryption
* ``drbg_util``: Implementation of utility functions for CTR-DRBG and HMAC-
  DRBG. This module has an explicit dependency on the mbedTLS DRBG software
  implementation
* ``ecc_util``: Utility functions for ECC key types conversion and parsing
* ``ecdh``: ECDH based raw key agreement
* ``gcm``: AES-GCM based AEAD
* ``hash_util``: Utility functions for hashing mode conversion between PSA and
  the rest of the CC-312 driver stack
* ``rsa_util``: Utility functions for RSA key types conversion and parsing

It's worth reminding that _both_ public and internal APIs do not implement
functionalities by themselves in software but call into the
low-level driver stack when possible to benefit from hardware acceleration.


Types
=====
The driver defines several types which are used to handle _multipart_ APIs,
i.e. those call flows where a context needs to be stored to keep track of the
state of the operation. On the opposite side, one-shot API for the same
functionality don't need to store contexts. The types for multipart operations
are the following;

* ``cc3xx_hash_operation_t``: Required for Hashing multipart APIs flow
* ``cc3xx_cipher_operation_t``: Required for symmetric cipher multipart
  encryption/decryption APIs flow
* ``cc3xx_mac_operation_t``: Required for MAC multipart signing/verification
  APIs flow
* ``cc3xx_aead_operation_t``: Required for AEAD multipart APIs flow


Configuration options
=====================
The driver interface is configurable through usage of compile time definitions.
The main compile time definition exported is ``PSA_CRYPTO_DRIVER_CC3XX`` which
is not strictly a configuration option of the driver interface but it enables
the driver module integration into the PSA Crypto driver core. The following
compile time defines are used instead to configure the ``cc3xx`` driver at
compile time:

* ``CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION``: Enable the RSA and ECC
  conversion functions to translate from CC-312 specific errors to PSA
  compliant errors. This option is useful only in debug as for normal
  operations (i.e. returning PSA_SUCCESS), the translation is trivial. Hence it
  is suggested to enable this option only when debugging is required, as the
  conversion functions involved take ~1KB of code size.
* ``CC3XX_CONFIG_SUPPORT_SHA1``: Enable the driver interface support for SHA-1
  algorithms in Hash and HMAC. As SHA-1 is deemed unsecure and its usage not
  recommended, this option is disabled by default to save code size.
* ``CC3XX_CONFIG_SUPPORT_RSA``: Enable the driver interface support for RSA
  algorithms. This option is enabled by default.
* ``CC3XX_CONFIG_SUPPORT_GCM``: Enable the driver interface support for GCM
  algorithm in AEAD. This option is enabled by default.

.. Note::

    The current status of the configuration option is still work in progress
    and can be modified following updates in the specification and in the
    reference implementation in mbed TLS. For simplicity, some of the options
    described above are directly tied to the equivalent `mbedcrypto_config.h`
    define in order to be controllable through the same mechanism at the
    moment.


Dependencies
============
The aim of the driver is to be self-contained as much as possible. The only
natural dependency should be on functions from the rest of the CC-312
low-level driver stack, in order to interface correctly to the underlying
hardware. Anyway, there are some dependencies on external modules as well
that need to be addressed.

* ``mbedtls_platform``: Currently some of the modules use directly the
  ``mbedtls_calloc`` and ``mbedtls_free`` functions for dynamic memory
  allocation. This implies that those abstractions need to be working correctly
  for the platform in use.
* ``mbedtls_drbg``: The module currently relies on a software implementation of
  DRBG algorithms (HMAC-DRBG and CTR-DRBG) provided by the mbedTLS software
  layer, as the CC-312 low-level driver stack does not entirely accelerate DRBG
  algorithms.


********************
Algorithm by modules
********************
* ``aead``: AES-GCM, AES-CCM, Chacha20-Poly1305
* ``asymmetric_encryption``: RSA-OAEP, RSA-PKCS#1 v1.5 encryption
* ``asymmetric_signature``: RSA-PSS, RSA-PKCS#1 v1.5 signing, ECDSA
* ``cipher``: AES-ECB, AES-CBC (No padding, PKCS7 padding), AES-CTR, AES-OFB,
  Chacha20
* ``hash``: SHA-1 (disabled by default), SHA-2 (SHA-224, SHA-256)
* ``key_agreement``: ECDH
* ``mac``: CMAC, HMAC(SHA-1), HMAC(SHA-224), HMAC(SHA-256)


****************
JSON description
****************
The driver is described through a JSON file as specified by the PSA
Cryptoprocessor Driver Interface specification. The JSON file describe the
overall driver interfaces and needs to be made available to the PSA Crypto
driver core at build time in order to properly autogenerate hooks into the core
layer. This integration is outside of the scope of the driver itself and needs
to be handled properly by the integration environment and build system.


**************************************
Doxygen style documentation generation
**************************************
The driver interface supports Doxygen style documentation for non-static APIs
when possible. To generate the doxygen report, it is sufficient to follow the
standard doxygen flow using ``psa_driver_api`` (this directory) as the working
directory, and setting ``RECURSIVE`` to ``YES`` in the Doxyfile.


*Copyright (c) 2021-2022, Arm Limited. All rights reserved.*
