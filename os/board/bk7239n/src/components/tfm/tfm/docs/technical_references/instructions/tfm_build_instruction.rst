##################
Build instructions
##################
Please make sure you have all required software installed as explained in the
:doc:`TF-M getting started </getting_started/tfm_getting_started>`.
The additional building materials you can find in the following links:

.. toctree::
    :maxdepth: 1

    IAR toolchain <tfm_build_instruction_iar>
    Run TF-M examples <run_tfm_examples_on_arm_platforms>
    Building documentation <documentation_generation>

****************
TF-M build steps
****************
TF-M uses `cmake <https://cmake.org/overview/>`__ to provide an out-of-source
build environment. The instructions are below.

Cmake version ``3.15.0`` or higher is required.

.. _Getting the source-code:

Getting the source-code
=======================
.. code-block:: bash

    cd <base folder>
    git clone https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git

To simplify documentation commands, the new created repository under
``trusted-firmware-m`` would be referenced as ``<TF-M base folder>`` and
its parent, the ``<base folder>``. Dependency management is now handled by
cmake. If you wish to alter this behaviour, see
:ref:`technical_references/instructions/tfm_build_instruction:Manual
dependency management`

.. Note::

 - For building with Armclang compiler version 6.10.0+, please follow the note
   in :doc:`TF-M getting started </getting_started/tfm_getting_started>`.
 - For building with the IAR toolchain, please see the notes in
   :doc:`IAR software requirements <tfm_build_instruction_iar>`

.. _tfm_cmake_configuration:

Cmake configuration
===================

All configuration options are provided by cmake variables, and their default
values, with docstrings, can be found in ``config/config_default.cmake``.

Configuration is provided in multiple stages. Each stage will not override any
config that has already been set at any of the prior stages.

   1. Command-line variable settings are applied.
   2. If the ``TFM_EXTRA_CONFIG_PATH`` variable has been set, that file is
      loaded.
   3. If TEST_PSA_TEST is set, then PSA API test related config is applied from
      ``config/tests/config_test_psa_api.cmake``.
   4. If it exists, CMAKE_BUILD_TYPE specific config is applied from
      ``config/build_type/<build_type>.cmake``.
   5. Target specific config from ``platform/ext/target/<target_platform>/config.cmake``
      is applied.
   6. If CRYPTO_HW_ACCELERATOR is set, then a config specific to the
      accelerator type is applied if it exists.
   7. If it exists, TFM Profile specific config is applied from
      ``config/profile/<tfm_profile>.cmake``.
   8. ``config/config_default.cmake`` is loaded.
   9. If ``TEST_S`` or ``TEST_NS`` or other single test suite config like
      ``TEST_NS_ATTESTATION`` (see `Regression test configuration`_)is set, then
      config from ``${TFM_TEST_REPO_PATH}/test/config/set_config.cmake`` and
      ``${TFM_TEST_REPO_PATH}/test/config/default_ns_test_config.cmake`` or
      ``${TFM_TEST_REPO_PATH}/test/config/default_s_test_config.cmake`` or
      ``${TFM_TEST_REPO_PATH}/test/config/default_test_config.cmake`` is
      applied.

.. Warning::
    This means that command-line settings are not applied when they conflict
    with required platform settings. If it is required to override platform
    settings (this is not usually a good idea) then TFM_EXTRA_CONFIG_PATH should be
    used.

Required cmake parameters for building TF-M
-------------------------------------------

``TFM_PLATFORM`` is required to select the target platform, it can be:
 - A relative path under ``<TF-M_root>/platform/ext/target``,
   for example ``arm/mps2/an521``.
 - An absolute path of target platform, mainly used for out-of-tree platform
   build.
 - A target platform name that is supported under
   <TF-M_root>/platform/ext/target, for example ``an521``.

Build type
----------

By default, a release configuration is built. Alternate build types can be
specified with the ``CMAKE_BUILD_TYPE`` variable. The possible
types are:

 - ``Debug``
 - ``RelWithDebInfo``
 - ``Release``
 - ``MinSizeRel``

``Release`` is default.

Debug symbols are added by default to all builds, but can be removed
from ``Release`` and ``MinSizeRel`` builds by setting
``TFM_DEBUG_SYMBOLS`` to ``OFF``.

``RelWithDebInfo``, ``Release`` and ``MinSizeRel`` all have different
optimizations turned on and hence will produce smaller, faster code
than ``Debug``. ``MinSizeRel`` will produce the smallest code, and
hence is often a good idea on RAM or flash constrained systems.

Other cmake parameters
----------------------

The full list of default options is in ``config/config_default.cmake``. Several
important options are listed below.


