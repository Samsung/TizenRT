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
//     typedef typename Alloc::value_type value_type;
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
};

int tc_utilities_memory_allocator_traits_value_type(void) {
    static_assert((std::is_same<std::allocator_traits<A<char> >::value_type, char>::value), "");

  return 0;
}
