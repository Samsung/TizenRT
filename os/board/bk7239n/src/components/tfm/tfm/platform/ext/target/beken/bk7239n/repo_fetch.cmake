include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(MBEDCRYPTO_PATH ${ARMINO_SDK_PATH}/components/tfm_mbedtls/mbedtls CACHE PATH "Path to MCUBOOT (or DOWNLOAD to get automatically" FORCE)

if ("${MBEDCRYPTO_PATH}" STREQUAL "")
    set(MBEDCRYPTO_VERSION                  "12d45fb" CACHE STRING "The version of Mbed Crypto to use")
    set(MBEDCRYPTO_GIT_REMOTE               "ssh://${USER}@192.168.0.46:29418/iot/wcn/components/tfm_mbedtls" CACHE STRING "The URL (or path) to retrieve MbedTLS from.")

    file(GLOB PATCH_FILES ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch)
    file(GLOB BK_PATCH_FILES ${CMAKE_SOURCE_DIR}/platform/ext/target/beken/patch/*.patch)
    list(APPEND PATCH_FILES "${BK_PATCH_FILES}")

    find_package(Git)
    if (PATCH_FILES)
        FetchContent_Declare(mbedcrypto
            GIT_REPOSITORY ${MBEDCRYPTO_GIT_REMOTE}
            GIT_TAG ${MBEDCRYPTO_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
            PATCH_COMMAND ${GIT_EXECUTABLE} apply ${PATCH_FILES}
        )
    else()
        FetchContent_Declare(mbedcrypto
            GIT_REPOSITORY ${MBEDCRYPTO_GIT_REMOTE}
            GIT_TAG ${MBEDCRYPTO_VERSION}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
        )
    endif()
    
    FetchContent_GetProperties(mbedcrypto)
    if(NOT mbedcrypto_POPULATED)
        FetchContent_Populate(mbedcrypto)
        set(MBEDCRYPTO_PATH ${mbedcrypto_SOURCE_DIR} CACHE PATH "Path to mbed-crypto (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()


set(MCUBOOT_IMAGE_VERSION_S             0.0.1 CACHE STRING "Version number of S image")
set(MCUBOOT_IMAGE_VERSION_NS            0.0.1 CACHE STRING "Version number of NS image")
set(MCUBOOT_PATH ${ARMINO_SDK_PATH}/components/mcuboot CACHE PATH "Path to MCUBOOT (or DOWNLOAD to get automatically" FORCE)

if (NOT MCUBOOT_PATH)
    set(MCUBOOT_VERSION                     "60d4e39c"  CACHE STRING    "The version of MCUboot to use")
    FetchContent_Declare(mcuboot
        GIT_REPOSITORY ssh://${USER}@192.168.0.46:29418/iot/wcn/bootloader/mcuboot
        GIT_TAG ${MCUBOOT_VERSION}
        GIT_SHALLOW FALSE
        GIT_PROGRESS TRUE
        GIT_SUBMODULES "${MCUBOOT_SUBMODULES}"
    )
    
    FetchContent_GetProperties(mcuboot)
    if(NOT mcuboot_POPULATED)
        FetchContent_Populate(mcuboot)
        set(MCUBOOT_PATH ${mcuboot_SOURCE_DIR} CACHE PATH "Path to MCUBOOT (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()

set(TFM_TEST_REPO_PATH ${ARMINO_SDK_PATH}/components/tfm_tests CACHE PATH "Path to TFM-TEST repo (or DOWNLOAD to fetch automatically" FORCE)

if (NOT TFM_TEST_REPO_PATH)
set(TFM_TEST_REPO_VERSION               "66103cc"   CACHE STRING    "The version of tf-m-tests to use")
FetchContent_Declare(tfm_test_repo
    GIT_REPOSITORY ssh://${USER}@192.168.0.46:29418/iot/wcn/components/tfm_tests
    GIT_TAG ${TFM_TEST_REPO_VERSION}
    GIT_PROGRESS TRUE
)

FetchContent_GetProperties(tfm_test_repo)
if(NOT tfm_test_repo_POPULATED)
    FetchContent_Populate(tfm_test_repo)
    set(TFM_TEST_REPO_PATH ${tfm_test_repo_SOURCE_DIR} CACHE PATH "Path to TFM-TEST repo (or DOWNLOAD to fetch automatically" FORCE)
endif()
endif()

if (ARMINO_SDK_PATH)
    set(ARMINO_MIDDLEWARE_REPO_PATH ${ARMINO_SDK_PATH}/middleware CACHE PATH "Path to Armino middleware repo (or DOWNLOAD to fetch automatically" FORCE)
    set(ARMINO_INCLUDE_REPO_PATH ${ARMINO_SDK_PATH}/include CACHE PATH "Path to Armino include repo (or DOWNLOAD to fetch automatically" FORCE)
    set(ARMINO_COMMON_REPO_PATH ${ARMINO_SDK_PATH}/components/bk_common CACHE PATH "Path to Armino common repo (or DOWNLOAD to fetch automatically" FORCE)
else()
    FetchContent_Declare(armino_middleware_repo
        GIT_REPOSITORY ssh://${USER}@192.168.0.46:29418/iot/wcn/middleware
        GIT_TAG "85a1e5f"
        GIT_PROGRESS TRUE
    )

    FetchContent_GetProperties(armino_middleware_repo)
    if(NOT armino_middleware_repo_POPULATED)
        FetchContent_Populate(armino_middleware_repo)
        set(ARMINO_MIDDLEWARE_REPO_PATH ${armino_middleware_repo_SOURCE_DIR} CACHE PATH "Path to Armino middleware repo (or DOWNLOAD to fetch automatically" FORCE)
    endif()

    FetchContent_Declare(armino_include_repo
        GIT_REPOSITORY ssh://${USER}@192.168.0.46:29418/iot/wcn/include
        GIT_TAG "c2aa488"
        GIT_PROGRESS TRUE
    )

    FetchContent_GetProperties(armino_include_repo)
    if(NOT armino_include_repo_POPULATED)
        FetchContent_Populate(armino_include_repo)
        set(ARMINO_INCLUDE_REPO_PATH ${armino_include_repo_SOURCE_DIR} CACHE PATH "Path to Armino include repo (or DOWNLOAD to fetch automatically" FORCE)
    endif()

    FetchContent_Declare(armino_common_repo
        GIT_REPOSITORY ssh://${USER}@192.168.0.46:29418/iot/wcn/components/bk_common
        GIT_TAG "8fb2e0d"
        GIT_PROGRESS TRUE
    )

    FetchContent_GetProperties(armino_common_repo)
    if(NOT armino_common_repo_POPULATED)
        FetchContent_Populate(armino_common_repo)
        set(ARMINO_COMMON_REPO_PATH ${armino_common_repo_SOURCE_DIR} CACHE PATH "Path to Armino common repo (or DOWNLOAD to fetch automatically" FORCE)
    endif()

endif()
