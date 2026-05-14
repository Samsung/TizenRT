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
//     typedef Alloc::size_type | size_t    size_type;
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
    typedef unsigned short size_type;
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
struct D {
    typedef T value_type;
    typedef short difference_type;
private:
    typedef void size_type;
};

namespace std
{

template <>
struct pointer_traits<C<char>::pointer>
{
    typedef signed char difference_type;
};

}

int tc_utilities_memory_allocator_traits_allocator_traits_types_size_type(void) {
    static_assert((std::is_same<std::allocator_traits<A<char> >::size_type, unsigned short>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::size_type,
                   std::make_unsigned<std::ptrdiff_t>::type>::value), "");
    static_assert((std::is_same<std::allocator_traits<C<char> >::size_type,
                   unsigned char>::value), "");
#if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<D<char> >::size_type, unsigned short>::value), "");
#endif

  return 0;
}
