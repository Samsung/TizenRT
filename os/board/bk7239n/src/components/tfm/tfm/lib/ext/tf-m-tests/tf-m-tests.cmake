#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

get_cmake_property(CACHE_VARS CACHE_VARIABLES)

# By default all non-secure regression tests are disabled.
# If TEST_NS or TEST_NS_XXX flag is passed via command line and set to ON,
# selected corresponding features to support non-secure regression tests.
foreach(CACHE_VAR ${CACHE_VARS})
    string(REGEX MATCH "^TEST_NS.*" _NS_TEST_FOUND "${CACHE_VAR}")
    if (_NS_TEST_FOUND AND "${${CACHE_VAR}}")
        # TFM_NS_REG_TEST is a TF-M internal cmake flag to manage building
        # tf-m-tests non-secure regression tests related source
        set(TFM_NS_REG_TEST ON)
        break()
    endif()
endforeach()

# By default all secure regression tests are disabled.
# If TEST_S or TEST_S_XXX flag is passed via command line and set to ON,
# selected corresponding features to support secure regression tests.
foreach(CACHE_VAR ${CACHE_VARS})
    string(REGEX MATCH "^TEST_S.*" _S_TEST_FOUND "${CACHE_VAR}")
    if (_S_TEST_FOUND AND "${${CACHE_VAR}}")
        # TFM_S_REG_TEST is a TF-M internal cmake flag to manage building
        # tf-m-tests secure regression tests related source
        set(TFM_S_REG_TEST ON)
        break()
    endif()
endforeach()

# By default EXTRA_<NS/S>_TEST_SUITES_PATHS is not set, extra test is also an
# out-of-tree build regression test, and if they are enabled,
# TFM_<NS/S>_REG_TEST will be enabled.
if (EXTRA_NS_TEST_SUITES_PATHS)
    set(TFM_NS_REG_TEST ON)
endif()

if (EXTRA_S_TEST_SUITES_PATHS)
    set(TFM_S_REG_TEST ON)
endif()

# If NS app, secure regression test or non-secure regression test is enabled,
# fetch tf-m-tests repo.
# The conditiions are actually overlapped but it can make the logic more clear.
# Besides, the dependencies between NS app and regression tests will be
# optimized later.
if (NS OR TFM_S_REG_TEST OR TFM_NS_REG_TEST OR TEST_BL2 OR TEST_BL1_1 OR TEST_BL1_2)
    # Set tf-m-tests repo config
    include(${CMAKE_SOURCE_DIR}/lib/ext/tf-m-tests/repo_config_default.cmake)
    # Fetch tf-m-tests repo
    include(${CMAKE_SOURCE_DIR}/lib/ext/tf-m-tests/fetch_repo.cmake)
endif()
