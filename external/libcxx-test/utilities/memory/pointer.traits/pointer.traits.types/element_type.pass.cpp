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
//     typedef <details> element_type;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    typedef char element_type;
};

template <class T>
struct B
{
    typedef char element_type;
};

template <class T>
struct C
{
};

template <class T, class U>
struct D
{
};

template <class T, class U>
struct E
{
    static int element_type;
};

template <class T>
struct F {
private:
  typedef int element_type;
};

int tc_utilities_memory_pointer_traits_pointer_traits_types_element_type(void) {
    static_assert((std::is_same<std::pointer_traits<A>::element_type, char>::value), "");
    static_assert((std::is_same<std::pointer_traits<B<int> >::element_type, char>::value), "");
    static_assert((std::is_same<std::pointer_traits<C<int> >::element_type, int>::value), "");
    static_assert((std::is_same<std::pointer_traits<D<double, int> >::element_type, double>::value), "");
    static_assert((std::is_same<std::pointer_traits<E<double, int> >::element_type, double>::value), "");
#if TEST_STD_VER >= 11
    static_assert((std::is_same<std::pointer_traits<F<double>>::element_type, double>::value), "");
#endif


  return 0;
}
