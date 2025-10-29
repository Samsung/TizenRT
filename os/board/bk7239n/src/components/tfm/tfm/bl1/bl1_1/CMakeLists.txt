#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_subdirectory(lib)
add_subdirectory(shared_lib)
add_executable(bl1_1)

set_target_properties(bl1_1
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_convert_to_bin_target(bl1_1)

target_link_options(bl1_1
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl1_1.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/bl1_1.map>
)

target_sources(bl1_1
    PRIVATE
        main.c
)

target_link_libraries(bl1_1
    PRIVATE
        bl1_1_lib
        bl1_1_shared_lib
        platform_bl1
        $<$<BOOL:${TEST_BL1_1}>:bl1_1_tests>
)

target_compile_definitions(bl1_1
    PRIVATE
        $<$<BOOL:${TEST_BL1_1}>:TEST_BL1_1>
)

target_share_symbols(bl1_1 ${CMAKE_CURRENT_SOURCE_DIR}/bl1_1_shared_symbols.txt)

################################################################################

add_library(signing_layout_bl1_2 OBJECT signing_layout.c)
target_compile_options(signing_layout_bl1_2
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-E\;-xc>
        $<$<C_COMPILER_ID:ARMClang>:-E\;-xc>
        $<$<C_COMPILER_ID:IAR>:--preprocess=ns\;$<TARGET_OBJECTS:signing_layout_bl1_2>>
)
target_link_libraries(signing_layout_bl1_2
    PRIVATE
        platform_region_defs
)

################################################################################
get_target_property(bin_dir bl1_2 RUNTIME_OUTPUT_DIRECTORY)

add_custom_target(bl1_2_padded_bin
    ALL
    SOURCES bl1_2_padded.bin
    SOURCES bl1_2_padded_hash.bin
)

add_custom_command(OUTPUT bl1_2_padded.bin bl1_2_padded_hash.bin
    OUTPUT ${bin_dir}/bl1_2_padded.bin ${bin_dir}/bl1_2_padded_hash.bin
    DEPENDS $<TARGET_FILE_DIR:bl1_2>/bl1_2.bin bl1_2_bin signing_layout_bl1_2
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_bl1_2_img.py
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_bl1_2_img.py
                    --input_file $<TARGET_FILE_DIR:bl1_2>/bl1_2.bin
                    --img_output_file bl1_2_padded.bin
                    --hash_output_file bl1_2_padded_hash.bin
                    --signing_layout_file $<TARGET_OBJECTS:signing_layout_bl1_2>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/bl1_2_padded.bin $<TARGET_FILE_DIR:bl1_2>
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/bl1_2_padded_hash.bin $<TARGET_FILE_DIR:bl1_2>
)

################################################################################

add_custom_target(bl1_provisioning_bundle
    ALL
    SOURCES bl1_provisioning_bundle.bin
)

add_custom_command(OUTPUT bl1_provisioning_bundle.bin
    DEPENDS ${TFM_BL2_ENCRYPTION_KEY_PATH}
    DEPENDS $<TARGET_FILE_DIR:bl1_2>/bl1_2_padded_hash.bin
    DEPENDS $<TARGET_FILE_DIR:bl2>/bl2_signed_hash.bin
    DEPENDS $<TARGET_FILE_DIR:bl1_2>/bl1_2_padded.bin
    bl1_2_padded_bin bl2_signed_bin
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_provisioning_bundle.py
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/create_provisioning_bundle.py
                    --bl2_encryption_key_input_file ${TFM_BL2_ENCRYPTION_KEY_PATH}
                    --bl1_2_padded_hash_input_file $<TARGET_FILE_DIR:bl1_2>/bl1_2_padded_hash.bin
                    --bl2_signed_hash_input_file $<TARGET_FILE_DIR:bl2>/bl2_signed_hash.bin
                    --bl1_2_input_file $<TARGET_FILE_DIR:bl1_2>/bl1_2_padded.bin
                    --bundle_output_file bl1_provisioning_bundle.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/bl1_provisioning_bundle.bin $<TARGET_FILE_DIR:bl1_1>
)
