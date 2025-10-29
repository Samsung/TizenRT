#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

# Don't load this file if it is specified as a cmake toolchain file
if(NOT TFM_TOOLCHAIN_FILE)
    message(DEPRECATION "SETTING CMAKE_TOOLCHAIN_FILE is deprecated. It has been replaced with TFM_TOOLCHAIN_FILE.")
    return()
endif()

SET(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER armclang)
set(CMAKE_ASM_COMPILER armasm)

set(LINKER_VENEER_OUTPUT_FLAG --import_cmse_lib_out=)
set(COMPILER_CMSE_FLAG $<$<COMPILE_LANGUAGE:C>:-mcmse>)

# This variable name is a bit of a misnomer. The file it is set to is included
# at a particular step in the compiler initialisation. It is used here to
# configure the extensions for object files. Despite the name, it also works
# with the Ninja generator.
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/set_extensions.cmake)

macro(tfm_toolchain_reset_compiler_flags)
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")

    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:-Wno-ignored-optimization-argument>
        $<$<COMPILE_LANGUAGE:C>:-Wno-unused-command-line-argument>
        $<$<COMPILE_LANGUAGE:C>:-Wall>
        # Don't error when the MBEDTLS_NULL_ENTROPY warning is shown
        $<$<COMPILE_LANGUAGE:C>:-Wno-error=cpp>
        $<$<COMPILE_LANGUAGE:C>:-c>
        $<$<COMPILE_LANGUAGE:C>:-fdata-sections>
        $<$<COMPILE_LANGUAGE:C>:-ffunction-sections>
        $<$<COMPILE_LANGUAGE:C>:-fno-builtin>
        $<$<COMPILE_LANGUAGE:C>:-fshort-enums>
        $<$<COMPILE_LANGUAGE:C>:-fshort-wchar>
        $<$<COMPILE_LANGUAGE:C>:-funsigned-char>
        $<$<COMPILE_LANGUAGE:C>:-masm=auto>
        $<$<COMPILE_LANGUAGE:C>:-nostdlib>
        $<$<COMPILE_LANGUAGE:C>:-std=c99>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<NOT:$<BOOL:${TFM_SYSTEM_FP}>>>:-mfpu=none>
        $<$<AND:$<COMPILE_LANGUAGE:ASM>,$<NOT:$<BOOL:${TFM_SYSTEM_FP}>>>:--fpu=none>
        $<$<COMPILE_LANGUAGE:ASM>:--cpu=${CMAKE_ASM_CPU_FLAG}>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<BOOL:${TFM_DEBUG_SYMBOLS}>>:-g>
    )
endmacro()

if(CONFIG_TFM_MEMORY_USAGE_QUIET)
    set(MEMORY_USAGE_FLAG "")
else()
    set(MEMORY_USAGE_FLAG --info=summarysizes,sizes,totals,unused,veneers)
endif()

macro(tfm_toolchain_reset_linker_flags)
    set_property(DIRECTORY PROPERTY LINK_OPTIONS "")

    add_link_options(
        ${MEMORY_USAGE_FLAG}
        --strict
        --symbols
        --xref
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<STREQUAL:"${TEST_PSA_API}","IPC">>:--no-merge>
        # Suppress link warnings that are consistant (and therefore hopefully
        # harmless)
        # https://developer.arm.com/documentation/100074/0608/linker-errors-and-warnings/list-of-the-armlink-error-and-warning-messages
        # Empty region description
        --diag_suppress=6312
        # Ns section matches pattern
        --diag_suppress=6314
        # Duplicate input files
        --diag_suppress=6304
        # Pattern only matches removed unused sections.
        --diag_suppress=6329
        $<$<NOT:$<BOOL:${TFM_SYSTEM_FP}>>:--fpu=softvfp>
    )
endmacro()

