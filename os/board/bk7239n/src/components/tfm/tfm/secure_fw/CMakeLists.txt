#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_executable(tfm_s)
add_library(secure_fw INTERFACE)
add_library(tfm_secure_api INTERFACE)

add_subdirectory(spm)
add_subdirectory(partitions)

target_include_directories(secure_fw
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/partitions>
)

target_link_libraries(secure_fw
    INTERFACE
        tfm_spm
        tfm_partitions
)

target_link_libraries(tfm_s
    PRIVATE
        secure_fw
        platform_s
        psa_interface
        tfm_sprt
        tfm_secure_api
)

set_target_properties(tfm_s
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(tfm_s
    PUBLIC
        ${COMPILER_CP_FLAG}
)

target_compile_definitions(tfm_s
	PUBLIC
	CONFIG_TFM_S
)

target_link_options(tfm_s
    PRIVATE
        --entry=Reset_Handler
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/tfm_s.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/tfm_s.map>
    PUBLIC
        ${LINKER_CP_OPTION}
)

add_convert_to_bin_target(tfm_s)

############################ Secure API ########################################

target_include_directories(tfm_secure_api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(tfm_secure_api
    INTERFACE
        tfm_arch
)

set_source_files_properties(
    ${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_svc.c
    ${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_cross.c
    ${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_sfn.c
    PROPERTIES
    COMPILE_FLAGS $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
    COMPILE_FLAGS $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

# Secure component relies on 'tfm_secure_api', append headers/sources
# into this target, also the SPE build indicator.
target_sources(tfm_secure_api
    INTERFACE
        $<$<BOOL:${CONFIG_TFM_PSA_API_SUPERVISOR_CALL}>:${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_svc.c>
        $<$<BOOL:${CONFIG_TFM_PSA_API_CROSS_CALL}>:${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_cross.c>
        $<$<BOOL:${CONFIG_TFM_PSA_API_SFN_CALL}>:${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/psa_interface_sfn.c>
)

target_compile_definitions(tfm_secure_api
    INTERFACE
        CONFIG_TFM_BUILDING_SPE=1
        $<$<BOOL:${CONFIG_TFM_DOORBELL_API}>:CONFIG_TFM_DOORBELL_API=1>
)

############################# Secure veneers ###################################

if(NOT (TFM_PSA_API AND TFM_MULTI_CORE_TOPOLOGY))
    add_library(tfm_s_veneers STATIC)

    target_sources(tfm_s_veneers
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
    )

    # Since s_veneers.o doesn't exist when this is evaluated by cmake we need to
    # explicity specify what language it will use.
    set_target_properties(tfm_s_veneers
        PROPERTIES
            LINKER_LANGUAGE C
    )

    # Pretend we have a command to generate the veneers, when in reality all
    # that's needed is the dependency on tfm_s. This is required for the ninja
    # build system
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
        COMMAND
        DEPENDS tfm_s
    )

    target_link_options(tfm_s
        PRIVATE
            ${LINKER_VENEER_OUTPUT_FLAG}${CMAKE_CURRENT_BINARY_DIR}/s_veneers.o
    )
endif()

############################### CODE SHARING ###################################
if (TFM_CODE_SHARING)
    target_link_shared_code(tfm_s
        bl2
    )

    # mbedtls is build outside of tree, so we have to use the _from_dependency
    # version of this function to attach the custom_command to the tfm_s target.
    # It's also picky about stripping the symbols, so we just make them weak
    # instead.
    target_weaken_symbols_from_dependency(tfm_s crypto_service_mbedcrypto
        mbedtls_asn1*
        mbedtls_mpi*
        mbedtls_platform*
        mbedtls_rsa*

        #This group is only relevant if BL2 image encryption is on
        mbedtls_md*

        #This group has two functions that cause runtime errors when shared, so the
        #error-free ones are listed piece by piece
        mbedtls_internal_sha256*
        mbedtls_sha256_free
        mbedtls_sha256_init
        mbedtls_sha256_finish
        mbedtls_sha256_starts

        #Symbols necessary to make sharing additional functions possible
        mbedtls_calloc*
        mbedtls_free*

        #Miscellaneous functions
        mbedtls_exit*
        memset_func*
    )

endif()
