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

// shared_ptr

// template<class T, class... Args>
// shared_ptr<T> make_shared(Args&&... args); // T is not an array

#include <memory>
#include <cassert>

#include "count_new.h"
#include "operator_hijacker.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A(int i, char c) : int_(i), char_(c) {++count;}
    A(const A& a)
        : int_(a.int_), char_(a.char_)
        {++count;}
    ~A() {--count;}

    int get_int() const {return int_;}
    char get_char() const {return char_;}

    A* operator& () = delete;

private:
    int int_;
    char char_;
};

int A::count = 0;


struct Foo
{
    Foo() = default;
    virtual ~Foo() = default;
};

#ifdef _LIBCPP_VERSION
struct Result {};
static Result theFunction() { return Result(); }
static int resultDeletorCount;
static void resultDeletor(Result (*pf)()) {
  TC_ASSERT_EXPR(pf == theFunction);
  ++resultDeletorCount;
}

void test_pointer_to_function() {
    { // https://llvm.org/PR27566
      std::shared_ptr<Result()> x(&theFunction, &resultDeletor);
      std::shared_ptr<Result()> y(theFunction, resultDeletor);
    }
    TC_ASSERT_EXPR(resultDeletorCount == 2);
}
#else // _LIBCPP_VERSION
void test_pointer_to_function() {}
#endif // _LIBCPP_VERSION

template <typename T>
void test(const T &t0)
{
    {
      T t1 = t0;
      std::shared_ptr<T> p0 = std::make_shared<T>(t0);
      std::shared_ptr<T> p1 = std::make_shared<T>(t1);
      TC_ASSERT_EXPR(*p0 == t0);
      TC_ASSERT_EXPR(*p1 == t1);
    }

    {
      const T t1 = t0;
      std::shared_ptr<const T> p0 = std::make_shared<const T>(t0);
      std::shared_ptr<const T> p1 = std::make_shared<const T>(t1);
      TC_ASSERT_EXPR(*p0 == t0);
      TC_ASSERT_EXPR(*p1 == t1);
    }
}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_make_shared(void) {
    int nc = globalMemCounter.outstanding_new;
    {
    int i = 67;
    char c = 'e';
    std::shared_ptr<A> p = std::make_shared<A>(i, c);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(nc+1));
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(p->get_int() == 67);
    TC_ASSERT_EXPR(p->get_char() == 'e');
    }

    { // https://llvm.org/PR24137
    std::shared_ptr<Foo> p1       = std::make_shared<Foo>();
    TC_ASSERT_EXPR(p1.get());
    std::shared_ptr<const Foo> p2 = std::make_shared<const Foo>();
    TC_ASSERT_EXPR(p2.get());
    }

    test_pointer_to_function();

#if TEST_STD_VER >= 11
    nc = globalMemCounter.outstanding_new;
    {
    char c = 'e';
    std::shared_ptr<A> p = std::make_shared<A>(67, c);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(nc+1));
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(p->get_int() == 67);
    TC_ASSERT_EXPR(p->get_char() == 'e');
    }
#endif
    TC_ASSERT_EXPR(A::count == 0);

    // Make sure std::make_shared handles badly-behaved types properly
    {
      std::shared_ptr<operator_hijacker> p1 = std::make_shared<operator_hijacker>();
      std::shared_ptr<operator_hijacker> p2 = std::make_shared<operator_hijacker>(operator_hijacker());
      TC_ASSERT_EXPR(p1 != nullptr);
      TC_ASSERT_EXPR(p2 != nullptr);
    }

    test<bool>(true);
    test<int>(3);
    test<double>(5.0);

  return 0;
}
