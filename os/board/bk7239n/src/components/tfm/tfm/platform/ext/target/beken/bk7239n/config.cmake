#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_EXTRA_GENERATED_FILE_LIST_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/beken/generated_file_list.yaml  CACHE PATH "Path to extra generated file list. Appended to stardard TFM generated file list." FORCE)

set(TFM_BEKEN  ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/beken/  CACHE PATH "Path to beken" FORCE)
set(TFM_SOC  bk7239n  CACHE STRING "bk7239n" FORCE)

set(PLATFORM_SLIH_IRQ_TEST_SUPPORT    ON    CACHE BOOL    "Platform supports SLIH IRQ tests")
set(PLATFORM_FLIH_IRQ_TEST_SUPPORT    ON    CACHE BOOL    "Platform supports FLIH IRQ tests")

# Make FLIH IRQ test as the default IRQ test on BK7236
set(TEST_NS_SLIH_IRQ                 OFF   CACHE BOOL    "Whether to build NS regression Second-Level Interrupt Handling tests")

set(CRYPTO_HW_ACCELERATOR            ON         CACHE BOOL      "Whether to enable the crypto hardware accelerator on supported platforms")
set(CRYPTO_NV_SEED                   OFF        CACHE BOOL      "Use stored NV seed to provide entropy")
set(BL2                              ON         CACHE BOOL      "Whether to build BL2")
set(DEFAULT_MCUBOOT_FLASH_MAP        OFF        CACHE BOOL      "Whether to use the default flash map defined by TF-M project")
set(FPGA                             OFF        CACHE BOOL      "Build for FPGA")

if(TEST_S)
set(TFM_PARTITION_FIRMWARE_UPDATE    OFF        CACHE BOOL      "Enable firmware update partition")
set(MCUBOOT_DATA_SHARING             OFF        CACHE BOOL      "Enable Data Sharing")
set(MCUBOOT_MEASURED_BOOT            ON         CACHE BOOL      "Add boot measurement values to boot status. Used for initial attestation token")
else()
set(TFM_PARTITION_FIRMWARE_UPDATE    ON         CACHE BOOL      "Enable firmware update partition")
set(MCUBOOT_DATA_SHARING             ON         CACHE BOOL      "Enable Data Sharing")
set(MCUBOOT_MEASURED_BOOT            ON         CACHE BOOL      "Add boot measurement values to boot status. Used for initial attestation token")
endif()

set(MCUBOOT_IMAGE_NUMBER             1          CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each seperately")

if(TFM_PARTITION_FIRMWARE_UPDATE)
set(CONFIG_TFM_FP                     soft  CACHE STRING "Whether to support float, and set the type")
set(CONFIG_TFM_FP_ARCH                fpv5-sp-d16 CACHE STRING "float processing unit arch version")
set(MCUBOOT_DATA_SHARING             ON         CACHE BOOL      "Enable Data Sharing")
set(PSA_FWU_MAX_BLOCK_SIZE           2048       CACHE STRING    "The maximum permitted size for block in psa_fwu_write, in bytes.")
set(NS                               OFF        CACHE BOOL      "Whether to build NS app")

# Disable MCUBOOT measured boot for FAST BOOT, otherwise FAST BOOT mode asserted in TFM
set(MCUBOOT_MEASURED_BOOT            OFF          CACHE BOOL      "Add boot measurement values to boot status. Used for initial attestation token")
endif()

set(PLATFORM_DEFAULT_ROTPK           OFF        CACHE BOOL      "Use default root of trust public key.")

set(PLATFORM_DEFAULT_NV_COUNTERS     OFF          CACHE BOOL      "Use default nv counter implementation.")

set(PLATFORM_DEFAULT_OTP             OFF        CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
set(PLATFORM_DEFAULT_OTP_WRITEABLE   ON         CACHE BOOL      "Use OTP memory with write support")
set(TFM_DUMMY_PROVISIONING           OFF        CACHE BOOL      "Provision with dummy values. NOT to be used in production")
set(PS_NUM_ASSETS                    "20"      CACHE STRING    "The maximum number of assets to be stored in the Protected Storage area")
set(PS_MAX_ASSET_SIZE                "1036"      CACHE STRING    "The maximum asset size to be stored in the Protected Storage area")
set(ITS_MAX_ASSET_SIZE               512       CACHE STRING    "The maximum asset size to be stored in the Internal Trusted Storage area")
set(ITS_BUF_SIZE                     1024      CACHE STRING    "Size of the ITS internal data transfer buffer (defaults to ITS_MAX_ASSET_SIZE if not set)")
set(ITS_NUM_ASSETS                   "20"        CACHE STRING    "The maximum number of assets to be stored in the Internal Trusted Storage area")

if(BL2)
    #At least 1 flash sector for header/trailer
    set(BL2_HEADER_SIZE  0x1000 CACHE STRING "Header size")
    set(BL2_TRAILER_SIZE 0x1000 CACHE STRING "Trailer size")
else()
    #No header if no bootloader, but keep IMAGE_CODE_SIZE the same
    set(BL2_TRAILER_SIZE 0x2000 CACHE STRING "Trailer size")
endif()

include(platform/ext/target/${TFM_PLATFORM}/repo_fetch.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCONFIG_BUILD_DEBUG)
endif()

if(FPGA)
    add_definitions(-DCONFIG_FPGA)
endif()
