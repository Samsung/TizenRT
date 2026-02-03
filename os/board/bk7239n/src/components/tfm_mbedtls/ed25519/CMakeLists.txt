#-------------------------------------------------------------------------------
# libsodium Ed25519 for TF-M Secure World
# This CMakeLists is used by TF-M build (add_subdirectory from beken platform)
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

# Define the libsodium_ed25519 static library for secure world
add_library(libsodium_ed25519 STATIC)

target_sources(libsodium_ed25519
    PRIVATE
        # Ed25519 core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/add_scalar.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/fe.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ge.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/key_exchange.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/keypair.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/seed.c
        # SHA512 implementation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sha512.c
        # Sign/Verify
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sign.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/verify.c
)

target_include_directories(libsodium_ed25519
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(libsodium_ed25519
    PUBLIC
        CONFIGURED=1
        NATIVE_LITTLE_ENDIAN
    PRIVATE
        USE_HW_SCALAR_OPS=0
        NO_PRECOMP_TABLES=1
)

# Link to PSA interface for secure world crypto operations if needed
# target_link_libraries(libsodium_ed25519
#     PRIVATE
#         psa_interface
# )