macro(tfm_toolchain_set_processor_arch)
    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR       ${TFM_SYSTEM_PROCESSOR})

        if (DEFINED TFM_SYSTEM_MVE)
            if(NOT TFM_SYSTEM_MVE)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nomve")
            endif()
        endif()

        if (DEFINED TFM_SYSTEM_FP)
            if(NOT TFM_SYSTEM_FP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nofp")
            endif()
        endif()

        if (DEFINED TFM_SYSTEM_DSP)
            if(NOT TFM_SYSTEM_DSP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nodsp")
            endif()
        endif()

        string(REGEX REPLACE "\\+nodsp" ".no_dsp" CMAKE_ASM_CPU_FLAG "${CMAKE_SYSTEM_PROCESSOR}")
    else()
        set(CMAKE_ASM_CPU_FLAG  ${TFM_SYSTEM_ARCHITECTURE})

        # Armasm uses different syntax than armclang for architecture targets
        string(REGEX REPLACE "\\armv" "" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")
        string(REGEX REPLACE "\\armv" "" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")

        # Modifiers are additive instead of subtractive (.fp Vs .no_fp)
        if (TFM_SYSTEM_DSP)
            string(APPEND CMAKE_ASM_CPU_FLAG ".dsp")
        else()
            if (TFM_SYSTEM_MVE)
                string(APPEND CMAKE_ASM_CPU_FLAG ".mve")
            endif()

            if (TFM_SYSTEM_FP)
                string(APPEND CMAKE_ASM_CPU_FLAG ".fp")
            endif()
        endif()
    endif()

    # CMAKE_SYSTEM_ARCH is an ARMCLANG CMAKE internal variable, used to set
    # compile and linker flags up until CMake 3.21 where CMP0123 was introduced:
    # https://cmake.org/cmake/help/latest/policy/CMP0123.html
    # This behavior is overwritten by setting CMAKE_C_FLAGS in
    # tfm_toolchain_reload_compiler.
    # Another use of this variable is to statisfy a requirement for ARMCLANG to
    # set either the target CPU or the Architecture. This variable needs to be
    # set to allow targeting architectures without a specific CPU.
    set(CMAKE_SYSTEM_ARCH            ${TFM_SYSTEM_ARCHITECTURE})

    set(CMAKE_C_COMPILER_TARGET      arm-${CROSS_COMPILE})
    set(CMAKE_ASM_COMPILER_TARGET    arm-${CROSS_COMPILE})

    if (DEFINED TFM_SYSTEM_MVE)
        if(NOT TFM_SYSTEM_MVE)
            string(APPEND CMAKE_SYSTEM_ARCH "+nomve")
        endif()
    endif()

    if (DEFINED TFM_SYSTEM_FP)
        if(NOT TFM_SYSTEM_FP)
            string(APPEND CMAKE_SYSTEM_ARCH "+nofp")
        endif()
    endif()

    if (DEFINED TFM_SYSTEM_DSP)
        if(NOT TFM_SYSTEM_DSP)
            string(APPEND CMAKE_SYSTEM_ARCH "+nodsp")
        endif()
    endif()

    # Cmake's ARMClang support has several issues with compiler validation. To
    # avoid these, we set the list of supported -mcpu and -march variables to
    # the ones we intend to use so that the validation will never fail.
    include(Compiler/ARMClang)
    set(CMAKE_C_COMPILER_PROCESSOR_LIST ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_C_COMPILER_ARCH_LIST ${CMAKE_SYSTEM_ARCH})
    set(CMAKE_ASM_COMPILER_PROCESSOR_LIST ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_ASM_COMPILER_ARCH_LIST ${CMAKE_SYSTEM_ARCH})
endmacro()

macro(tfm_toolchain_reload_compiler)
    tfm_toolchain_set_processor_arch()
    tfm_toolchain_reset_compiler_flags()
    tfm_toolchain_reset_linker_flags()

    unset(CMAKE_C_FLAGS_INIT)
    unset(CMAKE_C_LINK_FLAGS)
    unset(CMAKE_ASM_FLAGS_INIT)
    unset(CMAKE_ASM_LINK_FLAGS)
    unset(__mcpu_flag_set)
    unset(__march_flag_set)

    include(Compiler/ARMClang)
    __compiler_armclang(C)
    include(Compiler/ARMCC-ASM)
    __compiler_armcc(ASM)

    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 6.10.1)
        message(FATAL_ERROR "Please select newer Arm compiler version starting from 6.10.1.")
    endif()

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 6.15)
        message(WARNING "Armclang starting from v6.15 may cause MemManage fault."
                            " The root cause is still under analysis by Armclang."
                            " Please use lower Armclang versions instead.")
    endif()

    # Cmake's armclang support will set either mcpu or march, but march gives
    # better code size so we manually set it.
    set(CMAKE_C_FLAGS   "-march=${CMAKE_SYSTEM_ARCH}")
    set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})

    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_C_LINK_FLAGS   "--cpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_ASM_LINK_FLAGS "--cpu=${CMAKE_SYSTEM_PROCESSOR}")
        # But armlink doesn't support this +dsp syntax
        string(REGEX REPLACE "\\+nodsp" "" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nodsp" "" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")
        # And uses different syntax for +nofp
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")

        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")
    endif()

    # Workaround for issues with --depend-single-line with armasm and Ninja
    if (CMAKE_GENERATOR STREQUAL "Ninja")
        set( CMAKE_DEPFILE_FLAGS_ASM "--depend=<OBJECT>.d")
    endif()

    set(CMAKE_C_FLAGS_MINSIZEREL "-Oz -DNDEBUG")
endmacro()

# Configure environment for the compiler setup run by cmake at the first
# `project` call in <tfm_root>/CMakeLists.txt. After this mandatory setup is
# done, all further compiler setup is done via tfm_toolchain_reload_compiler()
tfm_toolchain_set_processor_arch()
tfm_toolchain_reset_compiler_flags()
tfm_toolchain_reset_linker_flags()

