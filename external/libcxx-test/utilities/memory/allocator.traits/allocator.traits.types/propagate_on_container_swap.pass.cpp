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

// template <class Alloc>
// struct allocator_traits
// {
//     typedef Alloc::propagate_on_container_swap
//           | false_type                   propagate_on_container_swap;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;
    typedef std::true_type propagate_on_container_swap;
};

template <class T>
struct B
{
    typedef T value_type;
};

template <class T>
struct C
{
    typedef T value_type;
private:
    typedef std::true_type propagate_on_container_swap;
};

int tc_utilities_memory_allocator_traits_allocator_traits_types_propagate_on_container_swap(void) {
    static_assert((std::is_same<std::allocator_traits<A<char> >::propagate_on_container_swap, std::true_type>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::propagate_on_container_swap, std::false_type>::value), "");
 #if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<C<char> >::propagate_on_container_swap, std::false_type>::value), "");
#endif

  return 0;
}