+---------------------+----------------------------------------+---------------+
| Parameter           | Description                            | Default value |
+=====================+========================================+===============+
| BL2                 | Build level 2 secure bootloader.       | ON            |
+---------------------+----------------------------------------+---------------+
| NS                  | Build NS app. Required for test code.  | ON            |
+---------------------+----------------------------------------+---------------+
| TFM_LIB_MODEL       | Use secure library model instead of    | OFF           |
|                     | PSA api (IPC model).                   |               |
+---------------------+----------------------------------------+---------------+
| TFM_ISOLATION_LEVEL | Set TFM isolation level.               | 1             |
+---------------------+----------------------------------------+---------------+
| TFM_PROFILE         | Set TFM profile.                       |               |
+---------------------+----------------------------------------+---------------+
| TEST_S              | Build secure regression tests.         | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_NS             | Build non-secure regression tests.     | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_PSA_API        | Build PSA API TESTS for the given      |               |
|                     | suite. Takes a PSA api ``SUITE`` as an |               |
|                     | argument (``CRYPTO`` etc).             |               |
+---------------------+----------------------------------------+---------------+

Regression test configuration
-----------------------------

Regression test configuration is controlled entirely by ``TEST_NS`` or
``TEST_S`` or single test suite configuration. The group test
configurations and single test suite configurations are listed below, all of
them are disabled by default.

+---------------------+--------------------------------------------------------------------+
| Parameter           | Description                                                        |
+=====================+====================================================================+
| TEST_NS_ATTESTATION | Build non-secure regression Attestation tests.                     |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_T_COSE      | Build non-secure regression t_cose tests.                          |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_QCBOR       | Build non-secure regression QCBOR tests.                           |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_AUDIT       | Build non-secure regression Audit log tests.                       |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_CORE        | Build non-secure regression Core tests.                            |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_CRYPTO      | Build non-secure regression Crypto tests.                          |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_ITS         | Build non-secure regression ITS tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_PS          | Build non-secure regression PS tests.                              |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_PLATFORM    | Build non-secure regression Platform tests.                        |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_FWU         | Build non-secure regression FWU tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_IPC         | Build non-secure regression IPC tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_SLIH_IRQ    | Build non-secure regression Second-Level Interrupt Handling tests. |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_FLIH_IRQ    | Build non-secure regression First-Level Interrupt Handling tests.  |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_MULTI_CORE  | Build non-secure regression multi-core tests.                      |
+---------------------+--------------------------------------------------------------------+
| TEST_S_ATTESTATION  | Build secure regression Attestation tests.                         |
+---------------------+--------------------------------------------------------------------+
| TEST_S_AUDIT        | Build secure regression Audit log tests.                           |
+---------------------+--------------------------------------------------------------------+
| TEST_S_CRYPTO       | Build secure regression Crypto tests.                              |
+---------------------+--------------------------------------------------------------------+
| TEST_S_ITS          | Build secure regression ITS tests.                                 |
+---------------------+--------------------------------------------------------------------+
| TEST_S_PS           | Build secure regression PS tests.                                  |
+---------------------+--------------------------------------------------------------------+
| TEST_S_PLATFORM     | Build secure regression Platform tests.                            |
+---------------------+--------------------------------------------------------------------+
| TEST_S_FWU          | Build secure regression FWU tests.                                 |
+---------------------+--------------------------------------------------------------------+
| TEST_S_IPC          | Build secure regression IPC tests.                                 |
+---------------------+--------------------------------------------------------------------+

Individual test suites can be enabled when their dependencies like partitions or
other specific configurations are set. On the one hand, some test suites depend
on other test suites. On the other hand, some test suites conflict with
other test suites. Test configurations and dependencies will be
checked in ``${TFM_TEST_REPO_PATH}/test/config/check_config.cmake``.

If regression testing is enabled by ``TEST_NS`` or ``TEST_S``, individual
test suites will be enabled or disabled as appropriate for the TF-M
configuration (i.e. all enabled secure partitions will be tested).

Some cryptographic tests can be enabled and disabled. This is done to prevent
false failures from being reported when a smaller Mbed Crypto config is being
used which does not support all features.

+-----------------------------+-------------------------------------+---------------+
| Parameter                   | Description                         | Default value |
+=============================+=====================================+===============+
| TFM_CRYPTO_TEST_ALG_CBC     | Test CBC cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CCM     | Test CCM cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CFB     | Test CFB cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_ECB     | Test ECB cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CTR     | Test CTR cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_OFB     | Test OFB cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_GCM     | Test GCM cryptography mode          | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_SHA_512 | Test SHA-512 cryptography algorithm | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_HKDF        | Test HKDF key derivation algorithm  | ON            |
+-----------------------------+-------------------------------------+---------------+
| TFM_CRYPTO_TEST_ECDH        | Test ECDH key agreement algorithm   | ON            |
+-----------------------------+-------------------------------------+---------------+

