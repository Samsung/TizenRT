cmake_minimum_required (VERSION 2.8)

if (CMAKE_MAJOR_VERSION GREATER 2)
	cmake_policy(SET CMP0042 NEW)
endif()

project (wakaama-client)

set(LIB_VERSION_MAJOR "1")
set(LIB_VERSION_MINOR "1")
set(LIB_VERSION_PATCH "0")

set(SOURCES
        lwm2mclient.c
        system_api.c
        object_security.c
        object_server.c
        object_device.c
        object_firmware.c
        object_location.c
        object_connectivity_moni.c
        object_connectivity_stat.c
        object_access_control.c
    )

add_definitions(-DLWM2M_CLIENT_MODE -DLWM2M_SUPPORT_JSON)

set(OPENSSL_SRC_DIR "${CMAKE_SOURCE_DIR}/external/openssl")
set(OPENSSL_LIBRARIES ${OPENSSL_SRC_DIR}/libssl.a ${OPENSSL_SRC_DIR}/libcrypto.a)
set(OPENSSL_INCLUDE_DIR "${OPENSSL_SRC_DIR}/include")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENSSL_CONFIG_CMD ./Configure darwin64-x86_64-cc)
else()
    if(NOT DEFINED OPENSSL_CONFIG_CMD)
        set(OPENSSL_CONFIG_CMD ./config)
    endif()
    set(OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} dl pthread)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/libwakaama-client.version -Wl,--strip-all")
endif()

add_custom_command(OUTPUT openssl
          COMMAND ${OPENSSL_CONFIG_CMD}
          COMMAND make
          WORKING_DIRECTORY ${OPENSSL_SRC_DIR}
          COMMENT "Building OpenSSL libraries")

add_custom_target(openssl-libs DEPENDS openssl)

include_directories(${OPENSSL_INCLUDE_DIR})

add_library(${PROJECT_NAME} SHARED ${WAKAAMA_SOURCES} ${SHARED_SOURCES} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}
    SOVERSION ${LIB_VERSION_MAJOR}
    OUTPUT_NAME ${PROJECT_NAME})

# Add WITH_LOGS to debug variant
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:WITH_LOGS>)

target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
add_dependencies (${PROJECT_NAME} openssl-libs)

install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install (FILES lwm2mclient.h DESTINATION /usr/include)
