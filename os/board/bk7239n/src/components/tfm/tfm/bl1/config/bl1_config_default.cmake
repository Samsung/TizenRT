#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_BL2_ENCRYPTION_KEY_PATH         ${CMAKE_SOURCE_DIR}/bl1/bl1_2/bl2_dummy_encryption_key.bin CACHE STRING "Path to binary key to use for encrypting BL2")
set(TFM_BL2_IMAGE_FLASH_AREA_NUM        0           CACHE STRING    "Which flash area BL2 is stored in")

set(TFM_BL1_MEMORY_MAPPED_FLASH         ON          CACHE BOOL      "Whether BL1 can directly access flash content")

set(TFM_BL1_LOGGING                     ON          CACHE BOOL      "Whether BL1 will log to uart")
set(TFM_BL1_DEFAULT_OTP                 ON          CACHE BOOL      "Whether BL1_1 will use default OTP memory")
set(TFM_BL1_DEFAULT_PROVISIONING        ON          CACHE BOOL      "Whether BL1_1 will use default provisioning")
set(TFM_BL1_SOFTWARE_CRYPTO             ON          CACHE BOOL      "Whether BL1_1 will use software crypto")
set(TFM_BL1_DUMMY_TRNG                  ON          CACHE BOOL      "Whether BL1_1 will use dummy TRNG")

set(TFM_BL1_IMAGE_VERSION_BL2           "1.9.0+0"   CACHE STRING    "Image version of BL2 image")
set(TFM_BL1_IMAGE_SECURITY_COUNTER_BL2  1           CACHE STRING    "Security counter value to include with BL2 image")

set(TFM_BL1_2_IN_OTP                    TRUE        CACHE BOOL      "Whether BL1_2 is stored in OTP")

set(BL1_HEADER_SIZE                     0x800       CACHE STRING    "BL1 Header size")
set(BL1_TRAILER_SIZE                    0x000       CACHE STRING    "BL1 Trailer size")

