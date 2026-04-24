/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// UNSUPPORTED: c++03, c++11, c++14

// to_chars requires functions in the dylib that have not been introduced in older
// versions of the dylib on macOS.
// XFAIL: availability-fp_to_chars-missing

// steady_clock requires threads.
// UNSUPPORTED: no-threads
// UNSUPPORTED: no-random-device
// UNSUPPORTED: no-localization

// XFAIL: LIBCXX-AIX-FIXME

// <charconv>

#include <type_traits>

// Work-around for sprintf_s's usage in the Microsoft tests.
#ifndef _WIN32
#  define sprintf_s snprintf
#endif

#ifdef _MSVC_STL_VERSION
#include <xutility>
using std::_Bit_cast;
#else
// FUNCTION TEMPLATE _Bit_cast
template <class _To, class _From,
          std::enable_if_t<sizeof(_To) == sizeof(_From) && std::is_trivially_copyable_v<_To> &&
                               std::is_trivially_copyable_v<_From>,
                           int> = 0>
[[nodiscard]] constexpr _To _Bit_cast(const _From& _From_obj) noexcept {
  return __builtin_bit_cast(_To, _From_obj);
}
#endif

// Includes Microsoft's test that tests the entire header.

#include "test.cpp"
#include "libcxx_tc_common.h"
