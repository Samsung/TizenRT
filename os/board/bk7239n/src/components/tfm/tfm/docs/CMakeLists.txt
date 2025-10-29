#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_custom_target(docs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Python3)
find_package(Sphinx)

# suppress warnings on mismatched names in PythonModules
set(FPHSA_NAME_MISMATCHED True)
find_package(PythonModules COMPONENTS m2r2 sphinx-rtd-theme sphinxcontrib.plantuml)
find_package(PlantUML)
find_package(Doxygen 1.8.0)
find_package(LATEX COMPONENTS PDFLATEX)

################################## ENV #########################################

include(../cmake/version.cmake)

project("Trusted Firmware M. Documentation" VERSION ${TFM_VERSION} LANGUAGES)

set(SPHINXCFG_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINXCFG_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINXCFG_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/user_guide)

set(DOXYCFG_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/reference_manual)
set(DOXYCFG_DOXYGEN_CFG_DIR ${CMAKE_SOURCE_DIR}/doxygen)

# Enable to request the interface to build the doxygen documentation as well
set(DOXYCFG_DOXYGEN_BUILD False)

################################## SPHINX ######################################

if (SPHINX_FOUND AND PLANTUML_FOUND AND PY_M2R2_FOUND AND PY_SPHINX-RTD-THEME_FOUND AND PY_SPHINXCONTRIB.PLANTUML)

    file(GLOB_RECURSE SPHINXCFG_DOC_FILES ${SPHINXCFG_SOURCE_PATH}/*.rst)

    add_custom_target(tfm_docs_userguide_html ALL
        COMMAND "${SPHINX_EXECUTABLE}" -W -b html -j auto -c ${SPHINXCFG_CONFIG_PATH} ${SPHINXCFG_SOURCE_PATH} "${SPHINXCFG_OUTPUT_PATH}/html"
        WORKING_DIRECTORY ${SPHINXCFG_SOURCE_PATH}
        DEPENDS ${SPHINXCFG_DOC_FILES}
    )

    add_dependencies(docs tfm_docs_userguide_html)

    if (LATEX_PDFLATEX_FOUND)
        add_custom_command(OUTPUT "${SPHINXCFG_OUTPUT_PATH}/latex/TF-M.tex"
            OUTPUT "${SPHINXCFG_OUTPUT_PATH}/latex/"
            COMMAND "${SPHINX_EXECUTABLE}" -b latex "${SPHINX_TMP_DOC_DIR}" "${SPHINXCFG_OUTPUT_PATH}/latex"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            DEPENDS tfm_docs_sphinx_cfg
            DEPENDS ${SPHINXCFG_DOC_FILES}
        )
        add_custom_command(OUTPUT "${SPHINXCFG_OUTPUT_PATH}/latex/TF-M.pdf"
            COMMAND ${PDFLATEX_COMPILER} TF-M.tex
            COMMAND ${CMAKE_COMMAND} -E copy TF-M.tex ${SPHINXCFG_OUTPUT_PATH}/tf-m_user_guide.pdf
            WORKING_DIRECTORY ${SPHINXCFG_OUTPUT_PATH}/latex/
            DEPENDS "${SPHINXCFG_OUTPUT_PATH}/latex/TF-M.tex"
        )
        add_custom_target(tfm_docs_userguide_pdf
            DEPENDS "${SPHINXCFG_OUTPUT_PATH}/latex/TF-M.pdf"
        )
        add_dependencies(docs tfm_docs_userguide_pdf)
    endif()
endif()

################################## DOXYGEN #####################################

if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND AND PLANTUML_FOUND)

    configure_file(${CMAKE_SOURCE_DIR}/doxygen/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    file(GLOB_RECURSE DOXYCFG_DOC_FILES ${CMAKE_SOURCE_DIR}/*.c ${CMAKE_SOURCE_DIR}/*.h)

    add_custom_command(OUTPUT ${DOXYCFG_OUTPUT_PATH}/html
        OUTPUT ${DOXYCFG_OUTPUT_PATH}/latex
        OUTPUT ${DOXYCFG_OUTPUT_PATH}/latex/refman.pdf
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${DOXYCFG_DOC_FILES}
    )
    add_custom_target(tfm_docs_refman_html ALL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${DOXYCFG_OUTPUT_PATH}/html
    )
    add_dependencies(docs tfm_docs_refman_html)

    if (LATEX_PDFLATEX_FOUND)
        add_custom_command(OUTPUT "${DOXYCFG_OUTPUT_PATH}/latex/refman.pdf"
            COMMAND "${PDFLATEX_COMPILER} refman.tex"
            COMMAND ${CMAKE_COMMAND} -E copy refman.tex ${DOXYCFG_OUTPUT_PATH}/tf-m_reference_manual.pdf
            WORKING_DIRECTORY ${DOXYCFG_OUTPUT_PATH}/latex/
            DEPENDS "${DOXYCFG_OUTPUT_PATH}/latex/refman.tex"
        )
        add_custom_target(tfm_docs_refman_pdf
            DEPENDS ${DOXYCFG_OUTPUT_PATH}/latex/refman.pdf
            DEPENDS tfm_docs_refman_html
        )
        add_dependencies(docs tfm_docs_refman_pdf)
    endif()
endif()
