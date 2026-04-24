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
// <functional>
//
// reference_wrapper
//
// template <class U>
//   reference_wrapper(U&&) noexcept(see below);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct convertible_to_int_ref {
  int val = 0;
  operator int&() { return val; }
  operator int const&() const { return val; }
};

template <bool IsNothrow>
struct nothrow_convertible {
  int val = 0;
  operator int&() TEST_NOEXCEPT_COND(IsNothrow) { return val; }
};

struct convertible_from_int {
  convertible_from_int(int) {}
};

void meow(std::reference_wrapper<int>) {}
void meow(convertible_from_int) {}

int tc_utilities_function_objects_refwrap_refwrap_const_type_conv_ctor(void) {
  {
    convertible_to_int_ref t;
    std::reference_wrapper<convertible_to_int_ref> r(t);
    TC_ASSERT_EXPR(&r.get() == &t);
  }
  {
    const convertible_to_int_ref t;
    std::reference_wrapper<const convertible_to_int_ref> r(t);
    TC_ASSERT_EXPR(&r.get() == &t);
  }
  {
    using Ref = std::reference_wrapper<int>;
    ASSERT_NOEXCEPT(Ref(nothrow_convertible<true>()));
    ASSERT_NOT_NOEXCEPT(Ref(nothrow_convertible<false>()));
  }
  {
    meow(0);
  }
  {
    extern std::reference_wrapper<int> purr();
    ASSERT_SAME_TYPE(decltype(true ? purr() : 0), int);
  }
#if TEST_STD_VER > 14
  {
    int i = 0;
    std::reference_wrapper ri(i);
    static_assert((std::is_same<decltype(ri), std::reference_wrapper<int>>::value), "" );
    const int j = 0;
    std::reference_wrapper rj(j);
    static_assert((std::is_same<decltype(rj), std::reference_wrapper<const int>>::value), "" );
  }
#endif

  return 0;
}
