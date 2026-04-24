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
// UNSUPPORTED: c++03

// <functional>

// template<class T> struct is_bind_expression

#include <functional>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <bool Expected, class T>
void
test(const T&)
{
    static_assert(std::is_bind_expression<T>::value == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression<T&>::value == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression<const T>::value == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression<const T&>::value == Expected, "");
    static_assert(std::is_base_of<std::integral_constant<bool, Expected>, std::is_bind_expression<T> >::value, "");

#if TEST_STD_VER > 14
    ASSERT_SAME_TYPE(decltype(std::is_bind_expression_v<T>), const bool);
    static_assert(std::is_bind_expression_v<T> == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression_v<T&> == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression_v<const T> == Expected, "");
    LIBCPP_STATIC_ASSERT(std::is_bind_expression_v<const T&> == Expected, "");
#endif
}

struct C {int operator()(...) const { return 0; }};

int tc_utilities_function_objects_bind_func_bind_func_bind_isbind_is_bind_expression(void) {
    test<true>(std::bind(C()));
    test<true>(std::bind(C(), std::placeholders::_2));
    test<true>(std::bind<int>(C()));
    test<false>(1);
    test<false>(std::placeholders::_2);

  return 0;
}
