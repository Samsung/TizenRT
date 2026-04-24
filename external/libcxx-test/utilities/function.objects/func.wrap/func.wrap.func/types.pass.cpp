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
// UNSUPPORTED: c++03

// <functional>

// template<Returnable R, CopyConstructible... ArgTypes>
// class function<R(ArgTypes...)> {
// public:
//   typedef R result_type;
//   typedef T1 argument_type;          // iff sizeof...(ArgTypes) == 1 and
//                                      // the type in ArgTypes is T1
//   typedef T1 first_argument_type;    // iff sizeof...(ArgTypes) == 2 and
//                                      // ArgTypes contains T1 and T2
//   typedef T2 second_argument_type;   // iff sizeof...(ArgTypes) == 2 and
//                                      // ArgTypes contains T1 and T2
//  ...
//  };

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"


template <typename T>
class has_argument_type
{
    typedef char yes;
    typedef long no;

    template <typename C> static yes check( typename C::argument_type * );
    template <typename C> static no  check(...);
public:
    enum { value = sizeof(check<T>(0)) == sizeof(yes) };
};

template <typename T>
class has_first_argument_type
{
    typedef char yes;
    typedef long no;

    template <typename C> static yes check( typename C::first_argument_type * );
    template <typename C> static no  check(...);
public:
    enum { value = sizeof(check<T>(0)) == sizeof(yes) };
};


template <typename T>
class has_second_argument_type
{
    typedef char yes;
    typedef long no;

    template <typename C> static yes check( typename C::second_argument_type *);
    template <typename C> static no  check(...);
public:
    enum { value = sizeof(check<T>(0)) == sizeof(yes) };
};

template <class F, class return_type>
void test_nullary_function ()
{
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename F::result_type, return_type>::value), "" );
#endif
    static_assert((!has_argument_type<F>::value), "" );
    static_assert((!has_first_argument_type<F>::value), "" );
    static_assert((!has_second_argument_type<F>::value), "" );
}

template <class F, class return_type, class arg_type>
void test_unary_function ()
{
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename F::result_type, return_type>::value), "" );
    static_assert((std::is_same<typename F::argument_type,  arg_type>::value), "" );
#endif
    static_assert((!has_first_argument_type<F>::value), "" );
    static_assert((!has_second_argument_type<F>::value), "" );
}

template <class F, class return_type, class arg_type1, class arg_type2>
void test_binary_function ()
{
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename F::result_type,        return_type>::value), "" );
    static_assert((std::is_same<typename F::first_argument_type,  arg_type1>::value), "" );
    static_assert((std::is_same<typename F::second_argument_type, arg_type2>::value), "" );
#endif
    static_assert((!has_argument_type<F>::value), "" );
}

template <class F, class return_type>
void test_other_function ()
{
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename F::result_type, return_type>::value), "" );
#endif
    static_assert((!has_argument_type<F>::value), "" );
    static_assert((!has_first_argument_type<F>::value), "" );
    static_assert((!has_second_argument_type<F>::value), "" );
}

int tc_utilities_function_objects_func_wrap_func_wrap_func_types(void) {
    test_nullary_function<std::function<int()>, int>();
    test_unary_function  <std::function<double(int)>, double, int>();
    test_binary_function <std::function<double(int, char)>, double, int, char>();
    test_other_function  <std::function<double(int, char, double)>, double>();

  return 0;
}
