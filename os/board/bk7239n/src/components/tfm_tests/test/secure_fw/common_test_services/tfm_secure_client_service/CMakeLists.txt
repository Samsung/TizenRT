#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_secure_client_service STATIC)

target_sources(tfm_psa_rot_partition_secure_client_service
    PRIVATE
        ./tfm_secure_client_service.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_secure_client_service
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_secure_client_service/auto_generated/intermedia_tfm_secure_client_service.c>
)

target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_secure_client_service/auto_generated/load_info_tfm_secure_client_service.c>
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_secure_client_service
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_secure_client_service
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_secure_client_service
)

target_link_libraries(tfm_psa_rot_partition_secure_client_service
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_test_framework_s
        tfm_s_tests
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_psa_rot_partition_secure_client_service
)

############################ NS API ############################################

# As secure regression tests can run without non-secure client trigger in IPC
# model now, function tfm_secure_client_service_api.c will be used only in
# library model
if(NS AND TFM_LIB_MODEL)
    target_sources(tfm_api_ns
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tfm_secure_client_service_api.c
    )

    # The veneers give warnings about not being properly declared so they get hidden
    # to not overshadow _real_ warnings.
    set_source_files_properties(tfm_secure_client_service_api.c
        PROPERTIES
            COMPILE_FLAGS -Wno-implicit-function-declaration
    )

    target_include_directories(tfm_api_ns
        PUBLIC
            .
    )
endif()

# Export test service non-secure interface API in Library model
if(TFM_LIB_MODEL)
    install(FILES       ${CMAKE_CURRENT_SOURCE_DIR}/tfm_secure_client_service_api.c
            DESTINATION ${TEST_SERVICE_SRC_INSTALL_DIR})
    install(FILES       ${CMAKE_CURRENT_SOURCE_DIR}/tfm_secure_client_service_api.h
            DESTINATION ${TEST_SERVICE_INC_INSTALL_DIR})
endif()

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_secure_client_service
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_TEST_SECURE_SERVICES
)
