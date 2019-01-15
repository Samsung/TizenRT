# Copyright 2015-2016 Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.8.12)

# Config
SET(prefix "/usr")
SET(exec_prefix "${prefix}/bin")
SET(includedir "${prefix}/include/${PROJECT_NAME}")
SET(libdir "${prefix}/lib")
SET(libname "${PROJECT_NAME}")

#
# { TUV_CHANGES@20161129: It corresponds to uv.gyp's `sources` }
#
set(COMMON_SRCFILES
    ${INCLUDE_ROOT}/uv.h
# { TUV_CHANGES@20161129:
#   Makefile.am include tree.h only in WINNT
#   But tree.h is used in unix's several files (i.e. loop.c)
#   But we don't provide unix's the corresponding part.     }
#   ${INCLUDE_ROOT}/tree.h
    ${INCLUDE_ROOT}/uv-errno.h
    ${INCLUDE_ROOT}/uv-threadpool.h
    ${INCLUDE_ROOT}/uv-version.h
    ${SOURCE_ROOT}/fs-poll.c
    ${SOURCE_ROOT}/heap-inl.h
    ${SOURCE_ROOT}/inet.c
    ${SOURCE_ROOT}/queue.h
    ${SOURCE_ROOT}/threadpool.c
    ${SOURCE_ROOT}/uv-common.c
    ${SOURCE_ROOT}/uv-common.h
#   ${SOURCE_ROOT}/version.c
    ${SOURCE_ROOT}/tuv_debuglog.c
    )

set(LIB_TUV_SRCFILES
      ${COMMON_SRCFILES}
      ${PLATFORM_SRCFILES}
      )

set(LIB_TUV_INCDIRS
      ${INCLUDE_ROOT}
      ${SOURCE_ROOT}
      )

# pkg-config
CONFIGURE_FILE(config/${libname}.pc.in ${libname}.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libname}.pc"
  DESTINATION "${libdir}/pkgconfig/")

# build tuv library
set(TARGETLIBNAME tuv)
add_library(${TARGETLIBNAME} ${LIB_TUV_SRCFILES})
target_include_directories(${TARGETLIBNAME} SYSTEM PRIVATE ${TARGET_INC})
target_include_directories(${TARGETLIBNAME} PUBLIC ${LIB_TUV_INCDIRS})
set_target_properties(${TARGETLIBNAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT}"
    LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUT}")
install(TARGETS ${TARGETLIBNAME} ARCHIVE
  DESTINATION "${libdir}")

# build tuv shared library
if (DEFINED CREATE_SHARED_LIB AND CREATE_SHARED_LIB STREQUAL "yes")
  set(TARGETSHAREDLIBNAME tuv_shared)
  add_library(${TARGETSHAREDLIBNAME} SHARED ${LIB_TUV_SRCFILES})
  target_include_directories(${TARGETSHAREDLIBNAME} SYSTEM PRIVATE ${TARGET_INC})
  target_include_directories(${TARGETSHAREDLIBNAME} PUBLIC ${LIB_TUV_INCDIRS})
  set_target_properties(${TARGETSHAREDLIBNAME} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT}"
      COMPILE_FLAGS -fPIC
      OUTPUT_NAME tuv)
install(TARGETS ${TARGETSHAREDLIBNAME} LIBRARY
  DESTINATION "${libdir}")
endif()

if(DEFINED COPY_TARGET_LIB)
  add_custom_command(TARGET ${TARGETLIBNAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGETLIBNAME}>
                                  "${COPY_TARGET_LIB}"
      COMMENT "Copying lib${TARGETLIBNAME} to ${COPY_TARGET_LIB}")
endif()

# Install headers
file(GLOB LIBTUV_HEADERS include/*.h)
install(FILES ${LIBTUV_HEADERS} DESTINATION ${includedir})
