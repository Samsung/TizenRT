#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

# Add test suites and test service common library
add_subdirectory(common/suites)
add_subdirectory(common/service)

# Add ipc and sfn test suite.
add_subdirectory(ipc)
add_subdirectory(sfn)
add_subdirectory(irq)
