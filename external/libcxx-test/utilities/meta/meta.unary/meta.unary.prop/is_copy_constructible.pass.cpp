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
// type_traits

// is_copy_constructible

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_copy_constructible()
{
    static_assert( std::is_copy_constructible<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_copy_constructible_v<T>, "");
#endif
}

template <class T>
void test_is_not_copy_constructible()
{
    static_assert(!std::is_copy_constructible<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_copy_constructible_v<T>, "");
#endif
}

class Empty
{
};

class NotEmpty
{
public:
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
public:
    virtual ~Abstract() = 0;
};

struct A
{
    A(const A&);
};

class B
{
    B(const B&);
};

struct C
{
    C(C&);  // not const
    void operator=(C&);  // not const
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_copy_constructible(void) {
    test_is_copy_constructible<A>();
    test_is_copy_constructible<int&>();
    test_is_copy_constructible<Union>();
    test_is_copy_constructible<Empty>();
    test_is_copy_constructible<int>();
    test_is_copy_constructible<double>();
    test_is_copy_constructible<int*>();
    test_is_copy_constructible<const int*>();
    test_is_copy_constructible<NotEmpty>();
    test_is_copy_constructible<bit_zero>();

    test_is_not_copy_constructible<char[3]>();
    test_is_not_copy_constructible<char[]>();
    test_is_not_copy_constructible<void>();
    test_is_not_copy_constructible<Abstract>();
    test_is_not_copy_constructible<C>();
#if TEST_STD_VER >= 11
    test_is_not_copy_constructible<B>();
#endif

  return 0;
}
