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
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_ENABLE_CXX20_REMOVED_RAW_STORAGE_ITERATOR
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// raw_storage_iterator

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include <MoveOnly.h>
#include "libcxx_tc_common.h"

int A_constructed = 0;

struct A
{
    int data_;
public:
    explicit A(int i) : data_(i) {++A_constructed;}

    A(const A& a) : data_(a.data_)  {++A_constructed;}
    ~A() {--A_constructed; data_ = 0;}

    bool operator==(int i) const {return data_ == i;}
    A* operator& () = delete;
};

int tc_utilities_memory_storage_iterator_raw_storage_iterator(void) {
    {
    typedef A S;
    typedef std::aligned_storage<3*sizeof(S), std::alignment_of<S>::value>::type
            Storage;
    Storage buffer;
    std::raw_storage_iterator<S*, S> it((S*)&buffer);
    TC_ASSERT_EXPR(A_constructed == 0);
    for (int i = 0; i < 3; ++i)
    {
        *it++ = S(i+1);
        S* ap = (S*)&buffer + i;
        TC_ASSERT_EXPR(*ap == i+1);
        TC_ASSERT_EXPR(A_constructed == i+1);
    }
    }
#if TEST_STD_VER >= 14
    {
    typedef MoveOnly S;
    typedef std::aligned_storage<3*sizeof(S), std::alignment_of<S>::value>::type
            Storage;
    Storage buffer;
    std::raw_storage_iterator<S*, S> it((S*)&buffer);
    S m{1};
    *it++ = std::move(m);
    TC_ASSERT_EXPR(m.get() == 0); // moved from
    S *ap = (S*) &buffer;
    TC_ASSERT_EXPR(ap->get() == 1); // original value
    }
#endif

  return 0;
}
