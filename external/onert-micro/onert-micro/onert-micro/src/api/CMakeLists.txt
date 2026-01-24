message(STATUS "ONERT MICRO DEV BUILD BEGIN")

set(SOURCES
    onert-micro.cpp)

add_library(${OM_DEV_LIB} STATIC ${SOURCES})
target_compile_options(${OM_DEV_LIB} PRIVATE "-fexceptions")
target_link_libraries(${OM_DEV_LIB} PUBLIC ${OM_TRAININFO_SCHEMA})
target_include_directories(${OM_DEV_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_DEV_LIB} PUBLIC ${OM_INTERPRETER_LIB})
target_link_libraries(${OM_DEV_LIB} PUBLIC ${OM_TRAINING_INTERPRETER_LIB})
target_link_libraries(${OM_DEV_LIB} PUBLIC onert_micro_coverage)

message(STATUS "ONERT MICRO DEV BUILD FINISHED")
