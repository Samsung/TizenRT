cmake_minimum_required(VERSION 3.0)
enable_testing()

if (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-long-long -std=c++11")
endif()

add_subdirectory(HippoMocksTest)

install(FILES ${PROJECT_SOURCE_DIR}/HippoMocks/hippomocks.h
        DESTINATION include/)
