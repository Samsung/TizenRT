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
//     template <class T> using rebind_alloc  = Alloc::rebind<U>::other | Alloc<T, Args...>;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct ReboundA {};

template <class T>
struct A
{
    typedef T value_type;

    template <class U> struct rebind {typedef ReboundA<U> other;};
};

template <class T, class U>
struct ReboundB {};

template <class T, class U>
struct B
{
    typedef T value_type;

    template <class V> struct rebind {typedef ReboundB<V, U> other;};
};

template <class T>
struct C
{
    typedef T value_type;
};

template <class T, class U>
struct D
{
    typedef T value_type;
};

template <class T>
struct E
{
    typedef T value_type;

    template <class U> struct rebind {typedef ReboundA<U> otter;};
};

template <class T>
struct F {
    typedef T value_type;
private:
    template <class>
    struct rebind { typedef void other; };
};

template <class T>
struct G {
    typedef T value_type;
    template <class>
    struct rebind {
    private:
        typedef void other;
    };
};

int tc_utilities_memory_allocator_traits_allocator_traits_types_rebind_alloc(void) {
#if TEST_STD_VER >= 11
    static_assert((std::is_same<std::allocator_traits<A<char> >::rebind_alloc<double>, ReboundA<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<B<int, char> >::rebind_alloc<double>, ReboundB<double, char> >::value), "");
    static_assert((std::is_same<std::allocator_traits<C<char> >::rebind_alloc<double>, C<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<D<int, char> >::rebind_alloc<double>, D<double, char> >::value), "");
    static_assert((std::is_same<std::allocator_traits<E<char> >::rebind_alloc<double>, E<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<F<char> >::rebind_alloc<double>, F<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<G<char> >::rebind_alloc<double>, G<double> >::value), "");
#else
    static_assert((std::is_same<std::allocator_traits<A<char> >::rebind_alloc<double>::other, ReboundA<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<B<int, char> >::rebind_alloc<double>::other, ReboundB<double, char> >::value), "");
    static_assert((std::is_same<std::allocator_traits<C<char> >::rebind_alloc<double>::other, C<double> >::value), "");
    static_assert((std::is_same<std::allocator_traits<D<int, char> >::rebind_alloc<double>::other, D<double, char> >::value), "");
    static_assert((std::is_same<std::allocator_traits<E<char> >::rebind_alloc<double>::other, E<double> >::value), "");
#endif

  return 0;
}