TF-M Profiles
-------------

TF-M Profiles are implemented as a single cmake configuration file, under the
``config/profile`` directory. A good understanding can be gained quickly by
looking at the Profile configuration files, but the ultimate reference for
Profiles are the design documents in the ``docs/technical_references/profiles/``
directory.

PSA test configuration
----------------------

PSA tests are configured by using the ``TEST_PSA_API`` cmake variable. The
variable should be set to the name of the test suite that is desired. It is
_not_ supported to set both ``TEST_PSA_API`` and ``TEST_S`` or ``TEST_NS``.

The Functional API tests are:
 - ``CRYPTO``
 - ``INITIAL_ATTESTATION``
 - ``STORAGE`` (INTERNAL_TRUSTED_STORAGE and PROTECTED_STORAGE)
 - ``INTERNAL_TRUSTED_STORAGE``
 - ``PROTECTED_STORAGE``

The Firmware Framework test suites are:
 - ``IPC``

Note that these map directly to the ``SUITE`` cmake variable used in the
psa-arch-tests documentation.

.. _Migration from legacy buildsystem:

Migration from legacy buildsystem
---------------------------------

The previous (legacy) cmake buildsystem made use of separate configuration
files, where now build options are controlled by variables. For ease of
transition, a table below is provided that maps the legacy files to the current
variables, in the format of cmake command line parameters.

+------------------------------------------+---------------------------------------+
| File                                     | Cmake command line                    |
+==========================================+=======================================+
| ConfigDefault.cmake                      | -DTFM_LIB_MODEL=ON                    |
+------------------------------------------+---------------------------------------+
| ConfigCoreIPC.cmake                      | <no options>                          |
+------------------------------------------+---------------------------------------+
| ConfigCoreIPCTfmLevel2.cmake             | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileS.cmake              | -DTFM_PROFILE=profile_small           |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileM.cmake              | -DTFM_PROFILE=profile_medium          |
+------------------------------------------+---------------------------------------+
| ConfigRegression.cmake                   | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigRegressionIPC.cmake                | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigRegressionIPCTfmLevel2.cmake       | -DTEST_NS=ON -DTEST_S=ON              |
|                                          | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigRegressionProfileS.cmake           | -DTFM_PROFILE=profile_small           |
|                                          | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigRegressionProfileM.cmake           | -DTFM_PROFILE=profile_medium          |
|                                          | -DTEST_NS=ON -DTEST_S=ON              |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTest.cmake                   | -DTEST_PSA_API=<test_suite>           |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTestIPC.cmake                | -DTEST_PSA_API=<test_suite>           |
+------------------------------------------+---------------------------------------+
| ConfigPsaApiTestIPCTfmLevel2.cmake       | -DTEST_PSA_API=<test_suite>           |
|                                          | -DTFM_ISOLATION_LEVEL=2               |
+------------------------------------------+---------------------------------------+
| ConfigDefaultProfileM.cmake              | -DTFM_PROFILE=profile_medium          |
| + profile_m_config_ext_ps_disabled.cmake | -DTFM_PARTITION_PROTECTED_STORAGE=OFF |
+------------------------------------------+---------------------------------------+

There has also been some changes to the PSA manifest file generation. The files
are now generated into a separate tree in the ``<tfm build dir>/generated``
directory. Therefore they have been removed from the source tree. Any changes
should be made only to the template files.

The API for the ``tools/tfm_parse_manifest_list.py`` script has also changed
slightly. It is no longer required to be run manually as it is run as part of
cmake.

*******************
TF-M build examples
*******************

Example: building TF-M for AN521 platform using GCC:
====================================================
.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521
    make install

.. Note::

    It is recommended to build each different build configuration in a separate
    build directory.

The default build uses Unix Makefiles. The ``-G`` option can be used to change
this. The default build uses the GNU ARM toolchain and creates a Release build.
These options can be overridden using the ``TFM_TOOLCHAIN_FILE`` and
``CMAKE_BUILD_TYPE`` parameters, as shown below

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -GNinja -DTFM_TOOLCHAIN_FILE=toolchain_ARMCLANG.cmake -DCMAKE_BUILD_TYPE=Debug
    cmake --build cmake_build -- install

Regression Tests for the AN521 target platform
==============================================

Regression tests can be build by using the TEST_S and TEST_NS settings. Either
can be used in isolation or both can be used to enable both suites. All tests
for all enabled partitions are run, along with IPC and Multicore tests if those
features are enabled.

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTEST_S=ON -DTEST_NS=ON
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTEST_S=ON -DTEST_NS=ON
    make install

Build for PSA API tests
=======================
The build system provides support for building and integrating the PSA API tests
from https://github.com/ARM-software/psa-arch-tests. PSA API tests are
controlled using the TEST_PSA_API variable. Enabling both regression tests and
PSA API tests simultaneously is **not** supported.

