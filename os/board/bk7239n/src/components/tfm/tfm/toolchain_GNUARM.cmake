#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Don't load this file if it is specified as a cmake toolchain file
if(NOT TFM_TOOLCHAIN_FILE)
    message(DEPRECATION "SETTING CMAKE_TOOLCHAIN_FILE is deprecated. It has been replaced with TFM_TOOLCHAIN_FILE.")
    return()
endif()

set(CMAKE_SYSTEM_NAME Generic)

find_program(CMAKE_C_COMPILER ${CROSS_COMPILE}-gcc)
find_program(CMAKE_CXX_COMPILER ${CROSS_COMPILE}-g++)

if(CMAKE_C_COMPILER STREQUAL "CMAKE_C_COMPILER-NOTFOUND")
    message(FATAL_ERROR "Could not find compiler: '${CROSS_COMPILE}-gcc'")
endif()

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(LINKER_VENEER_OUTPUT_FLAG -Wl,--cmse-implib,--out-implib=)
set(COMPILER_CMSE_FLAG -mcmse)

# This variable name is a bit of a misnomer. The file it is set to is included
# at a particular step in the compiler initialisation. It is used here to
# configure the extensions for object files. Despite the name, it also works
# with the Ninja generator.
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/set_extensions.cmake)

macro(tfm_toolchain_reset_compiler_flags)
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")

    add_compile_options(
        --specs=nano.specs
        -Wall
        -Wno-format
        -Wno-return-type
        -Wno-unused-but-set-variable
        -c
        -fdata-sections
        -ffunction-sections
        -fno-builtin
        -fshort-enums
        -funsigned-char
        -mthumb
        -nostdlib
        -std=c99
        $<$<OR:$<BOOL:${TFM_DEBUG_SYMBOLS}>,$<BOOL:${TFM_CODE_COVERAGE}>>:-g>
    )
endmacro()

if(CONFIG_TFM_MEMORY_USAGE_QUIET)
    set(MEMORY_USAGE_FLAG "")
else()
    set(MEMORY_USAGE_FLAG LINKER:--print-memory-usage)
endif()

macro(tfm_toolchain_reset_linker_flags)
    set_property(DIRECTORY PROPERTY LINK_OPTIONS "")

    add_link_options(
        --entry=Reset_Handler
        --specs=nano.specs
        LINKER:-check-sections
        LINKER:-fatal-warnings
        LINKER:--gc-sections
        LINKER:--no-wchar-size-warning
        ${MEMORY_USAGE_FLAG}
		${LINKER_CP_OPTION}
    )
endmacro()

