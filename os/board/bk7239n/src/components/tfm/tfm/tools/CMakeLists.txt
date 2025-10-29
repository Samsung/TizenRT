#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)
find_package(Python3)

############################### Manifest declaration ###########################

list(APPEND TEMP_MANIFEST_LISTS ${TFM_MANIFEST_LIST})

if (TFM_NS_REG_TEST OR TFM_S_REG_TEST)
    list(APPEND TEMP_MANIFEST_LISTS ${TFM_TEST_PATH}/secure_fw/tfm_test_manifest_list.yaml)
endif()

if ("${TEST_PSA_API}" STREQUAL "IPC")
    list(APPEND TEMP_MANIFEST_LISTS ${CMAKE_CURRENT_SOURCE_DIR}/tfm_psa_ff_test_manifest_list.yaml)
endif()

if (TFM_EXTRA_MANIFEST_LIST_FILES)
    list(APPEND TEMP_MANIFEST_LISTS ${TFM_EXTRA_MANIFEST_LIST_FILES})
endif()

# Build up the manifest list arrays:
# - CONFIGURED_MANIFEST_LISTS:
#       Array of Manifest lists under build directory which are the output of configure_file().
# - MANIFEST_LIST_PATHS:
#       Array of paths of the input manifest lists of configure_file().
#       They are NOT the paths of CONFIGURED_MANIFEST_LISTS.
#       They can be used to build up manifest file paths if manifest file paths are
#       relative ones in the manifest lists.
# - COMBINED_LIST:
#       A combined list of the above two, with the following format:
#           [configured_list_a, path_of_list_a, configured_list_b, path_of_list_b ... ]
set(POSTFIX 1)

foreach(MANIFEST_LIST IN LISTS TEMP_MANIFEST_LISTS)
    if (NOT EXISTS ${MANIFEST_LIST})
        message(FATAL_ERROR "Manifest list ${MANIFEST_LIST} doesn't exist")
    endif()

    get_filename_component(MANIFEST_LIST_NAME ${MANIFEST_LIST} NAME_WLE)
    set(CONFIGURED_LIST
        ${CMAKE_CURRENT_BINARY_DIR}/${MANIFEST_LIST_NAME}_${POSTFIX}.yaml)

    configure_file(${MANIFEST_LIST} ${CONFIGURED_LIST})
    list(APPEND CONFIGURED_MANIFEST_LISTS ${CONFIGURED_LIST})
    list(APPEND COMBINED_LIST             ${CONFIGURED_LIST})

    get_filename_component(MANIFEST_LIST_PATH ${MANIFEST_LIST} DIRECTORY)
    list(APPEND MANIFEST_LIST_PATHS ${MANIFEST_LIST_PATH})
    list(APPEND COMBINED_LIST       ${MANIFEST_LIST_PATH})

    math(EXPR POSTFIX "${POSTFIX} + 1")
endforeach()

############################### File list declaration ##########################

set(GENERATED_FILE_LISTS ${CMAKE_CURRENT_SOURCE_DIR}/tfm_generated_file_list.yaml)
set(GENERATED_FILE_LISTS ${GENERATED_FILE_LISTS} ${TFM_EXTRA_GENERATED_FILE_LIST_PATH})

############################### Dependency generation ##########################

function(parse_field_from_yaml files field output_variable)
    set(local_variable "")
    foreach(yaml_file ${files})
        # Load the lines that refer to the key we selected
        file(STRINGS ${yaml_file} temp_variable REGEX " *\"${field}\":")
        # Take only the value of the key
        list(TRANSFORM temp_variable REPLACE " *\"${field}\": *" ";")
        # Remove all commas
        list(TRANSFORM temp_variable REPLACE "," "")
        # Remove all quote marks
        list(TRANSFORM temp_variable REPLACE "\"" "")
        list(APPEND local_variable ${temp_variable})
    endforeach()
    set(${output_variable} ${local_variable} PARENT_SCOPE)
