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
// <memory>

// unique_ptr

// Test unique_ptr::pointer type

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Deleter {
  struct pointer {};
};

struct D2 {
private:
  typedef void pointer;
};

struct D3 {
  static long pointer;
};

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  {
    typedef std::unique_ptr<VT> P;
    static_assert((std::is_same<typename P::pointer, int*>::value), "");
  }
  {
    typedef std::unique_ptr<VT, Deleter> P;
    static_assert((std::is_same<typename P::pointer, Deleter::pointer>::value),
                  "");
  }
#if TEST_STD_VER >= 11
  {
    typedef std::unique_ptr<VT, D2> P;
    static_assert(std::is_same<typename P::pointer, int*>::value, "");
  }
  {
    typedef std::unique_ptr<VT, D3> P;
    static_assert(std::is_same<typename P::pointer, int*>::value, "");
  }
#endif
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_pointer_type(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
