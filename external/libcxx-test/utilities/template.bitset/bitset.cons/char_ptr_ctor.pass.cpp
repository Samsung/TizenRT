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
// template <class charT>
//     explicit bitset(const charT* str,
//                     typename basic_string<charT>::size_type n = basic_string<charT>::npos,
//                     charT zero = charT('0'), charT one = charT('1')); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <algorithm> // for 'min' and 'max'
#include <stdexcept> // for 'invalid_argument'

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_MSVC_DIAGNOSTIC_IGNORED(6294) // Ill-defined for-loop:  initial condition does not satisfy test.  Loop body not executed.

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_char_pointer_ctor()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
  if (!TEST_IS_CONSTANT_EVALUATED) {
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      std::bitset<N> v("xxx1010101010xxxx");
      TC_ASSERT_EXPR(false);
    }
    catch (std::invalid_argument&) {}
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

  {
    const char str[] = "1010101010";
    std::bitset<N> v(str);
    std::size_t M = std::min<std::size_t>(v.size(), 10);
    for (std::size_t i = 0; i < M; ++i)
        TC_ASSERT_EXPR(v[i] == (str[M - 1 - i] == '1'));
    for (std::size_t i = 10; i < v.size(); ++i)
        TC_ASSERT_EXPR(v[i] == false);
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_char_pointer_ctor<0>();
  test_char_pointer_ctor<1>();
  test_char_pointer_ctor<31>();
  test_char_pointer_ctor<32>();
  test_char_pointer_ctor<33>();
  test_char_pointer_ctor<63>();
  test_char_pointer_ctor<64>();
  test_char_pointer_ctor<65>();
  test_char_pointer_ctor<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_cons_char_ptr_ctor(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