endfunction()

parse_field_from_yaml("${GENERATED_FILE_LISTS}" template TEMPLATE_FILES)
# Replace relative paths with absolute paths
# Paths used in GENERATED_FILE_LISTS are all relative to TF-M root (${CMAKE_SOURCE_DIR})
list(TRANSFORM TEMPLATE_FILES REPLACE "^([^/\\][^:].*)" "${CMAKE_SOURCE_DIR}/\\1")

parse_field_from_yaml("${GENERATED_FILE_LISTS}" output OUTPUT_FILES)
# Replace relative paths with absolute paths
# Paths used in GENERATED_FILE_LISTS are all relative to TF-M root (${CMAKE_SOURCE_DIR})
list(TRANSFORM OUTPUT_FILES REPLACE "^([^/\\][^:].*)" "${CMAKE_BINARY_DIR}/generated/\\1")

# Each manifest list may have different original path
# Parse them one by one
set(INDEX 0)
foreach(CONFIGURED_LIST ${CONFIGURED_MANIFEST_LISTS})
    list(GET MANIFEST_LIST_PATHS ${INDEX} PATH_OF_LIST)

    parse_field_from_yaml(${CONFIGURED_LIST} manifest MANIFESTS)
    foreach(MANIFEST ${MANIFESTS})
        # The path of each manifest must be absolute path or relative path to
        # the path of manifest list that holds it
        if (NOT IS_ABSOLUTE ${MANIFEST})
            set(MANIFEST "${PATH_OF_LIST}/${MANIFEST}")
        endif()
        list(APPEND MANIFEST_FILES ${MANIFEST})
    endforeach()

    math(EXPR INDEX "${INDEX} + 1")
endforeach()

############################### Command declaration ############################

# Workaround for heap support
if ("${TEST_PSA_API}" STREQUAL "IPC")
    execute_process(
        WORKING_DIRECTORY ${PSA_ARCH_TESTS_PATH}/api-tests
        COMMAND ${Python3_EXECUTABLE} tools/scripts/manifest_update.py
    )
endif()

add_custom_target(tfm_generated_files
    SOURCES ${OUTPUT_FILES}
)

if (CONFIG_TFM_PARSE_MANIFEST_QUIET)
    set(PARSE_MANIFEST_QUIET_FLAG "-q")
else()
    set(PARSE_MANIFEST_QUIET_FLAG "")
endif()

add_custom_command(OUTPUT ${OUTPUT_FILES}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tfm_parse_manifest_list.py
                                  -m ${COMBINED_LIST}
                                  -f ${GENERATED_FILE_LISTS}
                                  -l ${TFM_ISOLATION_LEVEL}
                                  -b ${CONFIG_TFM_SPM_BACKEND}
                                  -o ${CMAKE_BINARY_DIR}/generated
                                  ${PARSE_MANIFEST_QUIET_FLAG}
    DEPENDS ${TEMPLATE_FILES} ${MANIFEST_FILES}
    DEPENDS ${TEMP_MANIFEST_LISTS} ${CONFIGURED_MANIFEST_LISTS} ${GENERATED_FILE_LISTS}
)

# The files need to be generated before cmake will allow them to be used as
# sources. Due to issue with custom_command scoping the easiest way to do this
# is to run the script at cmake-time.
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tfm_parse_manifest_list.py
                                  -m ${COMBINED_LIST}
                                  -f ${GENERATED_FILE_LISTS}
                                  -l ${TFM_ISOLATION_LEVEL}
                                  -b ${CONFIG_TFM_SPM_BACKEND}
                                  -o ${CMAKE_BINARY_DIR}/generated
                                  ${PARSE_MANIFEST_QUIET_FLAG}
    RESULT_VARIABLE RET
)

if(RET EQUAL 0)
    include(${CMAKE_BINARY_DIR}/generated/tools/config_impl.cmake)
else()
    message(FATAL_ERROR "File generation failed")
endif()
