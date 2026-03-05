message(STATUS "ONERT MICRO OPTIMIZER BUILD BEGIN")

set(SOURCES
        OMOptimizer.cpp
        )

# Add configure kernels
macro(REGISTER_PASS PASS_NAME)
    list(APPEND SOURCES "pass/${PASS_NAME}.cpp")
endmacro(REGISTER_PASS)

# To add REGISTER_KERNEL list
include(${PASS_REGISTER_FILE})

add_library(${OM_OPTIMIZE_LIB} STATIC ${SOURCES})

target_include_directories(${OM_OPTIMIZE_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_OPTIMIZE_LIB} PUBLIC ${OM_CORE_LIB})

message(STATUS "ONERT MICRO OPTIMIZER BUILD FINISHED")
