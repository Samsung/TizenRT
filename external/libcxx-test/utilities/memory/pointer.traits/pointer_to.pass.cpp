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

// template <class T>
// struct pointer_traits<T*>
// {
//     static pointer pointer_to(<details>); // constexpr in C++20
//     ...
// };

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        int i = 0;
        static_assert(std::is_same<decltype(std::pointer_traits<int*>::pointer_to(i)), int*>::value, "");
        TC_ASSERT_EXPR(std::pointer_traits<int*>::pointer_to(i) == &i);
    }
    {
        int i = 0;
        static_assert(std::is_same<decltype(std::pointer_traits<const int*>::pointer_to(i)), const int*>::value, "");
        TC_ASSERT_EXPR(std::pointer_traits<const int*>::pointer_to(i) == &i);
    }
    return true;
}

int tc_utilities_memory_pointer_traits_pointer_to(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    {
        // Check that pointer_traits<void*> is still well-formed, even though it has no pointer_to.
        static_assert(std::is_same<std::pointer_traits<void*>::element_type, void>::value, "");
        static_assert(std::is_same<std::pointer_traits<const void*>::element_type, const void>::value, "");
        static_assert(std::is_same<std::pointer_traits<volatile void*>::element_type, volatile void>::value, "");
    }

    return 0;
}
