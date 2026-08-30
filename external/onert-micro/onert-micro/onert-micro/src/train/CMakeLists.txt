message(STATUS "ONERT MICRO TRAIN BUILD BEGIN")

include("${OM_PAL_DIR}/pal.cmake")

initialize_pal()

if (NOT PAL_INITIALIZED)
    message(STATUS "ERROR: PAL Failed to initialize, skip BUILD EXECUTE")
    return()
endif()

set(SOURCES
        OMBackpropExecute.cpp
        OMBackpropExecutionBuilder.cpp
        losses_functions/MSE.cpp
        losses_functions/CrossEntropy.cpp
        losses_functions/SparseCrossEntropy.cpp
        metrics/CrossEntropy.cpp
        metrics/MAE.cpp
        metrics/MSE.cpp
        metrics/Accuracy.cpp
        metrics/SparseCrossEntropyAccuracy.cpp
        train_optimizers/SGD.cpp
        train_optimizers/Adam.cpp
        )

# Add configure kernels
macro(REGISTER_TRAIN_KERNEL OPERATOR, NODE)
    list(APPEND SOURCES "kernels/${NODE}.cpp")
endmacro(REGISTER_TRAIN_KERNEL)

# To add REGISTER_KERNEL list
include(${KERNEL_TRAIN_REGISTER_FILE})

add_library(${OM_TRAIN_LIB} STATIC ${SOURCES})

target_include_directories(${OM_TRAIN_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_TRAIN_LIB} PUBLIC ${OM_CORE_LIB})

message(STATUS "ONERT MICRO TRAIN BUILD FINISHED")

if(NOT ENABLE_TEST)
    return()
endif(NOT ENABLE_TEST)

message(STATUS "ONERT MICRO TEST TRAIN BUILD STARTED")

nnas_find_package(GTest REQUIRED)

set (TEST_SOURCES
        tests/BostonHousingTask.test.cpp
        tests/CheckpointsHandler.test.cpp
        tests/NumbersClassificationTask.test.cpp)

GTest_AddTest(${OM_TRAIN_LIB}_test ${TEST_SOURCES})
target_include_directories(${OM_TRAIN_LIB}_test PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_TRAIN_LIB}_test ${OM_TRAINING_INTERPRETER_LIB})
target_link_libraries(${OM_TRAIN_LIB}_test onert_micro_coverage)
target_link_libraries(${OM_TRAIN_LIB} PUBLIC onert_micro_coverage)
