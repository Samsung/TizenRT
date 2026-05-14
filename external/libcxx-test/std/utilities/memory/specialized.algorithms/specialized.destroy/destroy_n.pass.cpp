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
// UNSUPPORTED: c++03, c++11, c++14

// <memory>

// template <class ForwardIt, class Size>
// constexpr ForwardIt destroy_n(ForwardIt, Size s);

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct Counted {
    int* counter_;
    TEST_CONSTEXPR Counted(int* counter) : counter_(counter) { ++*counter_; }
    TEST_CONSTEXPR Counted(Counted const& other) : counter_(other.counter_) { ++*counter_; }
    TEST_CONSTEXPR_CXX20 ~Counted() { --*counter_; }
    friend void operator&(Counted) = delete;
};

#if TEST_STD_VER > 17
constexpr bool test_arrays()  {
    {
        using Array = Counted[3];
        using Alloc = std::allocator<Array>;
        int counter = 0;
        Alloc alloc;
        Array* pool = std::allocator_traits<Alloc>::allocate(alloc, 5);

        for (Array* p = pool; p != pool + 5; ++p) {
            Array& arr = *p;
            for (int i = 0; i != 3; ++i) {
                std::allocator_traits<Alloc>::construct(alloc, std::addressof(arr[i]), &counter);
            }
        }
        TC_ASSERT_EXPR(counter == 5 * 3);

        Array* p = std::destroy_n(pool, 5);
        ASSERT_SAME_TYPE(decltype(std::destroy_n(pool, 5)), Array*);
        TC_ASSERT_EXPR(p == pool + 5);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, pool, 5);
    }
    {
        using Array = Counted[3][2];
        using Alloc = std::allocator<Array>;
        int counter = 0;
        Alloc alloc;
        Array* pool = std::allocator_traits<Alloc>::allocate(alloc, 5);

        for (Array* p = pool; p != pool + 5; ++p) {
            Array& arr = *p;
            for (int i = 0; i != 3; ++i) {
                for (int j = 0; j != 2; ++j) {
                    std::allocator_traits<Alloc>::construct(alloc, std::addressof(arr[i][j]), &counter);
                }
            }
        }
        TC_ASSERT_EXPR(counter == 5 * 3 * 2);

        Array* p = std::destroy_n(pool, 5);
        ASSERT_SAME_TYPE(decltype(std::destroy_n(pool, 5)), Array*);
        TC_ASSERT_EXPR(p == pool + 5);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, pool, 5);
    }

    return true;
}
#endif

template <class It>
TEST_CONSTEXPR_CXX20 void test() {
    using Alloc = std::allocator<Counted>;
    int counter = 0;
    Alloc alloc;
    Counted* pool = std::allocator_traits<Alloc>::allocate(alloc, 5);

    for (Counted* p = pool; p != pool + 5; ++p)
        std::allocator_traits<Alloc>::construct(alloc, p, &counter);
    TC_ASSERT_EXPR(counter == 5);

    It it = std::destroy_n(It(pool), 5);
    ASSERT_SAME_TYPE(decltype(std::destroy_n(It(pool), 5)), It);
    TC_ASSERT_EXPR(it == It(pool + 5));
    TC_ASSERT_EXPR(counter == 0);

    std::allocator_traits<Alloc>::deallocate(alloc, pool, 5);
}

TEST_CONSTEXPR_CXX20 bool tests() {
    test<Counted*>();
    test<forward_iterator<Counted*>>();
    return true;
}

int tc_utilities_memory_specialized_algorithms_specialized_destroy_destroy_n(void) {
    tests();
#if TEST_STD_VER > 17
    test_arrays();
    static_assert(tests());
    // TODO: Until std::construct_at has support for arrays, it's impossible to test this
    //       in a constexpr context (see https://reviews.llvm.org/D114903).
    // static_assert(test_arrays());
#endif
    return 0;
}
