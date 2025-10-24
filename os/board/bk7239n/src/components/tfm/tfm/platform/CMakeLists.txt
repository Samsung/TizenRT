#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

add_library(platform_s STATIC)
add_library(platform_region_defs INTERFACE)
add_library(platform_common_interface INTERFACE)

# under the multicore topology, NS is declared in the same place as the PSA
# interface so that it picks up the compiler definitions for the NS CPU
if(NOT TFM_MULTI_CORE_TOPOLOGY)
    add_library(platform_ns STATIC EXCLUDE_FROM_ALL)
endif()

if (BL2)
    add_library(platform_bl2 STATIC)
endif()

if (BL1 AND PLATFORM_DEFAULT_BL1)
    add_library(platform_bl1 STATIC)
    add_library(platform_bl1_interface INTERFACE)
endif()

set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR})

#add_subdirectory(ext/target/${TFM_PLATFORM} target)
add_subdirectory(${TFM_BEKEN} target)

#========================= Platform Common interface ==========================#

target_include_directories(platform_common_interface
    INTERFACE
        ./ext
        ./ext/cmsis
        ./ext/common
        ./ext/driver
        ./include
)

#========================= Platform Secure ====================================#

target_include_directories(platform_s
    PUBLIC
        $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/accelerator/interface>
)

get_filename_component(BUILD_GEN_DIR "${CMAKE_BINARY_DIR}/_build" ABSOLUTE)

target_sources(platform_s
    PRIVATE
        $<$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/tfm_hal_ps.c>
        $<$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/tfm_hal_its.c>
        ext/common/tfm_platform.c
        $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/uart_stdout.c>
        $<$<BOOL:${TFM_SPM_LOG_RAW_ENABLED}>:ext/common/tfm_hal_spm_logdev_peripheral.c>
        $<$<BOOL:${TFM_EXCEPTION_INFO_DUMP}>:ext/common/exception_info.c>
        ext/common/faults.c
        ext/common/tfm_hal_memory_symbols.c
        $<$<BOOL:${PLATFORM_DEFAULT_ATTEST_HAL}>:ext/common/template/attest_hal.c>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
        $<$<AND:$<BOOL:${TFM_PARTITION_CRYPTO}>,$<BOOL:${PLATFORM_DEFAULT_CRYPTO_KEYS}>>:ext/common/template/crypto_keys.c>
        $<$<BOOL:${PLATFORM_DEFAULT_ROTPK}>:ext/common/template/tfm_rotpk.c>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_SEED}>:ext/common/template/crypto_nv_seed.c>
        $<$<AND:$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>,$<BOOL:${TEST_S_ATTESTATION}>>:ext/common/template/tfm_initial_attest_pub_key.c>
        $<$<OR:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/otp_flash.c>
        $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/nv_counters.c>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
        $<$<BOOL:${PLATFORM_DEFAULT_PROVISIONING}>:ext/common/provisioning.c>
        ${BUILD_GEN_DIR}/_otp.c
)

target_link_libraries(platform_s
    PUBLIC
        platform_common_interface
        platform_region_defs
        tfm_fih_headers
    PRIVATE
        psa_interface
        tfm_secure_api
        tfm_partition_defs
        tfm_spm
        $<$<BOOL:${PLATFORM_DEFAULT_ATTEST_HAL}>:tfm_sprt>
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:crypto_service_mbedcrypto>
)

target_compile_definitions(platform_s
    PUBLIC
        TFM_SPM_LOG_LEVEL=${TFM_SPM_LOG_LEVEL}
        $<$<BOOL:${TFM_SPM_LOG_RAW_ENABLED}>:TFM_SPM_LOG_RAW_ENABLED>
        $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
        $<$<BOOL:${TFM_EXCEPTION_INFO_DUMP}>:TFM_EXCEPTION_INFO_DUMP>
        # CONFIG_TFM_FP
        $<$<STREQUAL:${CONFIG_TFM_FP},hard>:CONFIG_TFM_FP=2>
        $<$<STREQUAL:${CONFIG_TFM_FP},soft>:CONFIG_TFM_FP=0>
        $<$<BOOL:${CONFIG_TFM_LAZY_STACKING}>:CONFIG_TFM_LAZY_STACKING>
    PRIVATE
        $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        $<$<OR:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<STREQUAL:"${TEST_PSA_API}","IPC">>:CONFIG_TFM_ENABLE_MEMORY_PROTECT>
        $<$<AND:$<BOOL:${TFM_PXN_ENABLE}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>>:TFM_PXN_ENABLE>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
        $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
        $<$<BOOL:${ATTEST_INCLUDE_COSE_KEY_ID}>:ATTEST_INCLUDE_COSE_KEY_ID>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
        $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
)

target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CP_FLAG}
)

#========================= Platform Non-Secure ================================#

target_sources(platform_ns
    PRIVATE
        $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/uart_stdout.c>
        $<$<AND:$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>,$<BOOL:${TEST_NS_ATTESTATION}>>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/template/tfm_initial_attest_pub_key.c>
)

target_link_libraries(platform_ns
    PUBLIC
        platform_common_interface
        platform_region_defs
        tfm_ns_interface
        test_ns_crypto_hw
)

target_compile_definitions(platform_ns
    PUBLIC
        $<$<STREQUAL:${CONFIG_TFM_FP},hard>:CONFIG_TFM_FP=2>
        $<$<STREQUAL:${CONFIG_TFM_FP},soft>:CONFIG_TFM_FP=0>
        $<$<BOOL:${TEST_NS_FPU}>:TEST_NS_FPU>
    PRIVATE
        $<$<BOOL:${TEST_NS_SLIH_IRQ}>:TEST_NS_SLIH_IRQ>
)