# Behaviour for handling scatter files is so wildly divergent between compilers
# that this macro is required.
macro(target_add_scatter_file target)
    target_link_options(${target}
        PRIVATE
        --scatter=$<TARGET_OBJECTS:${target}_scatter>
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
            -E
            -xc
    )
endmacro()

macro(add_convert_to_bin_target target)
    get_target_property(bin_dir ${target} RUNTIME_OUTPUT_DIRECTORY)

    add_custom_target(${target}_bin
        SOURCES ${bin_dir}/${target}.bin
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.bin
        DEPENDS ${target}
        COMMAND fromelf
            --bincombined $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.bin
    )

    add_custom_target(${target}_elf
        SOURCES ${bin_dir}/${target}.elf
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.elf
        DEPENDS ${target}
        COMMAND fromelf
            --elf $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.elf
    )

    add_custom_target(${target}_hex
        SOURCES ${bin_dir}/${target}.hex
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.hex
        DEPENDS ${target}
        COMMAND fromelf
            --i32combined $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.hex
    )

    add_custom_target(${target}_binaries
        ALL
        DEPENDS ${target}_bin
        DEPENDS ${target}_elf
        DEPENDS ${target}_hex
    )
endmacro()

macro(target_share_symbols target symbol_name_file)
    get_target_property(TARGET_TYPE ${target} TYPE)
    if (NOT TARGET_TYPE STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "${target} is not an executable. Symbols cannot be shared from libraries.")
    endif()

    FILE(STRINGS ${symbol_name_file} KEEP_SYMBOL_LIST
        LENGTH_MINIMUM 1
    )

    # strip all the symbols except those proveded as arguments. Long inline
    # python scripts aren't ideal, but this is both portable and possibly easier
    # to maintain than trying to filter files at build time in cmake.
    add_custom_command(TARGET ${target}
        POST_BUILD
        VERBATIM
        COMMAND python3 -c "from sys import argv; import re; f = open(argv[1], 'rt'); p = [x.replace('*', '.*') for x in argv[2:]]; l = [x for x in f.readlines() if re.search(r'(?=('+'$|'.join(p + ['SYMDEFS']) + r'))', x)]; f.close(); f = open(argv[1], 'wt'); f.writelines(l); f.close();" $<TARGET_FILE_DIR:${target}>/${target}_shared_symbols.txt ${KEEP_SYMBOL_LIST})

    # Force the target to not remove the symbols if they're unused. Not
    # currently possible on GNU, has to be part of the linker script.
    list(TRANSFORM KEEP_SYMBOL_LIST PREPEND --keep=)
    target_link_options(${target}
        PRIVATE
            ${KEEP_SYMBOL_LIST}
    )

    # Ask armclang to produce a symdefs file that will
    target_link_options(${target}
        PRIVATE
            --symdefs=$<TARGET_FILE_DIR:${target}>/${target}_shared_symbols.txt
    )
endmacro()

macro(target_link_shared_code target)
    get_target_property(TARGET_SOURCE_DIR ${target} SOURCE_DIR)

    foreach(symbol_provider ${ARGN})
        if (TARGET ${symbol_provider})
            get_target_property(SYMBOL_PROVIDER_TYPE ${symbol_provider} TYPE)
            if (NOT SYMBOL_PROVIDER_TYPE STREQUAL "EXECUTABLE")
                message(FATAL_ERROR "${symbol_provider} is not an executable. Symbols cannot be shared from libraries.")
            endif()
        endif()

        add_dependencies(${target} ${symbol_provider})
        # Some cmake functions don't allow generator expressions, so get the
        # property as a backup. If the symbol provider hasn't been created yet,
        # then use the output dir of the target.
        if (TARGET ${symbol_provider})
            get_target_property(SYMBOL_PROVIDER_OUTPUT_DIR ${symbol_provider} RUNTIME_OUTPUT_DIRECTORY)
        else()
            get_target_property(SYMBOL_PROVIDER_OUTPUT_DIR ${target} RUNTIME_OUTPUT_DIRECTORY)
        endif()
        # Set these properties so that cmake will allow us to use a source file
        # that doesn't exist at cmake-time, but we guarantee will exist at
        # compile-time.
        set_source_files_properties(${SYMBOL_PROVIDER_OUTPUT_DIR}/${symbol_provider}_shared_symbols.txt
            DIRECTORY ${TARGET_SOURCE_DIR}
            PROPERTIES
                EXTERNAL_OBJECT true
                GENERATED true
        )
        target_sources(${target}
            PRIVATE
                $<TARGET_FILE_DIR:${symbol_provider}>/${symbol_provider}_shared_symbols.txt
        )
    endforeach()
endmacro()

macro(target_strip_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_strip_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()

macro(target_weaken_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_weaken_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()
