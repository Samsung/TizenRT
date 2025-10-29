#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_1_test_suite_crypto)

target_sources(bl1_1_test_suite_crypto
    PRIVATE
        ./bl1_1_crypto_tests.c
)

target_include_directories(bl1_1_test_suite_crypto
    PUBLIC
        .
)

target_link_libraries(bl1_1_test_suite_crypto
    PRIVATE
        tfm_test_framework_common
        bl1_1_shared_lib
)
