#-------------------------------------------------------------------------------
# Copyright (c) 2020, IAR Systems AB. All rights reserved.
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)
cmake_policy(SET CMP0115 NEW)

# Don't load this file if it is specified as a cmake toolchain file
if(NOT TFM_TOOLCHAIN_FILE)
    message(DEPRECATION "SETTING CMAKE_TOOLCHAIN_FILE is deprecated. It has been replaced with TFM_TOOLCHAIN_FILE.")
    return()
endif()

SET(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR       ${TFM_SYSTEM_PROCESSOR})

if(CROSS_COMPILE)
    set(CMAKE_C_COMPILER_TARGET      arm-${CROSS_COMPILE})
    set(CMAKE_ASM_COMPILER_TARGET    arm-${CROSS_COMPILE})
else()
    set(CMAKE_C_COMPILER_TARGET      arm-arm-none-eabi)
    set(CMAKE_ASM_COMPILER_TARGET    arm-arm-none-eabi)
endif()

set(CMAKE_C_COMPILER iccarm)
set(CMAKE_ASM_COMPILER iasmarm)

set(LINKER_VENEER_OUTPUT_FLAG --import_cmse_lib_out= )
set(COMPILER_CMSE_FLAG --cmse)

# This variable name is a bit of a misnomer. The file it is set to is included
# at a particular step in the compiler initialisation. It is used here to
# configure the extensions for object files. Despite the name, it also works
# with the Ninja generator.
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/set_extensions.cmake)

macro(tfm_toolchain_reset_compiler_flags)
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")

    add_compile_options(
        $<$<COMPILE_LANGUAGE:C,CXX>:-e>
        $<$<COMPILE_LANGUAGE:C,CXX>:--dlib_config=full>
        $<$<COMPILE_LANGUAGE:C,CXX>:--vla>
        $<$<COMPILE_LANGUAGE:C,CXX>:--silent>
        $<$<COMPILE_LANGUAGE:C,CXX>:-DNO_TYPEOF>
        $<$<COMPILE_LANGUAGE:C,CXX>:-D_NO_DEFINITIONS_IN_HEADER_FILES>
        $<$<COMPILE_LANGUAGE:C,CXX>:--diag_suppress=Pe546,Pe940,Pa082,Pa084>
        $<$<AND:$<COMPILE_LANGUAGE:C,CXX,ASM>,$<NOT:$<BOOL:${TFM_SYSTEM_FP}>>>:--fpu=none>
        $<$<AND:$<COMPILE_LANGUAGE:C,CXX,ASM>,$<BOOL:${TFM_DEBUG_SYMBOLS}>,$<CONFIG:Release,MinSizeRel>>:-r>
    )
endmacro()

macro(tfm_toolchain_reset_linker_flags)
    set_property(DIRECTORY PROPERTY LINK_OPTIONS "")

    add_link_options(
      --silent
      --semihosting
      --redirect __write=__write_buffered
      $<$<NOT:$<BOOL:${TFM_SYSTEM_FP}>>:--fpu=none>
    )
endmacro()

macro(tfm_toolchain_set_processor_arch)
    if(${TFM_SYSTEM_PROCESSOR} STREQUAL "cortex-m0plus")
      set(CMAKE_SYSTEM_PROCESSOR Cortex-M0+)
    else()
      set(CMAKE_SYSTEM_PROCESSOR ${TFM_SYSTEM_PROCESSOR})
    endif()

    if (DEFINED TFM_SYSTEM_DSP)
        if(NOT TFM_SYSTEM_DSP)
            string(APPEND CMAKE_SYSTEM_PROCESSOR ".no_dsp")
        endif()
    endif()
endmacro()

macro(tfm_toolchain_reload_compiler)
    tfm_toolchain_set_processor_arch()
    tfm_toolchain_reset_compiler_flags()
    tfm_toolchain_reset_linker_flags()

    unset(CMAKE_C_FLAGS_INIT)
    unset(CMAKE_C_LINK_FLAGS)
    unset(CMAKE_ASM_FLAGS_INIT)
    unset(CMAKE_ASM_LINK_FLAGS)

    set(CMAKE_C_FLAGS_INIT "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ASM_FLAGS_INIT "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_C_LINK_FLAGS "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ASM_LINK_FLAGS "--cpu ${CMAKE_SYSTEM_PROCESSOR}")

    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT})
    set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})
endmacro()

# Configure environment for the compiler setup run by cmake at the first
# `project` call in <tfm_root>/CMakeLists.txt. After this mandatory setup is
# done, all further compiler setup is done via tfm_toolchain_reload_compiler()
tfm_toolchain_reload_compiler()

# Behaviour for handling scatter files is so wildly divergent between compilers
# that this macro is required.
macro(target_add_scatter_file target)
    target_link_options(${target}
        PRIVATE
        --config $<TARGET_OBJECTS:${target}_scatter>
    )

    add_library(${target}_scatter OBJECT)
    foreach(scatter_file ${ARGN})
        target_sources(${target}_scatter
            PRIVATE
                ${scatter_file}
        )
        # Cmake cannot use generator expressions in the
        # set_source_file_properties command, so instead we just parse the regex
        # for the filename and set the property on all files, regardless of if
        # the generator expression would evaluate to true or not.
        string(REGEX REPLACE ".*>:(.*)>$" "\\1" SCATTER_FILE_PATH "${scatter_file}")
        set_source_files_properties(${SCATTER_FILE_PATH}
            PROPERTIES
            LANGUAGE C
        )
    endforeach()

    add_dependencies(${target}
        ${target}_scatter
    )

    set_target_properties(${target} PROPERTIES LINK_DEPENDS $<TARGET_OBJECTS:${target}_scatter>)

    target_link_libraries(${target}_scatter
        platform_region_defs
        psa_interface
        tfm_partition_defs
    )

    target_compile_options(${target}_scatter
        PRIVATE
            --preprocess=sn $<TARGET_OBJECTS:${target}_scatter>
    )
endmacro()

macro(add_convert_to_bin_target target)
    get_target_property(bin_dir ${target} RUNTIME_OUTPUT_DIRECTORY)

    add_custom_target(${target}_bin
        SOURCES ${bin_dir}/${target}.bin
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.bin
        DEPENDS ${target}
        COMMAND ielftool
            --silent
            --bin $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.bin
    )

    add_custom_target(${target}_elf
        SOURCES ${bin_dir}/${target}.elf
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.elf
        DEPENDS ${target}
        COMMAND ielftool
            --silent
            $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.elf
    )

    add_custom_target(${target}_hex
        SOURCES ${bin_dir}/${target}.hex
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.hex
        DEPENDS ${target}
        COMMAND ielftool
            --silent
            --ihex $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.hex
    )

    add_custom_target(${target}_binaries
        ALL
        DEPENDS ${target}_bin
        DEPENDS ${target}_elf
        DEPENDS ${target}_hex
    )
endmacro()

macro(compiler_create_shared_code TARGET SHARED_SYMBOL_TEMPLATE)
    message(FATAL_ERROR "Code sharing support is not implemented by IAR.")
endmacro()

macro(compiler_link_shared_code TARGET SHARED_CODE_PATH ORIG_TARGET LIB_LIST)
    message(FATAL_ERROR "Code sharing support is not implemented by IAR.")
endmacro()
