#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_TOOLCHAIN_FILE                  ${CMAKE_SOURCE_DIR}/toolchain_GNUARM.cmake CACHE FILEPATH    "Path to TFM compiler toolchain file")
set(TFM_PLATFORM                        ""          CACHE STRING    "Platform to build TF-M for. Must be either a relative path from [TF-M]/platform/ext/target, or an absolute path.")
set(CROSS_COMPILE                       arm-none-eabi CACHE STRING  "Cross-compilation triplet")

set(BL1                                 OFF         CACHE BOOL      "Whether to build BL1")
set(BL2                                 ON          CACHE BOOL      "Whether to build BL2")
set(NS                                  ON          CACHE BOOL      "Whether to build NS app")

set(TEST_S                              OFF         CACHE BOOL      "Whether to build S regression tests")
set(TEST_NS                             OFF         CACHE BOOL      "Whether to build NS regression tests")
set(TEST_PSA_API                        ""          CACHE STRING    "Which (if any) of the PSA API tests should be compiled")
set(TEST_BL1_1                          OFF         CACHE BOOL      "Whether to build BL1_1 tests")
set(TEST_BL1_2                          OFF         CACHE BOOL      "Whether to build BL1_2 tests")

# TFM_LIB_MODEL is the only user configuration for Library Model selection.
# TFM_PSA_API becomes an internal variable. Please do NOT use it in build command line.
set(TFM_LIB_MODEL                       OFF         CACHE BOOL      "Use secure library model instead of PSA API (IPC model)")
set(TFM_ISOLATION_LEVEL                 1           CACHE STRING    "Isolation level")
set(PSA_FRAMEWORK_HAS_MM_IOVEC          OFF         CACHE BOOL      "Enable MM-IOVEC")
set(TFM_PROFILE                         ""          CACHE STRING    "Profile to use")
set(TFM_FIH_PROFILE                     OFF         CACHE STRING    "Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]")
set(CONFIG_TFM_CONN_HANDLE_MAX_NUM      8           CACHE STRING    "The maximal number of secure services that are connected or requested at the same time")
set(CONFIG_TFM_SPM_BACKEND              "IPC"       CACHE STRING    "The SPM backend [IPC, SFN]")

# An NSPE client_id is provided by the NSPE OS via the SPM or directly by the SPM.
# When `TFM_NS_MANAGE_NSID` is `ON`, TF-M supports NSPE OS providing NSPE client_id.
set(TFM_NS_MANAGE_NSID                  OFF         CACHE BOOL      "Support NSPE OS providing NSPE client_id")

set(TFM_EXTRA_CONFIG_PATH               ""          CACHE PATH      "Path to extra cmake config file")

set(TFM_MANIFEST_LIST                   ${CMAKE_SOURCE_DIR}/tools/tfm_manifest_list.yaml CACHE FILEPATH "TF-M native Secure Partition manifests list file")
set(TFM_EXTRA_MANIFEST_LIST_FILES       ""          CACHE FILEPATH  "Extra manifest list file(s), used to list extra Secure Partition manifests.")
set(TFM_EXTRA_GENERATED_FILE_LIST_PATH  ""          CACHE PATH      "Path to extra generated file list. Appended to stardard TFM generated file list.")
set(TFM_EXTRA_PARTITION_PATHS           ""          CACHE PATH      "List of extra Secure Partitions directories. An extra Secure Parition folder contains source code, CMakeLists.txt and manifest files")

set(TFM_SPM_LOG_LEVEL                   TFM_SPM_LOG_LEVEL_INFO          CACHE STRING    "Set default SPM log level as INFO level")
set(TFM_PARTITION_LOG_LEVEL             TFM_PARTITION_LOG_LEVEL_INFO    CACHE STRING    "Set default Secure Partition log level as INFO level")

set(TFM_CODE_SHARING                    OFF         CACHE PATH      "Enable code sharing between MCUboot and secure firmware")

set(TFM_INSTALL_PATH                    ${CMAKE_BINARY_DIR}/install CACHE PATH "Path to which to install TF-M files")

set(TFM_DEBUG_SYMBOLS                   ON          CACHE BOOL      "Add debug symbols. Note that setting CMAKE_BUILD_TYPE to Debug or RelWithDebInfo will also add debug symbols.")
set(TFM_CODE_COVERAGE                   OFF         CACHE BOOL      "Whether to build the binary for lcov tools")

