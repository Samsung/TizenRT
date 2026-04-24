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

// class function<R(ArgTypes...)>

// const std::type_info& target_type() const;

// UNSUPPORTED: no-rtti

#include <functional>
#include <typeinfo>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    int data_[10];
public:
    static int count;

    A()
    {
        ++count;
        for (int i = 0; i < 10; ++i)
            data_[i] = i;
    }

    A(const A&) {++count;}

    ~A() {--count;}

    int operator()(int i) const
    {
        for (int j = 0; j < 10; ++j)
            i += data_[j];
        return i;
    }

    int foo(int) const {return 1;}
};

int A::count = 0;

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_targ_target_type(void) {
    {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(f.target_type() == typeid(A));
    }
    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(f.target_type() == typeid(void));
    }

  return 0;
}
