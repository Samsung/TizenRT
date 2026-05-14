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
//     typedef Alloc::difference_type
//           | pointer_traits<pointer>::difference_type         difference_type;
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
    typedef short difference_type;
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
    struct pointer {};
    struct const_pointer {};
    struct void_pointer {};
    struct const_void_pointer {};
};


template <class T>
struct D
{
    typedef T value_type;
private:
    typedef void difference_type;
};

namespace std
{

template <>
struct pointer_traits<C<char>::pointer>
{
    typedef C<char>::pointer pointer;
    typedef char             element_type;
    typedef signed char      difference_type;
};

}

int tc_utilities_memory_allocator_traits_allocator_traits_types_difference_type(void) {
    static_assert((std::is_same<std::allocator_traits<A<char> >::difference_type, short>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::difference_type, std::ptrdiff_t>::value), "");
    static_assert((std::is_same<std::allocator_traits<C<char> >::difference_type, signed char>::value), "");
#if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<D<char> >::difference_type, std::ptrdiff_t>::value), "");
#endif

  return 0;
}
