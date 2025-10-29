#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(CMSIS_5_RTX_V8MMN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_V8MMFN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_V8MBN STATIC IMPORTED GLOBAL)
add_library(CMSIS_5_RTX_CM3 STATIC IMPORTED GLOBAL)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL GNU)
    set_target_properties(CMSIS_5_RTX_V8MMN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/GCC/libRTX_V8MMN.a)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/GCC/libRTX_V8MMFN.a)
    set_target_properties(CMSIS_5_RTX_V8MBN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/GCC/libRTX_V8MBN.a)
    set_target_properties(CMSIS_5_RTX_CM3   PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/GCC/libRTX_CM3.a)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL ARMClang)
    set_target_properties(CMSIS_5_RTX_V8MMN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/ARM/RTX_V8MMN.lib)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/ARM/RTX_V8MMFN.lib)
    set_target_properties(CMSIS_5_RTX_V8MBN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/ARM/RTX_V8MBN.lib)
    set_target_properties(CMSIS_5_RTX_CM3   PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/ARM/RTX_CM3.lib)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL IAR)
    add_library(CMSIS_5_RTX_V81MMN STATIC IMPORTED GLOBAL)
    set_target_properties(CMSIS_5_RTX_V8MMN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/IAR/RTX_V8MMN.a)
    set_target_properties(CMSIS_5_RTX_V81MMN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/IAR/RTX_V81MMN.a)
    set_target_properties(CMSIS_5_RTX_V8MMFN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/IAR/RTX_V8MMFN.a)
    set_target_properties(CMSIS_5_RTX_V8MBN PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/IAR/RTX_V8MBN.a)
    set_target_properties(CMSIS_5_RTX_CM3   PROPERTIES IMPORTED_LOCATION ${CMSIS_5_PATH}/RTOS2/RTX/Library/IAR/RTX_CM3.a)
    target_link_libraries(CMSIS_5_RTX_V81MMN
        INTERFACE
            tfm_s_veneers
    )
else()
    message(FATAL_ERROR "${CMAKE_C_COMPILER_ID} does not have CMSIS RTX static libraries set up")
endif()

target_link_libraries(CMSIS_5_RTX_V8MMN
    INTERFACE
        tfm_s_veneers
)

target_link_libraries(CMSIS_5_RTX_V8MMFN
    INTERFACE
        tfm_s_veneers
)

target_link_libraries(CMSIS_5_RTX_V8MBN
    INTERFACE
        tfm_s_veneers
)

