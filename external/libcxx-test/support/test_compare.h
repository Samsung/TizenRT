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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef TEST_COMPARE_H
#define TEST_COMPARE_H

#include <cstddef>
#include <type_traits>
#include <cstdlib>
#include <new>
#include <climits>

template <class C>
class test_compare
    : private C
{
    int data_;
public:
    explicit test_compare(int data = 0) : data_(data) {}

    typename C::result_type
    operator()(typename std::add_lvalue_reference<const typename C::first_argument_type>::type x,
               typename std::add_lvalue_reference<const typename C::second_argument_type>::type y) const
        {return C::operator()(x, y);}

    bool operator==(const test_compare& c) const
        {return data_ == c.data_;}
};


template <class C>
class non_const_compare
{
// operator() deliberately not marked as 'const'
    bool operator()(const C& x, const C& y) { return x < y; }
};


#endif  // TEST_COMPARE_H
