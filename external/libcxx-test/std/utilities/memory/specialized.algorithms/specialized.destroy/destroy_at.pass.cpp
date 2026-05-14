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

// template <class T>
// constexpr void destroy_at(T*);

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Counted {
    int* counter_;
    TEST_CONSTEXPR Counted(int* counter) : counter_(counter) { ++*counter_; }
    TEST_CONSTEXPR_CXX20 ~Counted() { --*counter_; }
    friend void operator&(Counted) = delete;
};

struct VirtualCounted {
    int* counter_;
    TEST_CONSTEXPR VirtualCounted(int* counter) : counter_(counter) { ++*counter_; }
    TEST_CONSTEXPR_CXX20 virtual ~VirtualCounted() { --*counter_; }
    void operator&() const = delete;
};

struct DerivedCounted : VirtualCounted {
    TEST_CONSTEXPR DerivedCounted(int* counter) : VirtualCounted(counter) { }
    TEST_CONSTEXPR_CXX20 ~DerivedCounted() override { }
};

#if TEST_STD_VER > 17
constexpr bool test_arrays() {
    {
        using Array = Counted[3];
        using Alloc = std::allocator<Array>;
        Alloc alloc;
        Array* ptr = std::allocator_traits<Alloc>::allocate(alloc, 1);
        Array& arr = *ptr;

        int counter = 0;
        for (int i = 0; i != 3; ++i)
            std::allocator_traits<Alloc>::construct(alloc, std::addressof(arr[i]), &counter);
        TC_ASSERT_EXPR(counter == 3);

        std::destroy_at(ptr);
        ASSERT_SAME_TYPE(decltype(std::destroy_at(ptr)), void);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, ptr, 1);
    }
    {
        using Array = Counted[3][2];
        using Alloc = std::allocator<Array>;
        Alloc alloc;
        Array* ptr = std::allocator_traits<Alloc>::allocate(alloc, 1);
        Array& arr = *ptr;

        int counter = 0;
        for (int i = 0; i != 3; ++i)
            for (int j = 0; j != 2; ++j)
                std::allocator_traits<Alloc>::construct(alloc, std::addressof(arr[i][j]), &counter);
        TC_ASSERT_EXPR(counter == 3 * 2);

        std::destroy_at(ptr);
        ASSERT_SAME_TYPE(decltype(std::destroy_at(ptr)), void);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, ptr, 1);
    }
    return true;
}
#endif

TEST_CONSTEXPR_CXX20 bool test() {
    {
        using Alloc = std::allocator<Counted>;
        Alloc alloc;
        Counted* ptr1 = std::allocator_traits<Alloc>::allocate(alloc, 1);
        Counted* ptr2 = std::allocator_traits<Alloc>::allocate(alloc, 1);

        int counter = 0;
        std::allocator_traits<Alloc>::construct(alloc, ptr1, &counter);
        std::allocator_traits<Alloc>::construct(alloc, ptr2, &counter);
        TC_ASSERT_EXPR(counter == 2);

        std::destroy_at(ptr1);
        ASSERT_SAME_TYPE(decltype(std::destroy_at(ptr1)), void);
        TC_ASSERT_EXPR(counter == 1);

        std::destroy_at(ptr2);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, ptr1, 1);
        std::allocator_traits<Alloc>::deallocate(alloc, ptr2, 1);
    }
    {
        using Alloc = std::allocator<DerivedCounted>;
        Alloc alloc;
        DerivedCounted* ptr1 = std::allocator_traits<Alloc>::allocate(alloc, 1);
        DerivedCounted* ptr2 = std::allocator_traits<Alloc>::allocate(alloc, 1);

        int counter = 0;
        std::allocator_traits<Alloc>::construct(alloc, ptr1, &counter);
        std::allocator_traits<Alloc>::construct(alloc, ptr2, &counter);
        TC_ASSERT_EXPR(counter == 2);

        std::destroy_at(ptr1);
        ASSERT_SAME_TYPE(decltype(std::destroy_at(ptr1)), void);
        TC_ASSERT_EXPR(counter == 1);

        std::destroy_at(ptr2);
        TC_ASSERT_EXPR(counter == 0);

        std::allocator_traits<Alloc>::deallocate(alloc, ptr1, 1);
        std::allocator_traits<Alloc>::deallocate(alloc, ptr2, 1);
    }

    return true;
}

int tc_utilities_memory_specialized_algorithms_specialized_destroy_destroy_at(void) {
    test();
#if TEST_STD_VER > 17
    test_arrays();
    static_assert(test());
    // TODO: Until std::construct_at has support for arrays, it's impossible to test this
    //       in a constexpr context (see https://reviews.llvm.org/D114903).
    // static_assert(test_arrays());
#endif
    return 0;
}
