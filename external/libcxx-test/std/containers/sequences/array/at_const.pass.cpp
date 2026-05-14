//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// const_reference at (size_type) const; // constexpr in C++14

#include <array>
#include <cassert>

#ifndef TEST_HAS_NO_EXCEPTIONS
#include <stdexcept>
#endif

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C const c = {1, 2, 3.5};
        typename C::const_reference r1 = c.at(0);
        TC_ASSERT_EXPR(r1 == 1);

        typename C::const_reference r2 = c.at(2);
        TC_ASSERT_EXPR(r2 == 3.5);
    }
    return true;
}

void test_exceptions()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::array<int, 4> const array = {1, 2, 3, 4};

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD array.at(4);
            TC_ASSERT_EXPR(false);
        } catch (std::out_of_range const&) {
            // pass
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD array.at(5);
            TC_ASSERT_EXPR(false);
        } catch (std::out_of_range const&) {
            // pass
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD array.at(6);
            TC_ASSERT_EXPR(false);
        } catch (std::out_of_range const&) {
            // pass
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            using size_type = decltype(array)::size_type;
            TEST_IGNORE_NODISCARD array.at(static_cast<size_type>(-1));
            TC_ASSERT_EXPR(false);
        } catch (std::out_of_range const&) {
            // pass
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    {
        std::array<int, 0> array = {};

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD array.at(0);
            TC_ASSERT_EXPR(false);
        } catch (std::out_of_range const&) {
            // pass
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
#endif
}

int tc_containers_sequences_array_at_const(void) {
    tests();
    test_exceptions();

#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif
    return 0;
}
