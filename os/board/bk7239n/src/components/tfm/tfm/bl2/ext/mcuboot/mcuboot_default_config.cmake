#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## MCUBoot #############################################

set(TEST_BL2                            OFF         CACHE BOOL      "Whether to build bl2 tests")

set(DEFAULT_MCUBOOT_SECURITY_COUNTERS   ON          CACHE BOOL      "Whether to use the default security counter configuration defined by TF-M project")
set(DEFAULT_MCUBOOT_FLASH_MAP           ON          CACHE BOOL      "Whether to use the default flash map defined by TF-M project")

set(MCUBOOT_S_IMAGE_FLASH_AREA_NUM      0           CACHE STRING    "ID of the flash area containing the primary Secure image")
set(MCUBOOT_NS_IMAGE_FLASH_AREA_NUM     1           CACHE STRING    "ID of the flash area containing the primary Non-Secure image")

set(MCUBOOT_IMAGE_NUMBER                2           CACHE STRING    "Whether to combine S and NS into either 1 image, or sign each seperately")
set(MCUBOOT_EXECUTION_SLOT              1           CACHE STRING    "Slot from which to execute the image, used for XIP mode")
set(MCUBOOT_LOG_LEVEL                   "INFO"      CACHE STRING    "Level of logging to use for MCUboot [OFF, ERROR, WARNING, INFO, DEBUG]")
set(MCUBOOT_HW_KEY                      ON          CACHE BOOL      "Whether to embed the entire public key in the image metadata instead of the hash only")
set(MCUBOOT_UPGRADE_STRATEGY            "OVERWRITE_ONLY" CACHE STRING "Upgrade strategy for images")
set(BL2_HEADER_SIZE                     0x400       CACHE STRING    "Header size")
set(BL2_TRAILER_SIZE                    0x400       CACHE STRING    "Trailer size")
set(MCUBOOT_ALIGN_VAL                   1           CACHE STRING    "align option for mcuboot and build image with imgtool [1, 2, 4, 8, 16, 32]")

# Specifying a scope of the accepted values of MCUBOOT_UPGRADE_STRATEGY for
# platforms to choose a specific upgrade strategy for images. These certain
# configurations will be used to facilitate the later validation.
set_property(CACHE MCUBOOT_UPGRADE_STRATEGY PROPERTY STRINGS "OVERWRITE_ONLY;SWAP_USING_SCRATCH;SWAP_USING_MOVE;DIRECT_XIP;RAM_LOAD")

# Specifying a scope of the accepted values of MCUBOOT_ALIGN_VAL for
# platforms requiring specific flash alignmnent
set_property(CACHE MCUBOOT_ALIGN_VAL PROPERTY STRINGS "1;2;4;8;16;32")

set(MCUBOOT_DIRECT_XIP_REVERT           OFF          CACHE BOOL      "Enable the revert mechanism in direct-xip mode")
set(MCUBOOT_MEASURED_BOOT               ON          CACHE BOOL      "Add boot measurement values to boot status. Used for initial attestation token")
if(CONFIG_XIP_NO_VERSION)
set(MCUBOOT_HW_ROLLBACK_PROT            OFF          CACHE BOOL      "Enable security counter validation against non-volatile HW counters")
else()
set(MCUBOOT_HW_ROLLBACK_PROT            ON          CACHE BOOL      "Enable security counter validation against non-volatile HW counters")
endif()
set(MCUBOOT_ENC_IMAGES                  OFF         CACHE BOOL      "Enable encrypted image upgrade support")
set(MCUBOOT_ENCRYPT_RSA                 OFF         CACHE BOOL      "Use RSA for encrypted image upgrade support")
set(MCUBOOT_FIH_PROFILE                 OFF         CACHE STRING    "Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]")

# Note - If either SIGNATURE_TYPE or KEY_LEN are changed, the entries for KEY_S
# and KEY_NS will either have to be updated manually or removed from the cache.
# `cmake .. -UMCUBOOT_KEY_S -UMCUBOOT_KEY_NS`. Once removed from the cache it
# will be set to default again.
set(MCUBOOT_SIGNATURE_TYPE              "RSA"       CACHE STRING    "Algorithm to use for signature validation")
set(MCUBOOT_SIGNATURE_KEY_LEN           3072        CACHE STRING    "Key length to use for signature validation")
set(MCUBOOT_KEY_S                       "${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}.pem" CACHE FILEPATH "Path to key with which to sign secure binary")
set(MCUBOOT_KEY_NS                      "${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}_1.pem" CACHE FILEPATH "Path to key with which to sign non-secure binary")

set(MCUBOOT_IMAGE_VERSION_S             ${TFM_VERSION} CACHE STRING "Version number of S image")
set(MCUBOOT_IMAGE_VERSION_NS            0.0.0       CACHE STRING    "Version number of NS image")
set(MCUBOOT_SECURITY_COUNTER_S          1           CACHE STRING    "Security counter for S image. auto sets it to IMAGE_VERSION_S")
set(MCUBOOT_SECURITY_COUNTER_NS         1           CACHE STRING    "Security counter for NS image. auto sets it to IMAGE_VERSION_NS")
set(MCUBOOT_S_IMAGE_MIN_VER             0.0.0+0     CACHE STRING    "Minimum version of secure image required by the non-secure image for upgrade to this non-secure image. If MCUBOOT_IMAGE_NUMBER == 1 this option has no effect")
set(MCUBOOT_NS_IMAGE_MIN_VER            0.0.0+0     CACHE STRING    "Minimum version of non-secure image required by the secure image for upgrade to this secure image. If MCUBOOT_IMAGE_NUMBER == 1 this option has no effect")
set(MCUBOOT_ENC_KEY_LEN_S               128         CACHE STRING    "Length of the AES key for encrypt S image")
set(MCUBOOT_ENC_KEY_LEN_NS              128         CACHE STRING    "Length of the AES key for encrypt NS image")
set(MCUBOOT_MBEDCRYPTO_CONFIG_FILEPATH  "${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/config/mcuboot-mbedtls-cfg.h" CACHE FILEPATH "Mbedtls config file to use with MCUboot")
