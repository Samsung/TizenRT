cmake_minimum_required (VERSION 3.0)

project (lwm2munittests)

include(${CMAKE_CURRENT_LIST_DIR}/../core/wakaama.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../examples/shared/shared.cmake)

add_definitions(-DLWM2M_CLIENT_MODE -DLWM2M_SUPPORT_JSON)
add_definitions(${SHARED_DEFINITIONS} ${WAKAAMA_DEFINITIONS})
# Enable all warnings for this test build  
add_definitions(-Wall -Wextra -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wswitch-default)  

include_directories (${WAKAAMA_SOURCES_DIR} ${SHARED_INCLUDE_DIRS})


file(GLOB SOURCES "*.c")

add_executable(${PROJECT_NAME} ${SOURCES} ${WAKAAMA_SOURCES} ${SHARED_SOURCES})
target_link_libraries(lwm2munittests cunit)

# Enable CMake Test Framework (CTest) which make testing available by
# a "test" target. For "make" this is "make test"
enable_testing()

add_test (test_all ${PROJECT_NAME})
