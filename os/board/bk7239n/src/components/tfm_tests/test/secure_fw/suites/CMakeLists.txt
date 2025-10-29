#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# For multi-core projects, the NS app can be run on a different CPU to the
# Secure code. To facilitate this, we once again reload the compiler to load the
# setting for the NS CPU. Cmake settings are directory scoped so this affects
# anything loaded from or declared in this dir.
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    tfm_toolchain_reload_compiler()
endif()

if (TEST_FRAMEWORK_NS)
    add_library(tfm_ns_tests INTERFACE)
    add_library(tfm_test_framework_ns INTERFACE)

    target_compile_definitions(tfm_test_framework_ns
        INTERFACE
            DOMAIN_NS=1
    )

    target_link_libraries(tfm_test_framework_ns
        INTERFACE
            tfm_test_framework_common
            tfm_api_ns
            tfm_ns_interface
            tfm_log
    )

    target_compile_options(tfm_test_framework_ns
        INTERFACE
            ${COMPILER_CP_FLAG}
    )

    target_sources(tfm_ns_tests
        INTERFACE
            ../non_secure_suites.c
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_framework_ns
            tfm_partition_defs
    )

    target_compile_definitions(tfm_ns_tests
        INTERFACE
            $<$<BOOL:${SYMMETRIC_INITIAL_ATTESTATION}>:SYMMETRIC_INITIAL_ATTESTATION>
            $<$<BOOL:${TFM_FUZZER_TOOL_TESTS}>:TFM_FUZZER_TOOL_TESTS>
    )
else()
    # Skip building NS regresstion test suites if TEST_FRAMEWORK_NS is OFF.
    # NS test flags are kept to select corresponding test services. They are
    # unset here to skip building NS regression test suites.
    get_cmake_property(CACHE_VARS CACHE_VARIABLES)

    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_NS_.*" _NS_TEST_FOUND "${CACHE_VAR}")
        if (_NS_TEST_FOUND AND "${${CACHE_VAR}}")
            unset(${CACHE_VAR} CACHE)
        endif()
    endforeach()
endif()

# Add test suites.
# Secure test suite library targets shall already be added in secure_tests.cmake
add_subdirectory(attestation)
add_subdirectory(audit)
add_subdirectory(core)
add_subdirectory(crypto)
add_subdirectory(extra)
add_subdirectory(its)
add_subdirectory(qcbor)
add_subdirectory(ps)
add_subdirectory(t_cose)
add_subdirectory(platform)
add_subdirectory(fwu)
add_subdirectory(multi_core/non_secure)
add_subdirectory(nsid)
add_subdirectory(fpu)
add_subdirectory(spm)
add_subdirectory(vt)

if (TFM_FUZZER_TOOL_TESTS AND TEST_FRAMEWORK_NS)
    if (NOT DEFINED TFM_FUZZER_TOOL_TESTS_CMAKE_INC_PATH)
        # The location of the file needs to be defined either from command line
        # or from config cmake file.
        message(FATAL_ERROR "Incomplete build configuration: TFM_FUZZER_TOOL_TESTS_CMAKE_INC_PATH is undefined.")
    else()
        add_subdirectory(${TFM_FUZZER_TOOL_TESTS_CMAKE_INC_PATH} ${CMAKE_BINARY_DIR}/suites/tfm_fuzz)
    endif()
endif()
