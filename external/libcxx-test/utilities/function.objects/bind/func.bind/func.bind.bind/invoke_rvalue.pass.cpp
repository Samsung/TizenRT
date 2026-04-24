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

// template<CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20

#include <stdio.h>

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int count = 0;

// 1 arg, return void

void f_void_1(int i)
{
    count += i;
}

struct A_void_1
{
    void operator()(int i)
    {
        count += i;
    }

    void mem1() {++count;}
    void mem2() const {count += 2;}
};

void
test_void_1()
{
    using namespace std::placeholders;
    int save_count = count;
    // function
    {
    std::bind(f_void_1, _1)(2);
    TC_ASSERT_EXPR(count == save_count + 2);
    save_count = count;
    }
    {
    std::bind(f_void_1, 2)();
    TC_ASSERT_EXPR(count == save_count + 2);
    save_count = count;
    }
    // function pointer
    {
    void (*fp)(int) = f_void_1;
    std::bind(fp, _1)(3);
    TC_ASSERT_EXPR(count == save_count+3);
    save_count = count;
    }
    {
    void (*fp)(int) = f_void_1;
    std::bind(fp, 3)();
    TC_ASSERT_EXPR(count == save_count+3);
    save_count = count;
    }
    // functor
    {
    A_void_1 a0;
    std::bind(a0, _1)(4);
    TC_ASSERT_EXPR(count == save_count+4);
    save_count = count;
    }
    {
    A_void_1 a0;
    std::bind(a0, 4)();
    TC_ASSERT_EXPR(count == save_count+4);
    save_count = count;
    }
    // member function pointer
    {
    void (A_void_1::*fp)() = &A_void_1::mem1;
    std::bind(fp, _1)(A_void_1());
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    A_void_1 a;
    std::bind(fp, _1)(&a);
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    }
    {
    void (A_void_1::*fp)() = &A_void_1::mem1;
    std::bind(fp, A_void_1())();
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    A_void_1 a;
    std::bind(fp, &a)();
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    }
    // const member function pointer
    {
    void (A_void_1::*fp)() const = &A_void_1::mem2;
    std::bind(fp, _1)(A_void_1());
    TC_ASSERT_EXPR(count == save_count+2);
    save_count = count;
    A_void_1 a;
    std::bind(fp, _1)(&a);
    TC_ASSERT_EXPR(count == save_count+2);
    save_count = count;
    }
    {
    void (A_void_1::*fp)() const = &A_void_1::mem2;
    std::bind(fp, A_void_1())();
    TC_ASSERT_EXPR(count == save_count+2);
    save_count = count;
    A_void_1 a;
    std::bind(fp, &a)();
    TC_ASSERT_EXPR(count == save_count+2);
    save_count = count;
    }
}

// 1 arg, return int

TEST_CONSTEXPR_CXX20 int f_int_1(int i) {
    return i + 1;
}

struct A_int_1 {
    TEST_CONSTEXPR_CXX20 A_int_1() : data_(5) {}
    TEST_CONSTEXPR_CXX20 int operator()(int i) {
        return i - 1;
    }

    TEST_CONSTEXPR_CXX20 int mem1() { return 3; }
    TEST_CONSTEXPR_CXX20 int mem2() const { return 4; }
    int data_;
};

TEST_CONSTEXPR_CXX20 bool test_int_1() {
    using namespace std::placeholders;
    // function
    {
    TC_ASSERT_EXPR(std::bind(f_int_1, _1)(2) == 3);
    TC_ASSERT_EXPR(std::bind(f_int_1, 2)() == 3);
    }
    // function pointer
    {
    int (*fp)(int) = f_int_1;
    TC_ASSERT_EXPR(std::bind(fp, _1)(3) == 4);
    TC_ASSERT_EXPR(std::bind(fp, 3)() == 4);
    }
    // functor
    {
    TC_ASSERT_EXPR(std::bind(A_int_1(), _1)(4) == 3);
    TC_ASSERT_EXPR(std::bind(A_int_1(), 4)() == 3);
    }
    // member function pointer
    {
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem1, _1)(A_int_1()) == 3);
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem1, A_int_1())() == 3);
    A_int_1 a;
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem1, _1)(&a) == 3);
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem1, &a)() == 3);
    }
    // const member function pointer
    {
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem2, _1)(A_int_1()) == 4);
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem2, A_int_1())() == 4);
    A_int_1 a;
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem2, _1)(&a) == 4);
    TC_ASSERT_EXPR(std::bind(&A_int_1::mem2, &a)() == 4);
    }
    // member data pointer
    {
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, _1)(A_int_1()) == 5);
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, A_int_1())() == 5);
    A_int_1 a;
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, _1)(a) == 5);
    std::bind(&A_int_1::data_, _1)(a) = 6;
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, _1)(a) == 6);
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, _1)(&a) == 6);
    std::bind(&A_int_1::data_, _1)(&a) = 7;
    TC_ASSERT_EXPR(std::bind(&A_int_1::data_, _1)(&a) == 7);
    }
    return true;
}

// 2 arg, return void

void f_void_2(int i, int j)
{
    count += i+j;
}

struct A_void_2
{
    void operator()(int i, int j)
    {
        count += i+j;
    }

    void mem1(int i) {count += i;}
    void mem2(int i) const {count += i;}
};

void
test_void_2()
{
    using namespace std::placeholders;
    int save_count = count;
    // function
    {
    std::bind(f_void_2, _1, _2)(2, 3);
    TC_ASSERT_EXPR(count == save_count+5);
    save_count = count;
    std::bind(f_void_2, 2, _1)(3);
    TC_ASSERT_EXPR(count == save_count+5);
    save_count = count;
    std::bind(f_void_2, 2, 3)();
    TC_ASSERT_EXPR(count == save_count+5);
    save_count = count;
    }
    // member function pointer
    {
    std::bind(&A_void_2::mem1, _1, _2)(A_void_2(), 3);
    TC_ASSERT_EXPR(count == save_count+3);
    save_count = count;
    std::bind(&A_void_2::mem1, _2, _1)(3, A_void_2());
    TC_ASSERT_EXPR(count == save_count+3);
    save_count = count;
    }
}

TEST_CONSTEXPR_CXX20 int f_nested(int i) {
    return i+1;
}

TEST_CONSTEXPR_CXX20 int g_nested(int i) {
    return i*10;
}

TEST_CONSTEXPR_CXX20 bool test_nested() {
    using namespace std::placeholders;
    TC_ASSERT_EXPR(std::bind(f_nested, std::bind(g_nested, _1))(3) == 31);
    return true;
}

TEST_CONSTEXPR_CXX20 bool test_many_args() {
    using namespace std::placeholders;
    auto f = [](int a, char, float, long) { return a; };
    auto bound = std::bind(f, _4, _3, _2, _1);
    TC_ASSERT_EXPR(bound(0l, 1.0f, '2', 3) == 3);
    return true;
}

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_invoke_rvalue(void) {
    test_void_1();
    test_int_1();
    test_void_2();
    test_nested();

    // The other tests are not constexpr-friendly since they need to use a global variable
#if TEST_STD_VER >= 20
    static_assert(test_int_1());
    static_assert(test_nested());
#endif

    return 0;
}
