#define core path
set(OM_SOURCE_CORE_DIR "${OM_SOURCE_DIR}/core")
set(OM_INCLUDE_CORE_DIR "${OM_INCLUDE_DIR}/core")
#define execute path
set(OM_SOURCE_EXECUTE_DIR "${OM_SOURCE_DIR}/execute")
set(OM_INCLUDE_EXECUTE_DIR "${OM_INCLUDE_DIR}/execute")
#define import path
set(OM_SOURCE_IMPORT_DIR "${OM_SOURCE_DIR}/import")
set(OM_INCLUDE_IMPORT_DIR "${OM_INCLUDE_DIR}/import")
#define optimize path
set(OM_SOURCE_OPTIMIZE_DIR "${OM_SOURCE_DIR}/optimize")
set(OM_INCLUDE_OPTIMIZE_DIR "${OM_INCLUDE_DIR}/optimize")
#define train path
set(OM_SOURCE_TRAIN_DIR "${OM_SOURCE_DIR}/train")
set(OM_INCLUDE_TRAIN_DIR "${OM_INCLUDE_DIR}/train")
#define train path
set(OM_SOURCE_DEV_DIR "${OM_SOURCE_DIR}/api")

#OM_Interpreter lib binary name
set(OM_INTERPRETER_LIB "onert_micro_interpreter${OM_SUFFIX}")
#OM_Training_Interpreter lib binary name
set(OM_TRAINING_INTERPRETER_LIB "onert_micro_training_interpreter${OM_SUFFIX}")
#Core lib binary name
set(OM_CORE_LIB "onert_micro_core${OM_SUFFIX}")
#Execute lib binary name
set(OM_EXECUTE_LIB "onert_micro_execute${OM_SUFFIX}")
#Import lib binary name
set(OM_IMPORT_LIB "onert_micro_import${OM_SUFFIX}")
#Optimize lib binary name
set(OM_OPTIMIZE_LIB "onert_micro_optimize${OM_SUFFIX}")
#PAL lib binary name
set(OM_PAL_LIB "onert_micro_pal${OM_SUFFIX}")
#Train lib binary name
set(OM_TRAIN_LIB "onert_micro_train${OM_SUFFIX}")
#dev name
set(OM_DEV_LIB "onert_micro_dev${OM_SUFFIX}")

message(STATUS "ONERT MICRO BEGIN")

#build core lib
add_subdirectory(${OM_SOURCE_CORE_DIR})
#build import lib
add_subdirectory(${OM_SOURCE_IMPORT_DIR})
#build execute lib
add_subdirectory(${OM_SOURCE_EXECUTE_DIR})
#build optimize lib
add_subdirectory(${OM_SOURCE_OPTIMIZE_DIR})
#build dev lib
add_subdirectory(${OM_SOURCE_DEV_DIR})

target_link_libraries(${OM_CORE_LIB} PUBLIC ${OM_CIRCLE_SCHEMA})
target_link_libraries(${OM_CORE_LIB} PUBLIC ${OM_IMPORT_LIB})
target_link_libraries(${OM_CORE_LIB} PUBLIC ${OM_EXECUTE_LIB})
target_link_libraries(${OM_CORE_LIB} PUBLIC ${OM_OPTIMIZE_LIB})

add_library(${OM_INTERPRETER_LIB} STATIC OMInterpreter.cpp)

target_include_directories(${OM_INTERPRETER_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_INTERPRETER_LIB} PUBLIC ${OM_CORE_LIB})

# Training part
message (STATUS "ONERT MICRO TRAINING BEGIN")

#build train lib
add_subdirectory(${OM_SOURCE_TRAIN_DIR})

target_link_libraries(${OM_CORE_LIB} PUBLIC ${OM_TRAIN_LIB})

add_library(${OM_TRAINING_INTERPRETER_LIB} STATIC OMTrainingInterpreter.cpp)

target_include_directories(${OM_TRAINING_INTERPRETER_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_TRAINING_INTERPRETER_LIB} PUBLIC ${OM_CORE_LIB})
target_link_libraries(${OM_TRAINING_INTERPRETER_LIB} PUBLIC ${OM_TRAIN_LIB})

message (STATUS "ONERT MICRO TRAINING END")

message(STATUS "ONERT MICRO FINISHED")
