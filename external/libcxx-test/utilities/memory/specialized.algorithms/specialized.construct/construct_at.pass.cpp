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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <memory>

// template <class T, class ...Args>
// constexpr T* construct_at(T* location, Args&& ...args);

#include <memory>
#include <cassert>
#include <utility>

#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct Foo {
    constexpr Foo() { }
    constexpr Foo(int a, char b, double c) : a_(a), b_(b), c_(c) { }
    constexpr Foo(int a, char b, double c, int* count) : Foo(a, b, c) { *count += 1; }
    constexpr bool operator==(Foo const& other) const {
        return a_ == other.a_ && b_ == other.b_ && c_ == other.c_;
    }

private:
    int a_;
    char b_;
    double c_;
};

struct Counted {
    int& count_;
    constexpr Counted(int& count) : count_(count) { ++count; }
    constexpr Counted(Counted const& that) : count_(that.count_) { ++count_; }
    constexpr ~Counted() { --count_; }
};

constexpr bool test()
{
    {
        int i = 99;
        int* res = std::construct_at(&i);
        TC_ASSERT_EXPR(res == &i);
        TC_ASSERT_EXPR(*res == 0);
    }

    {
        int i = 0;
        int* res = std::construct_at(&i, 42);
        TC_ASSERT_EXPR(res == &i);
        TC_ASSERT_EXPR(*res == 42);
    }

    {
        Foo foo = {};
        int count = 0;
        Foo* res = std::construct_at(&foo, 42, 'x', 123.89, &count);
        TC_ASSERT_EXPR(res == &foo);
        TC_ASSERT_EXPR(*res == Foo(42, 'x', 123.89));
        TC_ASSERT_EXPR(count == 1);
    }

    {
        std::allocator<Counted> a;
        Counted* p = a.allocate(2);
        int count = 0;
        std::construct_at(p, count);
        TC_ASSERT_EXPR(count == 1);
        std::construct_at(p+1, count);
        TC_ASSERT_EXPR(count == 2);
        (p+1)->~Counted();
        TC_ASSERT_EXPR(count == 1);
        p->~Counted();
        TC_ASSERT_EXPR(count == 0);
        a.deallocate(p, 2);
    }

    {
        std::allocator<Counted const> a;
        Counted const* p = a.allocate(2);
        int count = 0;
        std::construct_at(p, count);
        TC_ASSERT_EXPR(count == 1);
        std::construct_at(p+1, count);
        TC_ASSERT_EXPR(count == 2);
        (p+1)->~Counted();
        TC_ASSERT_EXPR(count == 1);
        p->~Counted();
        TC_ASSERT_EXPR(count == 0);
        a.deallocate(p, 2);
    }

    return true;
}

template <class ...Args, class = decltype(std::construct_at(std::declval<Args>()...))>
constexpr bool can_construct_at(Args&&...) { return true; }

template <class ...Args>
constexpr bool can_construct_at(...) { return false; }

// Check that SFINAE works.
static_assert( can_construct_at((int*)nullptr, 42));
static_assert( can_construct_at((Foo*)nullptr, 1, '2', 3.0));
static_assert(!can_construct_at((Foo*)nullptr, 1, '2'));
static_assert(!can_construct_at((Foo*)nullptr, 1, '2', 3.0, 4));
static_assert(!can_construct_at(nullptr, 1, '2', 3.0));
static_assert(!can_construct_at((int*)nullptr, 1, '2', 3.0));
static_assert(!can_construct_at(contiguous_iterator<Foo*>(), 1, '2', 3.0));
// Can't construct function pointers.
static_assert(!can_construct_at((int(*)())nullptr));
static_assert(!can_construct_at((int(*)())nullptr, nullptr));

int tc_utilities_memory_specialized_algorithms_specialized_construct_construct_at(void) {
    test();
    static_assert(test());
    return 0;
}
