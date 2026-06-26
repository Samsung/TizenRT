message(STATUS "ONERT MICRO EXECUTE BUILD BEGIN")

include("${OM_PAL_DIR}/pal.cmake")

initialize_pal()

if (NOT PAL_INITIALIZED)
    message(STATUS "ERROR: PAL Failed to initialize, skip BUILD EXECUTE")
    return()
endif()

set(SOURCES
        OMKernelExecute.cpp
        OMKernelExecutionBuilder.cpp
        OMRuntimeKernel.cpp
        OMUtils.cpp
        kernels/ReluCommon.cpp
        kernels/ConvolutionCommon.cpp
        kernels/MathCommon.cpp
        kernels/PoolingCommon.cpp
        kernels/ArgCommon.cpp
        kernels/ReadKernelDataCommon.cpp
        kernels/ReshapeCommon.cpp
        kernels/SpacesBatchesNDCommon.cpp
        )

# Add configure kernels
macro(REGISTER_KERNEL OPERATOR, NODE)
    list(APPEND SOURCES "kernels/${NODE}.cpp")
endmacro(REGISTER_KERNEL)

# To add REGISTER_KERNEL list
include(${KERNEL_REGISTER_FILE})

macro(REGISTER_CUSTOM_KERNEL NODE)
    list(APPEND SOURCES "kernels/${NODE}.cpp")
endmacro(REGISTER_CUSTOM_KERNEL)

# To add CUSTOM_REGISTER_KERNEL list
include(${CUSTOM_KERNEL_REGISTER_FILE})

add_library(${OM_EXECUTE_LIB} STATIC ${SOURCES})

target_include_directories(${OM_EXECUTE_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_EXECUTE_LIB} PUBLIC ${OM_CORE_LIB})

target_include_directories(${OM_EXECUTE_LIB} PUBLIC ${OM_PAL_COMMON_DIR})
add_pal_to_target(${OM_EXECUTE_LIB})

message(STATUS "ONERT MICRO EXECUTE BUILD FINISHED")

if(NOT ENABLE_TEST)
    return()
endif(NOT ENABLE_TEST)

nnas_find_package(GTest REQUIRED)

macro(REGISTER_KERNEL OPERATOR, NODE)
    list(APPEND TEST_SOURCES "kernels/tests/${NODE}.test.cpp")
endmacro(REGISTER_KERNEL)

include(${KERNEL_REGISTER_FILE})

macro(REGISTER_CUSTOM_KERNEL NODE)
    list(APPEND TEST_SOURCES "kernels/tests/${NODE}.test.cpp")
endmacro(REGISTER_CUSTOM_KERNEL)

# To add CUSTOM_REGISTER_KERNEL list
include(${CUSTOM_KERNEL_REGISTER_FILE})

list(APPEND TEST_SOURCES OMTestUtils.cpp)

GTest_AddTest(${OM_EXECUTE_LIB}_kernels_test ${TEST_SOURCES})
target_include_directories(${OM_EXECUTE_LIB}_kernels_test PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_EXECUTE_LIB}_kernels_test ${OM_INTERPRETER_LIB})
target_link_libraries(${OM_EXECUTE_LIB}_kernels_test onert_micro_coverage)
target_link_libraries(${OM_EXECUTE_LIB} PUBLIC onert_micro_coverage)
