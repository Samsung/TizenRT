//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// bitset(string, pos, n, zero, one); // constexpr since C++23

#include <algorithm> // for 'min' and 'max'
#include <bitset>
#include <cassert>
#include <stdexcept> // for 'invalid_argument'
#include <string>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_string_ctor() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  if (!TEST_IS_CONSTANT_EVALUATED) {
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        std::string s("xxx1010101010xxxx");
        std::bitset<N> v(s, s.size()+1, 10);
        TC_ASSERT_EXPR(false);
    }
    catch (std::out_of_range&)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        std::string s("xxx1010101010xxxx");
        std::bitset<N> v(s, 2, 10);
        TC_ASSERT_EXPR(false);
    }
    catch (std::invalid_argument&)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        std::string s("xxxbababababaxxxx");
        std::bitset<N> v(s, 2, 10, 'a', 'b');
        TC_ASSERT_EXPR(false);
    }
    catch (std::invalid_argument&)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }
#endif // TEST_HAS_NO_EXCEPTIONS
  {
    std::string s("xxx1010101010xxxx");
    std::bitset<N> v(s, 3, 10);
    std::size_t M = std::min<std::size_t>(v.size(), 10);
    for (std::size_t i = 0; i < M; ++i)
        TC_ASSERT_EXPR(v[i] == (s[3 + M - 1 - i] == '1'));
    for (std::size_t i = 10; i < v.size(); ++i)
        TC_ASSERT_EXPR(v[i] == false);
  }
  {
    std::string s("xxxbababababaxxxx");
    std::bitset<N> v(s, 3, 10, 'a', 'b');
    std::size_t M = std::min<std::size_t>(v.size(), 10);
    for (std::size_t i = 0; i < M; ++i)
        TC_ASSERT_EXPR(v[i] == (s[3 + M - 1 - i] == 'b'));
    for (std::size_t i = 10; i < v.size(); ++i)
        TC_ASSERT_EXPR(v[i] == false);
  }
}

struct Nonsense {
    virtual ~Nonsense() {}
};

TEST_CONSTEXPR_CXX23 void test_for_non_eager_instantiation() {
    // Ensure we don't accidentally instantiate `std::basic_string<Nonsense>`
    // since it may not be well formed and can cause an error in the
    // non-immediate context.
    static_assert(!std::is_constructible<std::bitset<3>, Nonsense*>::value, "");
    static_assert(!std::is_constructible<std::bitset<3>, Nonsense*, std::size_t, Nonsense&, Nonsense&>::value, "");
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_string_ctor<0>();
  test_string_ctor<1>();
  test_string_ctor<31>();
  test_string_ctor<32>();
  test_string_ctor<33>();
  test_string_ctor<63>();
  test_string_ctor<64>();
  test_string_ctor<65>();
  test_string_ctor<1000>();
  test_for_non_eager_instantiation();

  return true;
}

int tc_utilities_template_bitset_bitset_cons_string_ctor(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
