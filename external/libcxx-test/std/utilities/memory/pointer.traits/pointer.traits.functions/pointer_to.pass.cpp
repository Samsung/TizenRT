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

// template <class Ptr>
// struct pointer_traits
// {
//     static pointer pointer_to(<details>);
//     ...
// };

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
private:
    struct nat {};
public:
    typedef T element_type;
    element_type* t_;

    A(element_type* t) : t_(t) {}

    static A pointer_to(typename std::conditional<std::is_void<element_type>::value,
                                           nat, element_type>::type& et)
        {return A(&et);}
};

int tc_utilities_memory_pointer_traits_pointer_traits_functions_pointer_to(void) {
    {
        int i = 0;
        static_assert((std::is_same<A<int>, decltype(std::pointer_traits<A<int> >::pointer_to(i))>::value), "");
        A<int> a = std::pointer_traits<A<int> >::pointer_to(i);
        TC_ASSERT_EXPR(a.t_ == &i);
    }
    {
        (std::pointer_traits<A<void> >::element_type)0;
    }

  return 0;
}
