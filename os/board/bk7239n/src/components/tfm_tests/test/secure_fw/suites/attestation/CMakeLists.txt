#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_ATTESTATION AND NOT TEST_S_ATTESTATION)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_ATTESTATION)
    add_library(tfm_test_suite_attestation_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_attestation_ns
        PRIVATE
            attest_public_key.c
            attest_token_test.c
            attest_token_decode_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ext/qcbor_util/qcbor_util.c
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:attest_token_decode_symmetric.c>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:non_secure/attest_symmetric_ns_interface_testsuite.c>
            $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:attest_token_decode_asymmetric.c>
            $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:non_secure/attest_asymmetric_ns_interface_testsuite.c>
    )

    target_include_directories(tfm_test_suite_attestation_ns
        PUBLIC
            non_secure
            ${CMAKE_CURRENT_SOURCE_DIR}/ext/qcbor_util
        PRIVATE
            .
    )

    target_compile_definitions(tfm_test_suite_attestation_ns
        PRIVATE
            DOMAIN_NS=1
            $<$<CONFIG:Debug>:INCLUDE_TEST_CODE>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        INTERFACE
            TEST_NS_ATTESTATION
    )

    target_link_libraries(tfm_test_suite_attestation_ns
        PRIVATE
            tfm_qcbor_ns
            tfm_t_cose_ns
            tfm_test_framework_ns
            #TODO I'm not sure this should _really_ be linked here
            tfm_attestation_defs
            tfm_boot_status
            platform_ns
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_attestation_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_ATTESTATION)
    target_sources(tfm_test_suite_attestation_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/attest_public_key.c
            ${CMAKE_CURRENT_SOURCE_DIR}/attest_token_test.c
            ${CMAKE_CURRENT_SOURCE_DIR}/attest_token_decode_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ext/qcbor_util/qcbor_util.c
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:${CMAKE_CURRENT_SOURCE_DIR}/attest_token_decode_symmetric.c>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:${CMAKE_CURRENT_SOURCE_DIR}/secure/attest_symmetric_s_interface_testsuite.c>
            $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:${CMAKE_CURRENT_SOURCE_DIR}/attest_token_decode_asymmetric.c>
            $<$<NOT:$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>>:${CMAKE_CURRENT_SOURCE_DIR}/secure/attest_asymmetric_s_interface_testsuite.c>
    )

    target_include_directories(tfm_test_suite_attestation_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
            ${CMAKE_CURRENT_SOURCE_DIR}/ext/qcbor_util
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_definitions(tfm_test_suite_attestation_s
        PRIVATE
            $<$<CONFIG:Debug>:INCLUDE_TEST_CODE>
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
        INTERFACE
            TEST_S_ATTESTATION
    )

    target_compile_options(tfm_test_suite_attestation_s
        PUBLIC
            ${COMPILER_CP_FLAG}
    )

    target_link_libraries(tfm_test_suite_attestation_s
        PRIVATE
            tfm_qcbor_s
            tfm_t_cose_s
            tfm_test_framework_s
            tfm_attestation_defs
            tfm_boot_status
            $<$<NOT:$<BOOL:${FORWARD_PROT_MSG}>>:tfm_psa_rot_partition_crypto>
            platform_s
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_attestation_s
    )
endif()
