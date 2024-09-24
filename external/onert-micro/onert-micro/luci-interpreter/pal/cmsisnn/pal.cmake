macro(initialize_pal)
    nnas_find_package(CMSIS-NN EXACT 4.1.0 REQUIRED)

    if (NOT CMSIS_NNSource_FOUND)
        message(STATUS "Skipping luci-interpreter: CMSIS-NN not found")
        return()
    endif ()

    set(PAL_INITIALIZED TRUE)
endmacro()

macro(add_pal_to_target TGT)
    target_include_directories(${TGT} PUBLIC ${LUCI_INTERPRETER_PAL_DIR})

    file(GLOB_RECURSE PAL_SOURCES "${CMSIS_NNSource_DIR}/Source/ActivationFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/BasicMathFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/ConcatenationFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/ConvolutionFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/FullyConnectedFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/LSTMFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/NNSupportFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/PoolingFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/ReshapeFunctions/*.c"
            "${CMSIS_NNSource_DIR}/Source/SoftmaxFunctions/*.c")
    add_library(luci_interpreter_cmsisnn_pal STATIC ${PAL_SOURCES})
    set_property(TARGET luci_interpreter_cmsisnn_pal PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(luci_interpreter_cmsisnn_pal PRIVATE
            "${CMSIS_NNSource_DIR}")
    add_subdirectory(${CMSIS_NNSource_DIR} ${CMAKE_CURRENT_BINARY_DIR}/CMSISNN)
    target_include_directories(luci_interpreter_cmsisnn_pal PUBLIC
            "${CMSIS_NNSource_DIR}/Include")
    target_link_libraries(${TGT} PRIVATE luci_interpreter_cmsisnn_pal)
endmacro()
