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

// template<class R, class ...Args>
// function(R(*)(Args...)) -> function<R(Args...)>;

// UNSUPPORTED: c++03, c++11, c++14

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct R { };
struct A1 { };
struct A2 { };
struct A3 { };

R f0() { return {}; }
R f1(A1) { return {}; }
R f2(A1, A2) { return {}; }
R f3(A1, A2, A3) { return {}; }
R f4(A1 = {}) { return {}; }

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_deduct_ptr(void) {
  {
    // implicit
    std::function a = f0;
    ASSERT_SAME_TYPE(decltype(a), std::function<R()>);

    std::function b = &f0;
    ASSERT_SAME_TYPE(decltype(b), std::function<R()>);

    // explicit
    std::function c{f0};
    ASSERT_SAME_TYPE(decltype(c), std::function<R()>);

    std::function d{&f0};
    ASSERT_SAME_TYPE(decltype(d), std::function<R()>);
  }
  {
    // implicit
    std::function a = f1;
    ASSERT_SAME_TYPE(decltype(a), std::function<R(A1)>);

    std::function b = &f1;
    ASSERT_SAME_TYPE(decltype(b), std::function<R(A1)>);

    // explicit
    std::function c{f1};
    ASSERT_SAME_TYPE(decltype(c), std::function<R(A1)>);

    std::function d{&f1};
    ASSERT_SAME_TYPE(decltype(d), std::function<R(A1)>);
  }
  {
    // implicit
    std::function a = f2;
    ASSERT_SAME_TYPE(decltype(a), std::function<R(A1, A2)>);

    std::function b = &f2;
    ASSERT_SAME_TYPE(decltype(b), std::function<R(A1, A2)>);

    // explicit
    std::function c{f2};
    ASSERT_SAME_TYPE(decltype(c), std::function<R(A1, A2)>);

    std::function d{&f2};
    ASSERT_SAME_TYPE(decltype(d), std::function<R(A1, A2)>);
  }
  {
    // implicit
    std::function a = f3;
    ASSERT_SAME_TYPE(decltype(a), std::function<R(A1, A2, A3)>);

    std::function b = &f3;
    ASSERT_SAME_TYPE(decltype(b), std::function<R(A1, A2, A3)>);

    // explicit
    std::function c{f3};
    ASSERT_SAME_TYPE(decltype(c), std::function<R(A1, A2, A3)>);

    std::function d{&f3};
    ASSERT_SAME_TYPE(decltype(d), std::function<R(A1, A2, A3)>);
  }
  // Make sure defaulted arguments don't mess up the deduction
  {
    // implicit
    std::function a = f4;
    ASSERT_SAME_TYPE(decltype(a), std::function<R(A1)>);

    std::function b = &f4;
    ASSERT_SAME_TYPE(decltype(b), std::function<R(A1)>);

    // explicit
    std::function c{f4};
    ASSERT_SAME_TYPE(decltype(c), std::function<R(A1)>);

    std::function d{&f4};
    ASSERT_SAME_TYPE(decltype(d), std::function<R(A1)>);
  }

  return 0;
}
