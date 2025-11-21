#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_1_test_suite_trng)

target_sources(bl1_1_test_suite_trng
    PRIVATE
        ./bl1_1_trng_tests.c
)

target_include_directories(bl1_1_test_suite_trng
    PUBLIC
        .
)

target_link_libraries(bl1_1_test_suite_trng
    PRIVATE
        tfm_test_framework_common
        bl1_1_shared_lib
)