The value of the TEST_PSA_API variable is the suite to be run.

.. code-block:: bash

    -DTEST_PSA_API=INTERNAL_TRUSTED_STORAGE
    -DTEST_PSA_API=PROTECTED_STORAGE
    -DTEST_PSA_API=STORAGE
    -DTEST_PSA_API=CRYPTO
    -DTEST_PSA_API=INITIAL_ATTESTATION

Respectively for the corresponding service. For example, to enable the PSA API
tests for the Crypto service:

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTEST_PSA_API=CRYPTO
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTEST_PSA_API=CRYPTO
    make install

Location of build artifacts
===========================

All build artifacts are provided in the ``<build_dir>/bin`` directory. It is
**not** required to run ``make install`` to generate artifacts in this location.


For the purposes of maintaining compatibility with the legacy cmake build
system, they are also provided in
``<build_dir>/install/outputs/<target_platform>/``. In order to generate the
artifacts in this location ``make install`` must be run.

****************************
Manual dependency management
****************************

The TF-M build system will by default fetch all dependencies with appropriate
versions and store them inside the build tree. In this case, the build tree
location is ``<build_dir>/lib/ext``, and the extra libraries can be cleaned by
deleting that directory.

If you have local copies already, and wish to avoid having the libraries
downloaded every time the build directory is deleted, then the following
variables can be set to the path to the root directory of the local repo. This
will disable the automatic downloading for that dependency.

+----------------+---------------------+-----------------------------------------------------+
| Dependency     | Cmake variable      | Git repo URL                                        |
+================+=====================+=====================================================+
| Mbed Crypto    | MBEDCRYPTO_PATH     | https://github.com/ARMmbed/mbedtls                  |
+----------------+---------------------+-----------------------------------------------------+
| tf-m-tests     | TFM_TEST_REPO_PATH  | https://git.trustedfirmware.org/TF-M/tf-m-tests.git |
+----------------+---------------------+-----------------------------------------------------+
| MCUboot        | MCUBOOT_PATH        | https://github.com/mcu-tools/mcuboot                |
+----------------+---------------------+-----------------------------------------------------+
| psa-arch-tests | PSA_ARCH_TESTS_PATH | https://github.com/ARM-software/psa-arch-tests      |
+----------------+---------------------+-----------------------------------------------------+

For required versions of the dependencies, refer to ``config/config_default.cmake``.

.. Note::
 - Some patches are required to the mbedtls repo to allow building it as part of
   TF-M. While these patches are being upstreamed they are stored in
   ``lib/ext/mbedcrypo``. In order to use a local copy of Mbed Crypto it is
   required to apply all patch files in this directory.

.. Note::
 - CMSIS 5 is provided by the TF-M tests repo. If you wish to use a different
   source for CMSIS 5, it can be configured using CMSIS_5_PATH.

TF-M Tests
==========

Dependency auto downloading is used by default.
The TF-M build system downloads the tf-m-tests repo with a fixed version
specified by ``TFM_TEST_REPO_VERSION`` in
:file:`lib/ext/tf-m-tests/repo_config_default.cmake`.
The version can be a release tag or a commit hash.

Developers who want a different version of tf-m-tests can override
``TFM_TEST_REPO_PATH`` to a local copy with the desired version.

As the test repo is part of the TF-M project and coupled with TF-M repo,
the version should be updated when there are dependency changes between the TF-M
repo and the test repo and when there is a complete change merged in test repo.

A complete change is one or more patches that are for the same purpose, for
example a new test suite or enhancements on the test cases.
Patches in one change can be merge individually provided they do not break
anything or cause any regressions.
But the version in the TF-M gets updated only when all the patches are merged.

Example: building TF-M for AN521 platform with local Mbed Crypto
================================================================

Prepare Mbed Crypto repository
------------------------------

This is only required to be done once. For dependencies that do not have any
``.patch`` files in their ``lib/ext`` directory the only required step is
cloning the repo and checking out the correct branch.

.. code-block:: bash

    cd <Mbed Crypto base folder>
    git clone https://github.com/ARMmbed/mbedtls
    cd mbedtls
    git checkout <MBEDCRYPTO_VERSION from config_default.cmake>
    git apply <TF-M base folder>/trusted-firmware-m/lib/ext/mbedcrypo/*.patch

.. Note::
 - <Mbed Crypto base folder> does not need to have any fixed posisition related
   to the TF-M repo.

Build TF-M
----------

With new cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    make install

--------------

*Copyright (c) 2017-2022, Arm Limited. All rights reserved.*
*Copyright (c) 2022, Cypress Semiconductor Corporation. All rights reserved.*