set(TFM_PXN_ENABLE                      OFF         CACHE BOOL      "Use Privileged execute never (PXN)")

set(TFM_EXCEPTION_INFO_DUMP             ON          CACHE BOOL      "On fatal errors in the secure firmware, capture info about the exception. Print the info if the SPM log level is sufficient.")

set(CONFIG_TFM_HALT_ON_CORE_PANIC       OFF         CACHE BOOL       "On fatal errors in the secure firmware, halt instead of rebooting.")

set(CONFIG_TFM_FP                       "soft"      CACHE STRING    "FP ABI type in SPE and NSPE: soft-Software ABI, hard-Hardware ABI")
set(CONFIG_TFM_LAZY_STACKING            OFF         CACHE BOOL      "Enable/disable lazy stacking")

set(CONFIG_TFM_DOORBELL_API             ON          CACHE BOOL      "Enable the doorbell APIs")

############################ Platform ##########################################

set(TFM_MULTI_CORE_TOPOLOGY             OFF         CACHE BOOL      "Whether to build for a dual-cpu architecture")
set(NUM_MAILBOX_QUEUE_SLOT              1           CACHE BOOL      "Number of mailbox queue slots")
set(TFM_PLAT_SPECIFIC_MULTI_CORE_COMM   OFF         CACHE BOOL      "Whether to use a platform specific inter-core communication instead of mailbox in dual-cpu topology")

set(DEBUG_AUTHENTICATION                CHIP_DEFAULT CACHE STRING   "Debug authentication setting. [CHIP_DEFAULT, NONE, NS_ONLY, FULL")
set(SECURE_UART1                        OFF         CACHE BOOL      "Enable secure UART1")

