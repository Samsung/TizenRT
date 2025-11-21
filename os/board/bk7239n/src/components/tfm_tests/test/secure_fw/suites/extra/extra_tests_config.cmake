#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## load extra NS test suites source ####################

if(EXTRA_NS_TEST_SUITES_PATHS)
    # load source
    foreach(EXTRA_NS_TEST_SUITE IN LISTS EXTRA_NS_TEST_SUITES_PATHS)
        get_filename_component(EXTRA_NS_TEST_SUITE_NAME ${EXTRA_NS_TEST_SUITE} NAME_WLE)

        # load subdirectories
        set(TEMP_BINARY_EXTRA_NS_TEST_SUITE
            "${CMAKE_CURRENT_BINARY_DIR}/${EXTRA_NS_TEST_SUITE_NAME}")
        add_subdirectory(${EXTRA_NS_TEST_SUITE} ${TEMP_BINARY_EXTRA_NS_TEST_SUITE})
    endforeach()
endif()

########################## load extra S test suites source #####################

if(EXTRA_S_TEST_SUITES_PATHS)
    # load source
    foreach(EXTRA_S_TEST_SUITE IN LISTS EXTRA_S_TEST_SUITES_PATHS)
        get_filename_component(EXTRA_S_TEST_SUITE_NAME ${EXTRA_S_TEST_SUITE} NAME_WLE)

        # load subdirectories
        set(TEMP_BINARY_EXTRA_S_TEST_SUITE
            "${CMAKE_CURRENT_BINARY_DIR}/${EXTRA_S_TEST_SUITE_NAME}")
        add_subdirectory(${EXTRA_S_TEST_SUITE} ${TEMP_BINARY_EXTRA_S_TEST_SUITE})
    endforeach()
endif()