macro(tfm_toolchain_set_processor_arch)
    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR ${TFM_SYSTEM_PROCESSOR})

        if (DEFINED TFM_SYSTEM_DSP)
            if (NOT TFM_SYSTEM_DSP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nodsp")
            endif()
        endif()
        if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
            if (DEFINED CONFIG_TFM_FP)
                if(CONFIG_TFM_FP STREQUAL "0" AND
                   NOT TFM_SYSTEM_ARCHITECTURE STREQUAL "armv6-m")
                    string(APPEND CMAKE_SYSTEM_PROCESSOR "+nofp")
                endif()
            endif()
        endif()
    endif()

    # CMAKE_SYSTEM_ARCH variable is not a built-in CMAKE variable. It is used to
    # set the compile and link flags when TFM_SYSTEM_PROCESSOR is not specified.
    # The variable name is choosen to align with the ARMCLANG toolchain file.
    set(CMAKE_SYSTEM_ARCH         ${TFM_SYSTEM_ARCHITECTURE})

    if (DEFINED TFM_SYSTEM_DSP)
        # +nodsp modifier is only supported from GCC version 8.
        # CMAKE_C_COMPILER_VERSION is not guaranteed to be defined.
        EXECUTE_PROCESS( COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION )
        if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
            # armv8.1-m.main arch does not have +nodsp option
            if ((NOT TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main") AND
                NOT TFM_SYSTEM_DSP)
                string(APPEND CMAKE_SYSTEM_ARCH "+nodsp")
            endif()
        endif()
    endif()

    if(GCC_VERSION VERSION_GREATER_EQUAL "8.0.0")
        if (DEFINED CONFIG_TFM_FP)
            if(CONFIG_TFM_FP STREQUAL "hard")
                string(APPEND CMAKE_SYSTEM_ARCH "+fp")
            endif()
        endif()
    endif()
endmacro()

macro(tfm_toolchain_reload_compiler)
    tfm_toolchain_set_processor_arch()
    tfm_toolchain_reset_compiler_flags()
    tfm_toolchain_reset_linker_flags()

    # CMAKE_C_COMPILER_VERSION is not guaranteed to be defined.
    EXECUTE_PROCESS( COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION )

    if (GCC_VERSION VERSION_LESS 7.3.1)
        message(FATAL_ERROR "Please use newer GNU Arm compiler version starting from 7.3.1.")
    endif()

    if (GCC_VERSION VERSION_EQUAL 10.2.1)
        message(FATAL_ERROR "GNU Arm compiler version 10-2020-q4-major has an issue in CMSE support."
                            " Select other GNU Arm compiler versions instead."
                            " See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99157 for the issue detail.")
    endif()

    unset(CMAKE_C_FLAGS_INIT)
    unset(CMAKE_ASM_FLAGS_INIT)

    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_C_FLAGS_INIT "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_ASM_FLAGS_INIT "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_C_LINK_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_ASM_LINK_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
    else()
        set(CMAKE_C_FLAGS_INIT "-march=${CMAKE_SYSTEM_ARCH}")
        set(CMAKE_ASM_FLAGS_INIT "-march=${CMAKE_SYSTEM_ARCH}")
        set(CMAKE_C_LINK_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
        set(CMAKE_ASM_LINK_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
    endif()

    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT})
    set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})


    if (CONFIG_TFM_FP STREQUAL "hard")
        set(COMPILER_CP_FLAG -mfloat-abi=hard -mfpu=${CONFIG_TFM_FP_ARCH})
        set(LINKER_CP_OPTION -mfloat-abi=hard -mfpu=${CONFIG_TFM_FP_ARCH})
        set(BL2_COMPILER_CP_FLAG -mfloat-abi=hard -mfpu=${CONFIG_TFM_FP_ARCH})
    else()
        set(COMPILER_CP_FLAG -mfloat-abi=soft)
        set(LINKER_CP_OPTION -mfloat-abi=soft)
        set(BL2_COMPILER_CP_FLAG -mfloat-abi=soft)
    endif()

    # For GNU Arm Embedded Toolchain doesn't emit __ARM_ARCH_8_1M_MAIN__, adding this macro manually.
    add_compile_definitions($<$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8.1-m.main>:__ARM_ARCH_8_1M_MAIN__>)
endmacro()

# Configure environment for the compiler setup run by cmake at the first
# `project` call in <tfm_root>/CMakeLists.txt. After this mandatory setup is
# done, all further compiler setup is done via tfm_toolchain_reload_compiler()
tfm_toolchain_reload_compiler()

macro(target_add_scatter_file target)
    target_link_options(${target}
        PRIVATE
        -T $<TARGET_OBJECTS:${target}_scatter>
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
            KEEP_EXTENSION True # Don't use .o extension for the preprocessed file
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
            -P
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
        COMMAND ${CMAKE_OBJCOPY}
            -O binary $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.bin
    )

    add_custom_target(${target}_elf
        SOURCES ${bin_dir}/${target}.elf
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.elf
        DEPENDS ${target}
        COMMAND ${CMAKE_OBJCOPY}
            -O elf32-littlearm $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.elf
    )

    add_custom_target(${target}_hex
        SOURCES ${bin_dir}/${target}.hex
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.hex
        DEPENDS ${target}
        COMMAND ${CMAKE_OBJCOPY}
            -O ihex $<TARGET_FILE:${target}>
            ${bin_dir}/${target}.hex
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


    list(TRANSFORM KEEP_SYMBOL_LIST PREPEND  --keep-symbol=)
    # strip all the symbols except those proveded as arguments
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${target}> --wildcard ${KEEP_SYMBOL_LIST} --strip-all $<TARGET_FILE_DIR:${target}>/${target}_shared_symbols.axf
    )
endmacro()

macro(target_link_shared_code target)
    foreach(symbol_provider ${ARGN})
        if (TARGET ${symbol_provider})
            get_target_property(SYMBOL_PROVIDER_TYPE ${symbol_provider} TYPE)
            if (NOT SYMBOL_PROVIDER_TYPE STREQUAL "EXECUTABLE")
                message(FATAL_ERROR "${symbol_provider} is not an executable. Symbols cannot be shared from libraries.")
            endif()
        endif()

        add_dependencies(${target} ${symbol_provider})
        target_link_options(${target} PRIVATE LINKER:-R$<TARGET_FILE_DIR:${symbol_provider}>/${symbol_provider}_shared_symbols.axf)
    endforeach()
endmacro()

macro(target_strip_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

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

    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CROSS_COMPILE}-objcopy
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()