set(CRYPTO_HW_ACCELERATOR               OFF         CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")

set(OTP_NV_COUNTERS_RAM_EMULATION       OFF         CACHE BOOL      "Enable OTP/NV_COUNTERS emulation in RAM. Has no effect on non-default implementations of the OTP and NV_COUNTERS")

set(PLATFORM_DEFAULT_BL1                ON          CACHE STRING    "Whether to use default BL1 or platform-specific one")

set(PLATFORM_DEFAULT_ATTEST_HAL         ON          CACHE BOOL      "Use default attest hal implementation.")
set(PLATFORM_DEFAULT_NV_COUNTERS        ON          CACHE BOOL      "Use default nv counter implementation.")
set(PLATFORM_DEFAULT_CRYPTO_KEYS        ON          CACHE BOOL      "Use default crypto keys implementation.")
set(PLATFORM_DEFAULT_ROTPK              ON          CACHE BOOL      "Use default root of trust public key.")
set(PLATFORM_DEFAULT_IAK                ON          CACHE BOOL      "Use default initial attestation_key.")
set(PLATFORM_DEFAULT_UART_STDOUT        ON          CACHE BOOL      "Use default uart stdout implementation.")
set(PLATFORM_DEFAULT_NV_SEED            ON          CACHE BOOL      "Use default NV seed implementation.")
set(PLATFORM_DEFAULT_OTP                ON          CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
set(PLATFORM_DEFAULT_OTP_WRITEABLE      ON          CACHE BOOL      "Use OTP memory with write support")
set(PLATFORM_DEFAULT_PROVISIONING       ON          CACHE BOOL      "Use default provisioning implementation")

set(TFM_DUMMY_PROVISIONING              ON          CACHE BOOL      "Provision with dummy values. NOT to be used in production")
set(PLATFORM_IS_FVP                     FALSE       CACHE BOOL      "Whether to enable FVP or FPGA build of the platform.")

set(PLATFORM_PSA_ADAC_SECURE_DEBUG      FALSE       CACHE BOOL      "Whether to use psa-adac secure debug.")
set(PLATFORM_PSA_ADAC_SOURCE_PATH       "DOWNLOAD"  CACHE PATH      "Path to source dir of psa-adac.")
set(PLATFORM_PSA_ADAC_VERSION           "427923cc0152578d536fb2065154d5d0dd874910" CACHE STRING "The version of psa-adac to use.")

set(BL1_HEADER_SIZE                     0x000       CACHE STRING    "BL1 Header size")
set(BL1_TRAILER_SIZE                    0x000       CACHE STRING    "BL1 Trailer size")

set(BL2_HEADER_SIZE                     0x000       CACHE STRING    "BL2 Header size")
set(BL2_TRAILER_SIZE                    0x000       CACHE STRING    "BL2 Trailer size")

############################ Partitions ########################################
set(TFM_PARTITION_PROTECTED_STORAGE     ON          CACHE BOOL      "Enable Protected Storage partition")
set(PS_CREATE_FLASH_LAYOUT              ON          CACHE BOOL      "Create flash FS if it doesn't exist for Protected Storage partition")
set(PS_ENCRYPTION                       ON          CACHE BOOL      "Enable encryption for Protected Storage partition")
set(PS_RAM_FS                           OFF         CACHE BOOL      "Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition")
set(PS_ROLLBACK_PROTECTION              ON          CACHE BOOL      "Enable rollback protection for Protected Storage partition")
set(PS_VALIDATE_METADATA_FROM_FLASH     ON          CACHE BOOL      "Validate filesystem metadata every time it is read from flash")
set(PS_MAX_ASSET_SIZE                   "2048"      CACHE STRING    "The maximum asset size to be stored in the Protected Storage area")
set(PS_NUM_ASSETS                       "10"        CACHE STRING    "The maximum number of assets to be stored in the Protected Storage area")
set(PS_CRYPTO_AEAD_ALG                  PSA_ALG_GCM CACHE STRING    "The AEAD algorithm to use for authenticated encryption in Protected Storage")

set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE ON       CACHE BOOL      "Enable Internal Trusted Storage partition")
set(ITS_CREATE_FLASH_LAYOUT             ON          CACHE BOOL      "Create flash FS if it doesn't exist for Internal Trusted Storage partition")
set(ITS_RAM_FS                          OFF         CACHE BOOL      "Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition")
set(ITS_VALIDATE_METADATA_FROM_FLASH    ON          CACHE BOOL      "Validate filesystem metadata every time it is read from flash")
set(ITS_MAX_ASSET_SIZE                  "512"       CACHE STRING    "The maximum asset size to be stored in the Internal Trusted Storage area")
set(ITS_NUM_ASSETS                      "10"        CACHE STRING    "The maximum number of assets to be stored in the Internal Trusted Storage area")
set(ITS_BUF_SIZE                        ""          CACHE STRING    "Size of the ITS internal data transfer buffer (defaults to ITS_MAX_ASSET_SIZE if not set)")

set(TFM_PARTITION_CRYPTO                ON          CACHE BOOL      "Enable Crypto partition")
# CRYPTO_ENGINE_BUF_SIZE needs to be >8KB for EC signing by attest module.
set(CRYPTO_ENGINE_BUF_SIZE              0x2080      CACHE STRING    "Heap size for the crypto backend")
set(CRYPTO_CONC_OPER_NUM                8           CACHE STRING    "The max number of concurrent operations that can be active (allocated) at any time in Crypto")
set(CRYPTO_RNG_MODULE_DISABLED          FALSE       CACHE BOOL      "Disable PSA Crypto random number generator module")
set(CRYPTO_KEY_MODULE_DISABLED          FALSE       CACHE BOOL      "Disable PSA Crypto Key module")
set(CRYPTO_AEAD_MODULE_DISABLED         FALSE       CACHE BOOL      "Disable PSA Crypto AEAD module")
set(CRYPTO_MAC_MODULE_DISABLED          FALSE       CACHE BOOL      "Disable PSA Crypto MAC module")
set(CRYPTO_HASH_MODULE_DISABLED         FALSE       CACHE BOOL      "Disable PSA Crypto Hash module")
set(CRYPTO_CIPHER_MODULE_DISABLED       FALSE       CACHE BOOL      "Disable PSA Crypto Cipher module")
set(CRYPTO_ASYM_SIGN_MODULE_DISABLED    FALSE       CACHE BOOL      "Disable PSA Crypto asymmetric key signature module")
set(CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED FALSE       CACHE BOOL      "Disable PSA Crypto asymmetric key encryption module")
set(CRYPTO_KEY_DERIVATION_MODULE_DISABLED FALSE     CACHE BOOL      "Disable PSA Crypto key derivation module")
#set(CRYPTO_IOVEC_BUFFER_SIZE            5120       CACHE STRING    "Default size of the internal scratch buffer used for PSA FF IOVec allocations note:this macro is configed under project config, and default value is 5120")
set(CRYPTO_NV_SEED                      ON          CACHE BOOL      "Use stored NV seed to provide entropy")

set(TFM_PARTITION_INITIAL_ATTESTATION   ON          CACHE BOOL      "Enable Initial Attestation partition")
set(SYMMETRIC_INITIAL_ATTESTATION       OFF         CACHE BOOL      "Use symmetric crypto for inital attestation")
set(ATTEST_INCLUDE_OPTIONAL_CLAIMS      ON          CACHE BOOL      "Include optional claims in initial attestation token")
set(ATTEST_INCLUDE_COSE_KEY_ID          OFF         CACHE BOOL      "Include COSE key-id in initial attestation token")

set(TFM_PARTITION_PLATFORM              ON          CACHE BOOL      "Enable Platform partition")

set(TFM_PARTITION_AUDIT_LOG             OFF         CACHE BOOL      "Enable Audit Log partition")

set(TFM_PARTITION_PSA_PROXY             OFF         CACHE BOOL      "Enable PSA Proxy partition")

set(TFM_PARTITION_SYSTEM_CLOCK          ON          CACHE BOOL      "Enable System CLOCK partition")

set(FORWARD_PROT_MSG                    OFF         CACHE BOOL      "Whether to forward all PSA RoT messages to a Secure Enclave")
set(TFM_PARTITION_FIRMWARE_UPDATE       OFF         CACHE BOOL      "Enable firmware update partition")
set(TFM_FWU_BOOTLOADER_LIB              "mcuboot"   CACHE STRING    "Bootloader configure file for Firmware Update partition")
set(PSA_FWU_MAX_BLOCK_SIZE              1024        CACHE STRING    "The maximum permitted size for block in psa_fwu_write, in bytes.")
set(TFM_FWU_BUF_SIZE                    ""          CACHE STRING    "Size of the FWU internal data transfer buffer (defaults to PSA_FWU_MAX_BLOCK_SIZE if not set)")

################################## Dependencies ################################

set(MBEDCRYPTO_PATH                     "DOWNLOAD"  CACHE PATH      "Path to Mbed Crypto (or DOWNLOAD to fetch automatically")
set(MBEDCRYPTO_VERSION                  "mbedtls-3.1.0" CACHE STRING "The version of Mbed Crypto to use")
set(MBEDCRYPTO_GIT_REMOTE               "https://github.com/Mbed-TLS/mbedtls.git" CACHE STRING "The URL (or path) to retrieve MbedTLS from.")
set(MBEDCRYPTO_BUILD_TYPE               "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type of Mbed Crypto library")
set(TFM_MBEDCRYPTO_CONFIG_PATH
  "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_default.h" CACHE PATH
  "Config to use for Mbed Crypto. For increased flexibility when pointing to a file, set the type \
of this setting to 'STRING' by passing the :<type> portion when specifying the setting value in \
the command line. E.g. '-DTFM_MBEDCRYPTO_CONFIG_PATH:STRING=some_file_which_is_generated.h' \
This can be useful if the config file is generated and placed inside a directory already added \
to the include path of mbedtls.")
set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH "" CACHE PATH "Config to append to standard Mbed Crypto config, used by platforms to cnfigure feature support")

set(MCUBOOT_PATH                        "DOWNLOAD"        CACHE PATH      "Path to MCUboot (or DOWNLOAD to fetch automatically")
set(MCUBOOT_VERSION                     "v1.9.0"  CACHE STRING    "The version of MCUboot to use")

set(PSA_ARCH_TESTS_PATH                 "DOWNLOAD"  CACHE PATH      "Path to PSA arch tests (or DOWNLOAD to fetch automatically")
set(PSA_ARCH_TESTS_VERSION              "f7e8495"   CACHE STRING    "The version of PSA arch tests to use")

################################################################################
################################################################################

# Specifying the accepted values for certain configuration options to facilitate
# their later validation.

########################## FIH #################################################

set_property(CACHE TFM_FIH_PROFILE PROPERTY STRINGS "OFF;LOW;MEDIUM;HIGH")

########################## FP #################################################

set_property(CACHE CONFIG_TFM_FP PROPERTY STRINGS "soft;hard")
