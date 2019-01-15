# Copyright 2018 Samsung Electronics Co., Ltd.
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

# linux common
include("cmake/option/option_unix_common.cmake")
include("cmake/option/option_linux_common.cmake")

# gbs-tizen specific
if(DEFINED TARGET_BOARD)
  if(${TARGET_BOARD} STREQUAL "rpi3")
    set(FLAGS_COMMON
          ${FLAGS_COMMON}
          "-mfloat-abi=softfp"
          "-mfpu=neon-vfpv4"
          )
  elseif(${TARGET_BOARD} STREQUAL "None")
  else()
    message(FATAL_ERROR "TARGET_BOARD=`${TARGET_BOARD}` is unknown to make")
  endif()
else()
  message(FATAL_ERROR "TARGET_BOARD is undefined")
endif()