target_compile_options(platform_ns
    PUBLIC
        ${COMPILER_CP_FLAG}
)

#========================= Platform BL2 =======================================#
if(BL2)
    #TODO import policy
    target_include_directories(platform_bl2
        PUBLIC
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/accelerator/interface>
    )

    target_sources(platform_bl2
        PRIVATE
            ext/common/boot_hal_bl2.c
            $<$<BOOL:${PLATFORM_DEFAULT_UART_STDOUT}>:${CMAKE_CURRENT_SOURCE_DIR}/ext/common/uart_stdout.c>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
            $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/otp_flash.c>
            $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/nv_counters.c>
            $<$<BOOL:${PLATFORM_DEFAULT_ROTPK}>:ext/common/template/tfm_rotpk.c>
            $<$<OR:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
    )

    target_link_libraries(platform_bl2
        PUBLIC
            platform_common_interface
            platform_region_defs
        PRIVATE
            bl2_hal
    )

    target_compile_definitions(platform_bl2
        PUBLIC
            BL2
            MCUBOOT_${MCUBOOT_UPGRADE_STRATEGY}
            $<$<BOOL:${MCUBOOT_DIRECT_XIP_REVERT}>:MCUBOOT_DIRECT_XIP_REVERT>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
            $<$<BOOL:${MCUBOOT_HW_KEY}>:MCUBOOT_HW_KEY>
            MCUBOOT_FIH_PROFILE_${MCUBOOT_FIH_PROFILE}
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
            $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
            $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
            $<$<BOOL:${ATTEST_INCLUDE_COSE_KEY_ID}>:ATTEST_INCLUDE_COSE_KEY_ID>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
    )

    if (${PLATFORM_PSA_ADAC_SECURE_DEBUG})

        target_link_libraries(platform_bl2
            PRIVATE
                trusted-firmware-m-psa-adac
        )

        target_compile_definitions(platform_bl2
            PRIVATE
                PLATFORM_PSA_ADAC_SECURE_DEBUG
        )

    endif()

endif()

#========================= Platform BL1_1 =====================================#

if(BL1 AND PLATFORM_DEFAULT_BL1)
    target_include_directories(platform_bl1_interface
        INTERFACE
            .
            ./include
            ./ext/cmsis
            ./ext/driver
    )

    target_link_libraries(platform_bl1_interface
        INTERFACE
            platform_region_defs
            platform_common_interface
    )

    target_compile_definitions(platform_bl1_interface
        INTERFACE
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:CRYPTO_HW_ACCELERATOR>
            $<$<BOOL:${TFM_BL1_LOGGING}>:TFM_BL1_LOGGING>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:PLATFORM_DEFAULT_OTP>
            $<$<BOOL:${OTP_NV_COUNTERS_RAM_EMULATION}>:OTP_NV_COUNTERS_RAM_EMULATION>
            $<$<BOOL:${TFM_DUMMY_PROVISIONING}>:TFM_DUMMY_PROVISIONING>
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:PLATFORM_DEFAULT_NV_COUNTERS>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP_WRITEABLE}>:OTP_WRITEABLE>
    )

    target_sources(platform_bl1
        PRIVATE
            ./ext/common/boot_hal_bl1.c
            ./ext/common/uart_stdout.c
            $<$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>:ext/common/template/nv_counters.c>
            $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/otp_flash.c>
            $<$<NOT:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>>:ext/target/beken/nv_otp/nv_counters.c>
            $<$<OR:$<BOOL:${PLATFORM_DEFAULT_NV_COUNTERS}>,$<BOOL:${PLATFORM_DEFAULT_OTP}>>:ext/common/template/flash_otp_nv_counters_backend.c>
            $<$<BOOL:${PLATFORM_DEFAULT_OTP}>:ext/common/template/otp_flash.c>
    )

    target_link_libraries(platform_bl1
        PUBLIC
            platform_bl1_interface
        PRIVATE
            tfm_fih_implementation
            tfm_fih_headers
            $<$<BOOL:${CRYPTO_HW_ACCELERATOR}>:bl1_crypto_hw>
    )

endif()

#========================= Platform region defs ===============================#

#TODO maybe just link the other platforms to this
target_compile_definitions(platform_region_defs
    INTERFACE
        $<$<BOOL:${BL1}>:BL1>
        $<$<BOOL:${BL2}>:BL2>
        BL2_HEADER_SIZE=${BL2_HEADER_SIZE}
        BL2_TRAILER_SIZE=${BL2_TRAILER_SIZE}
        BL1_HEADER_SIZE=${BL1_HEADER_SIZE}
        BL1_TRAILER_SIZE=${BL1_TRAILER_SIZE}
        $<$<BOOL:${PLATFORM_DEFAULT_BL1}>:PLATFORM_DEFAULT_BL1>
        $<$<BOOL:${SECURE_UART1}>:SECURE_UART1>
        DAUTH_${DEBUG_AUTHENTICATION}
        $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA_LEN=${MCUBOOT_SIGNATURE_KEY_LEN}>
        $<$<STREQUAL:${MCUBOOT_EXECUTION_SLOT},2>:LINK_TO_SECONDARY_PARTITION>
        $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_${TEST_PSA_API}>
        $<$<BOOL:${FORWARD_PROT_MSG}>:FORWARD_PROT_MSG=${FORWARD_PROT_MSG}>
        $<$<BOOL:${TFM_CODE_SHARING}>:CODE_SHARING>
)
