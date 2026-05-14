//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// T *data();

#include <array>
#include <cassert>
#include <cstddef>       // for std::max_align_t
#include <cstdint>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NoDefault {
    TEST_CONSTEXPR NoDefault(int) { }
};

#if TEST_STD_VER < 11
struct natural_alignment {
    long t1;
    long long t2;
    double t3;
    long double t4;
};
#endif

TEST_CONSTEXPR_CXX17 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c = {1, 2, 3.5};
        ASSERT_NOEXCEPT(c.data());
        T* p = c.data();
        TC_ASSERT_EXPR(p[0] == 1);
        TC_ASSERT_EXPR(p[1] == 2);
        TC_ASSERT_EXPR(p[2] == 3.5);
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c = {};
        ASSERT_NOEXCEPT(c.data());
        T* p = c.data();
        (void)p;
    }
    {
        typedef double T;
        typedef std::array<const T, 0> C;
        C c = {{}};
        ASSERT_NOEXCEPT(c.data());
        const T* p = c.data();
        (void)p;
        static_assert((std::is_same<decltype(c.data()), const T*>::value), "");
    }
    {
        typedef NoDefault T;
        typedef std::array<T, 0> C;
        C c = {};
        ASSERT_NOEXCEPT(c.data());
        T* p = c.data();
        (void)p;
    }
    {
        std::array<int, 5> c = {0, 1, 2, 3, 4};
        TC_ASSERT_EXPR(c.data() == &c[0]);
        TC_ASSERT_EXPR(*c.data() == c[0]);
    }

    return true;
}

int tc_containers_sequences_array_array_data_data(void) {
    tests();
#if TEST_STD_VER >= 17
    static_assert(tests(), "");
#endif

    // Test the alignment of data()
    {
#if TEST_STD_VER < 11
        typedef natural_alignment T;
#else
        typedef std::max_align_t T;
#endif
        typedef std::array<T, 0> C;
        const C c = {};
        const T* p = c.data();
        std::uintptr_t pint = reinterpret_cast<std::uintptr_t>(p);
        TC_ASSERT_EXPR(pint % TEST_ALIGNOF(T) == 0);
    }
    return 0;
}
