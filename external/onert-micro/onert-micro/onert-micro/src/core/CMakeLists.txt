message(STATUS "ONERT MICRO CORE BUILD BEGIN")

set(SOURCES
        OMKernelType.cpp
        OMRuntimeContext.cpp
        OMRuntimeStorage.cpp
        OMTrainingRuntimeModule.cpp
        OMRuntimeGraph.cpp
        OMRuntimeModule.cpp
        OMUtils.cpp
        OMDataType.cpp
        train/OMTrainingHandler.cpp
        train/OMTrainingStorage.cpp
        train/OMCheckpointSaver.cpp
        train/OMCheckpointLoader.cpp
        memory/OMMemoryManager.cpp
        memory/OMRuntimeAllocator.cpp
        reader/OMCircleReader.cpp
        reader/OMWeightOnlyFormatReader.cpp
        reader/OMTrainingConfigFileReader.cpp)

add_library(${OM_CORE_LIB} STATIC ${SOURCES})

target_include_directories(${OM_CORE_LIB} PUBLIC "${OM_INCLUDE_DIR}")
target_include_directories(${OM_CORE_LIB} PUBLIC "${GENERATED_INCLUDE_DIR}")

message(STATUS "ONERT MICRO CORE BUILD FINISHED")
