#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

###################### Check compiler for FP vulnerability #####################

# Check compiler with mitigation for the VLLDM instruction security vulnerability or not.
# For more information, please check https://developer.arm.com/support/arm-security-updates/vlldm-instruction-security-vulnerability.
if (CONFIG_TFM_FP STREQUAL "hard")
    # Create test C file.
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cvetest.c "int x;")
    # Compile with mitigation -mfix-cmse-cve-2021-35465.
    execute_process (
        COMMAND ${CMAKE_C_COMPILER} -mfix-cmse-cve-2021-35465 -S ${CMAKE_CURRENT_BINARY_DIR}/cvetest.c -o ${CMAKE_CURRENT_BINARY_DIR}/cvetest.s
        RESULT_VARIABLE ret
        ERROR_VARIABLE err
    )
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/cvetest.c)
    # Check result
    if(NOT ret EQUAL 0)
        message(FATAL_ERROR "To enable FPU usage in SPE and NSPE both, please use the compiler with '-mfix-cmse-cve-2021-35465' support")
    else()
        file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/cvetest.s)
    endif()
endif()
