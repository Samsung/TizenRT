#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_spm STATIC)
add_library(tfm_spm_defs INTERFACE)
add_library(tfm_boot_status INTERFACE)
add_library(tfm_arch INTERFACE)
add_library(tfm_utilities INTERFACE)

set(TFM_PARTITION_PLATFORM ON CACHE BOOL "Enable the TF-M Platform partition")

# Generate TF-M version
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/tfm_version.h.in
               ${CMAKE_BINARY_DIR}/generated/secure_fw/spm/include/tfm_version.h)

target_include_directories(tfm_spm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa>
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa/include>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_func>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_func/include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/secure_fw/include
        ${CMAKE_BINARY_DIR}/generated
        ${CMAKE_BINARY_DIR}/generated/secure_fw/spm/include
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_BINARY_DIR}/generated/secure_fw/spm/cmsis_psa>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:${CMAKE_BINARY_DIR}/generated/secure_fw/spm/cmsis_func>
)

target_sources(tfm_spm
    PRIVATE
        ffm/tfm_boot_data.c
        ffm/tfm_core_utils.c
        ffm/utilities.c
        $<$<NOT:$<STREQUAL:${TFM_SPM_LOG_LEVEL},TFM_SPM_LOG_LEVEL_SILENCE>>:ffm/spm_log.c>
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:cmsis_psa/tfm_multi_core_mem_check.c>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:ffm/tfm_core_mem_check.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/arch/tfm_arch.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/main.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/spm_ipc.c>
        $<$<BOOL:${CONFIG_TFM_PSA_API_CROSS_CALL}>:cmsis_psa/spm_cross_call.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/static_load.c>
        $<$<BOOL:${TFM_PSA_API}>:ffm/psa_api.c>
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_IPC}>:ffm/backend_ipc.c>
        $<$<BOOL:${CONFIG_TFM_SPM_BACKEND_SFN}>:ffm/backend_sfn.c>
        $<$<BOOL:${TFM_PSA_API}>:ffm/interrupt.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/tfm_core_svcalls_ipc.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/tfm_pools.c>
        $<$<BOOL:${TFM_PSA_API}>:cmsis_psa/thread.c>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:cmsis_func/main.c>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:cmsis_func/arch.c>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:cmsis_func/spm_func.c>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:cmsis_func/tfm_core_svcalls_func.c>
        $<$<BOOL:${TFM_NS_MANAGE_NSID}>:ns_client_ext/tfm_ns_ctx.c>
        ns_client_ext/tfm_spm_ns_ctx.c
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:cmsis_func/tfm_secure_api.c>
        #TODO add other arches
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>>:cmsis_psa/arch/tfm_arch_v8m_main.c>
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>>:cmsis_psa/arch/tfm_arch_v8m_base.c>
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.main>>:cmsis_psa/arch/tfm_arch_v8m_main.c>
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>>:cmsis_psa/arch/tfm_arch_v6m_v7m.c>
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv7-m>>:cmsis_psa/arch/tfm_arch_v6m_v7m.c>
)

target_include_directories(tfm_spm_defs
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/interface>
        $<$<NOT:$<BOOL:${TFM_PSA_API}>>:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_func/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/arch/include>
)

target_link_libraries(tfm_spm
    PUBLIC
        tfm_arch
        tfm_spm_defs
    PRIVATE
        psa_interface
        platform_s
        tfm_boot_status
        tfm_secure_api
        tfm_partitions
        tfm_fih_headers
        tfm_sprt
)

target_compile_definitions(tfm_spm
    PRIVATE
        $<$<CONFIG:Debug>:TFM_CORE_DEBUG>
        $<$<AND:$<BOOL:${BL2}>,$<BOOL:${MCUBOOT_MEASURED_BOOT}>>:BOOT_DATA_AVAILABLE>
        $<$<BOOL:${CONFIG_TFM_HALT_ON_CORE_PANIC}>:CONFIG_TFM_HALT_ON_CORE_PANIC>
        $<$<BOOL:${TFM_NS_MANAGE_NSID}>:TFM_NS_MANAGE_NSID>
        $<$<BOOL:${TFM_PSA_API}>:CONFIG_TFM_CONN_HANDLE_MAX_NUM=${CONFIG_TFM_CONN_HANDLE_MAX_NUM}>
        # CONFIG_TFM_FP
        $<$<STREQUAL:${CONFIG_TFM_FP},hard>:CONFIG_TFM_FP=2>
        $<$<STREQUAL:${CONFIG_TFM_FP},soft>:CONFIG_TFM_FP=0>
        $<$<BOOL:${CONFIG_TFM_DOORBELL_API}>:CONFIG_TFM_DOORBELL_API=1>
)

target_compile_options(tfm_spm
    PUBLIC
        ${COMPILER_CP_FLAG}
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ TFM arch ##########################################

target_include_directories(tfm_arch
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cmsis_psa/arch>
)

############################ Boot Status #######################################

target_include_directories(tfm_boot_status
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

############################ TFM utilities #####################################

target_include_directories(tfm_utilities
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

############################ Secure API ########################################

target_link_libraries(tfm_secure_api
    INTERFACE
        tfm_spm_defs
)

############################# Secure veneers ###################################

if(NOT TFM_MULTI_CORE_TOPOLOGY)
    # If this is added to the spm, it is discarded as it is not used. Since the
    # spm is a static library it can't generate veneers under all compilers so
    # instead this single file is added to the tfm_s target.
    target_sources(tfm_s
        PRIVATE
            $<$<NOT:$<BOOL:${TFM_PSA_API}>>:${CMAKE_BINARY_DIR}/generated/secure_fw/spm/cmsis_func/tfm_veneers.c>
            $<$<BOOL:${TFM_NS_MANAGE_NSID}>:${CMAKE_CURRENT_SOURCE_DIR}/ns_client_ext/tfm_ns_client_ext.c>
    )
endif()
