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
// <optional>
// UNSUPPORTED: c++03, c++11, c++14

// template<class T>
//   optional(T) -> optional<T>;

#include <optional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

int tc_utilities_optional_optional_object_optional_object_ctor_deduct(void) {
//  Test the explicit deduction guides
    {
//  optional(T)
    std::optional opt(5);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<int>);
    TC_ASSERT_EXPR(static_cast<bool>(opt));
    TC_ASSERT_EXPR(*opt == 5);
    }

    {
//  optional(T)
    std::optional opt(A{});
    ASSERT_SAME_TYPE(decltype(opt), std::optional<A>);
    TC_ASSERT_EXPR(static_cast<bool>(opt));
    }

    {
//  optional(const T&);
    const int& source = 5;
    std::optional opt(source);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<int>);
    TC_ASSERT_EXPR(static_cast<bool>(opt));
    TC_ASSERT_EXPR(*opt == 5);
    }

    {
//  optional(T*);
    const int* source = nullptr;
    std::optional opt(source);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<const int*>);
    TC_ASSERT_EXPR(static_cast<bool>(opt));
    TC_ASSERT_EXPR(*opt == nullptr);
    }

    {
//  optional(T[]);
    int source[] = {1, 2, 3};
    std::optional opt(source);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<int*>);
    TC_ASSERT_EXPR(static_cast<bool>(opt));
    TC_ASSERT_EXPR((*opt)[0] == 1);
    }

//  Test the implicit deduction guides
    {
//  optional(optional);
    std::optional<char> source('A');
    std::optional opt(source);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<char>);
    TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(source));
    TC_ASSERT_EXPR(*opt == *source);
    }

  return 0;
}
