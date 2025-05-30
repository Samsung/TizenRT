message(STATUS "ONERT MICRO IMPORT BUILD BEGIN")

set(SOURCES
        OMExecutionPlanCreator.cpp
        OMKernelConfiguration.cpp
        OMKernelConfigureBuilder.cpp
        OMDynamicShapesHandler.cpp
        OMUtils.cpp
        helpers/OMConfigureSISOKernel.cpp
        helpers/OMPadCommon.cpp
        helpers/OMConfigureTISOKernel.cpp
        helpers/OMSpacesBatchesNDCommon.cpp
        helpers/OMPoolingCommon.cpp
        helpers/OMArgCommon.cpp
        helpers/OMFloorCommon.cpp
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

add_library(${OM_IMPORT_LIB} STATIC ${SOURCES})

target_include_directories(${OM_IMPORT_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_link_libraries(${OM_IMPORT_LIB} PUBLIC ${OM_CORE_LIB})

message(STATUS "ONERT MICRO IMPORT BUILD FINISHED")
