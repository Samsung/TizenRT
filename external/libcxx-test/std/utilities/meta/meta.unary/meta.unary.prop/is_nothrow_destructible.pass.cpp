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

// is_nothrow_destructible

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_nothrow_destructible()
{
    static_assert( std::is_nothrow_destructible<T>::value, "");
    static_assert( std::is_nothrow_destructible<const T>::value, "");
    static_assert( std::is_nothrow_destructible<volatile T>::value, "");
    static_assert( std::is_nothrow_destructible<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_nothrow_destructible_v<T>, "");
    static_assert( std::is_nothrow_destructible_v<const T>, "");
    static_assert( std::is_nothrow_destructible_v<volatile T>, "");
    static_assert( std::is_nothrow_destructible_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_nothrow_destructible()
{
    static_assert(!std::is_nothrow_destructible<T>::value, "");
    static_assert(!std::is_nothrow_destructible<const T>::value, "");
    static_assert(!std::is_nothrow_destructible<volatile T>::value, "");
    static_assert(!std::is_nothrow_destructible<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_nothrow_destructible_v<T>, "");
    static_assert(!std::is_nothrow_destructible_v<const T>, "");
    static_assert(!std::is_nothrow_destructible_v<volatile T>, "");
    static_assert(!std::is_nothrow_destructible_v<const volatile T>, "");
#endif
}


struct PublicDestructor           { public:     ~PublicDestructor() {}};
struct ProtectedDestructor        { protected:  ~ProtectedDestructor() {}};
struct PrivateDestructor          { private:    ~PrivateDestructor() {}};

struct VirtualPublicDestructor           { public:    virtual ~VirtualPublicDestructor() {}};
struct VirtualProtectedDestructor        { protected: virtual ~VirtualProtectedDestructor() {}};
struct VirtualPrivateDestructor          { private:   virtual ~VirtualPrivateDestructor() {}};

struct PurePublicDestructor              { public:    virtual ~PurePublicDestructor() = 0; };
struct PureProtectedDestructor           { protected: virtual ~PureProtectedDestructor() = 0; };
struct PurePrivateDestructor             { private:   virtual ~PurePrivateDestructor() = 0; };

class Empty
{
};


union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
    virtual void foo() = 0;
};


int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_destructible(void) {
    test_is_not_nothrow_destructible<void>();
    test_is_not_nothrow_destructible<char[]>();
    test_is_not_nothrow_destructible<char[][3]>();

    test_is_nothrow_destructible<int&>();
    test_is_nothrow_destructible<int>();
    test_is_nothrow_destructible<double>();
    test_is_nothrow_destructible<int*>();
    test_is_nothrow_destructible<const int*>();
    test_is_nothrow_destructible<char[3]>();

#if TEST_STD_VER >= 11
    // requires noexcept. These are all destructible.
    test_is_nothrow_destructible<PublicDestructor>();
    test_is_nothrow_destructible<VirtualPublicDestructor>();
    test_is_nothrow_destructible<PurePublicDestructor>();
    test_is_nothrow_destructible<bit_zero>();
    test_is_nothrow_destructible<Abstract>();
    test_is_nothrow_destructible<Empty>();
    test_is_nothrow_destructible<Union>();
#endif
    // requires access control
    test_is_not_nothrow_destructible<ProtectedDestructor>();
    test_is_not_nothrow_destructible<PrivateDestructor>();
    test_is_not_nothrow_destructible<VirtualProtectedDestructor>();
    test_is_not_nothrow_destructible<VirtualPrivateDestructor>();
    test_is_not_nothrow_destructible<PureProtectedDestructor>();
    test_is_not_nothrow_destructible<PurePrivateDestructor>();


  return 0;
}
