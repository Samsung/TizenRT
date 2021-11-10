cmake_minimum_required(VERSION 3.8)
set(PROJECT netlog)
set(ROOT_PATH "../../../")
string(APPEND EXT_INC_PATH "${ROOT_PATH}" "external/include")
string(APPEND OS_INC_PATH "${ROOT_PATH}" "os/include")

project(${PROJECT})

message("message pkbuild: " "${ROOT_PATH}   ${EXT_INC_PATH}sss")
# C++ settings
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE)
set(CMAKE_BUILD_TYPE Debug)

# set(HEADERS
# "${EXT_INC_PATH}/stress_tool/st_perf.h"
#  )

# set(SOURCES
# 	Printer.cc
# )

set(MAIN
netlog_test.c
)

# set(SOURCES
#     VideoStoreTest.cc
# )


add_executable(${PROJECT} ${MAIN} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC "/usr/inc")
target_include_directories(${PROJECT} PUBLIC "${EXT_INC_PATH}")
#target_include_directories(${PROJECT} PUBLIC "${OS_INC_PATH}")

#add_library(videoStore STATIC ${SOURCES} ${HEADERS})
# add_subdirectory(src)
# add_subdirectory(lib)
# add_subdirectory(test)
