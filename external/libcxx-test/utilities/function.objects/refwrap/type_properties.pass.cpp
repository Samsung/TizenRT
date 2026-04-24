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
// <functional>

// reference_wrapper

// Test that reference wrapper meets the requirements of CopyConstructible and
// CopyAssignable, and TriviallyCopyable (starting in C++14).

#include <functional>
#include <type_traits>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 11
class MoveOnly
{
    MoveOnly(const MoveOnly&);
    MoveOnly& operator=(const MoveOnly&);

    int data_;
public:
    MoveOnly(int data = 1) : data_(data) {}
    MoveOnly(MoveOnly&& x)
        : data_(x.data_) {x.data_ = 0;}
    MoveOnly& operator=(MoveOnly&& x)
        {data_ = x.data_; x.data_ = 0; return *this;}

    int get() const {return data_;}
};
#endif


template <class T>
void test()
{
    typedef std::reference_wrapper<T> Wrap;
    static_assert(std::is_copy_constructible<Wrap>::value, "");
    static_assert(std::is_copy_assignable<Wrap>::value, "");
#if TEST_STD_VER >= 14
    static_assert(std::is_trivially_copyable<Wrap>::value, "");
#endif
}

int tc_utilities_function_objects_refwrap_type_properties(void) {
    test<int>();
    test<double>();
    test<std::string>();
#if TEST_STD_VER >= 11
    test<MoveOnly>();
#endif

  return 0;
}
