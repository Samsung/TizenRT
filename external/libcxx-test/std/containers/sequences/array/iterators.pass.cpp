//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// iterator begin() noexcept;                         // constexpr in C++17
// const_iterator begin() const noexcept;             // constexpr in C++17
// iterator end() noexcept;                           // constexpr in C++17
// const_iterator end() const noexcept;               // constexpr in C++17
//
// reverse_iterator rbegin() noexcept;                // constexpr in C++17
// const_reverse_iterator rbegin() const noexcept;    // constexpr in C++17
// reverse_iterator rend() noexcept;                  // constexpr in C++17
// const_reverse_iterator rend() const noexcept;      // constexpr in C++17
//
// const_iterator cbegin() const noexcept;            // constexpr in C++17
// const_iterator cend() const noexcept;              // constexpr in C++17
// const_reverse_iterator crbegin() const noexcept;   // constexpr in C++17
// const_reverse_iterator crend() const noexcept;     // constexpr in C++17

#include <array>
#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NoDefault {
    TEST_CONSTEXPR NoDefault(int) { }
};

template <class T>
TEST_CONSTEXPR_CXX17 void check_noexcept(T& c) {
    ASSERT_NOEXCEPT(c.begin());
    ASSERT_NOEXCEPT(c.end());
    ASSERT_NOEXCEPT(c.cbegin());
    ASSERT_NOEXCEPT(c.cend());
    ASSERT_NOEXCEPT(c.rbegin());
    ASSERT_NOEXCEPT(c.rend());
    ASSERT_NOEXCEPT(c.crbegin());
    ASSERT_NOEXCEPT(c.crend());

    const T& cc = c; (void)cc;
    ASSERT_NOEXCEPT(cc.begin());
    ASSERT_NOEXCEPT(cc.end());
    ASSERT_NOEXCEPT(cc.rbegin());
    ASSERT_NOEXCEPT(cc.rend());
}

TEST_CONSTEXPR_CXX17 bool tests()
{
    {
        typedef std::array<int, 5> C;
        C array = {};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        typename C::const_iterator j = array.cbegin();
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef std::array<int, 0> C;
        C array = {};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        typename C::const_iterator j = array.cbegin();
        TC_ASSERT_EXPR(i == j);
    }

    {
        typedef std::array<int, 0> C;
        C array = {};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        typename C::const_iterator j = array.cbegin();
        TC_ASSERT_EXPR(i == array.end());
        TC_ASSERT_EXPR(j == array.cend());
    }
    {
        typedef std::array<int, 1> C;
        C array = {1};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        TC_ASSERT_EXPR(*i == 1);
        TC_ASSERT_EXPR(&*i == array.data());
        *i = 99;
        TC_ASSERT_EXPR(array[0] == 99);
    }
    {
        typedef std::array<int, 2> C;
        C array = {1, 2};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        TC_ASSERT_EXPR(*i == 1);
        TC_ASSERT_EXPR(&*i == array.data());
        *i = 99;
        TC_ASSERT_EXPR(array[0] == 99);
        TC_ASSERT_EXPR(array[1] == 2);
    }
    {
        typedef std::array<double, 3> C;
        C array = {1, 2, 3.5};
        check_noexcept(array);
        typename C::iterator i = array.begin();
        TC_ASSERT_EXPR(*i == 1);
        TC_ASSERT_EXPR(&*i == array.data());
        *i = 5.5;
        TC_ASSERT_EXPR(array[0] == 5.5);
        TC_ASSERT_EXPR(array[1] == 2.0);
    }
    {
        typedef std::array<NoDefault, 0> C;
        C array = {};
        typename C::iterator ib = array.begin();
        typename C::iterator ie = array.end();
        TC_ASSERT_EXPR(ib == ie);
    }

#if TEST_STD_VER >= 14
    { // N3644 testing
        {
            typedef std::array<int, 5> C;
            C::iterator ii1{}, ii2{};
            C::iterator ii4 = ii1;
            C::const_iterator cii{};
            TC_ASSERT_EXPR(ii1 == ii2);
            TC_ASSERT_EXPR(ii1 == ii4);
            TC_ASSERT_EXPR(ii1 == cii);

            TC_ASSERT_EXPR(!(ii1 != ii2));
            TC_ASSERT_EXPR(!(ii1 != cii));

            C c = {};
            check_noexcept(c);
            TC_ASSERT_EXPR(c.begin()   == std::begin(c));
            TC_ASSERT_EXPR(c.cbegin()  == std::cbegin(c));
            TC_ASSERT_EXPR(c.rbegin()  == std::rbegin(c));
            TC_ASSERT_EXPR(c.crbegin() == std::crbegin(c));
            TC_ASSERT_EXPR(c.end()     == std::end(c));
            TC_ASSERT_EXPR(c.cend()    == std::cend(c));
            TC_ASSERT_EXPR(c.rend()    == std::rend(c));
            TC_ASSERT_EXPR(c.crend()   == std::crend(c));

            TC_ASSERT_EXPR(std::begin(c)   != std::end(c));
            TC_ASSERT_EXPR(std::rbegin(c)  != std::rend(c));
            TC_ASSERT_EXPR(std::cbegin(c)  != std::cend(c));
            TC_ASSERT_EXPR(std::crbegin(c) != std::crend(c));
        }
        {
            typedef std::array<int, 0> C;
            C::iterator ii1{}, ii2{};
            C::iterator ii4 = ii1;
            C::const_iterator cii{};
            TC_ASSERT_EXPR(ii1 == ii2);
            TC_ASSERT_EXPR(ii1 == ii4);

            TC_ASSERT_EXPR(!(ii1 != ii2));

            TC_ASSERT_EXPR( (ii1 == cii));
            TC_ASSERT_EXPR( (cii == ii1));
            TC_ASSERT_EXPR(!(ii1 != cii));
            TC_ASSERT_EXPR(!(cii != ii1));
            TC_ASSERT_EXPR(!(ii1 <  cii));
            TC_ASSERT_EXPR(!(cii <  ii1));
            TC_ASSERT_EXPR( (ii1 <= cii));
            TC_ASSERT_EXPR( (cii <= ii1));
            TC_ASSERT_EXPR(!(ii1 >  cii));
            TC_ASSERT_EXPR(!(cii >  ii1));
            TC_ASSERT_EXPR( (ii1 >= cii));
            TC_ASSERT_EXPR( (cii >= ii1));
            TC_ASSERT_EXPR(cii - ii1 == 0);
            TC_ASSERT_EXPR(ii1 - cii == 0);

            C c = {};
            check_noexcept(c);
            TC_ASSERT_EXPR(c.begin()   == std::begin(c));
            TC_ASSERT_EXPR(c.cbegin()  == std::cbegin(c));
            TC_ASSERT_EXPR(c.rbegin()  == std::rbegin(c));
            TC_ASSERT_EXPR(c.crbegin() == std::crbegin(c));
            TC_ASSERT_EXPR(c.end()     == std::end(c));
            TC_ASSERT_EXPR(c.cend()    == std::cend(c));
            TC_ASSERT_EXPR(c.rend()    == std::rend(c));
            TC_ASSERT_EXPR(c.crend()   == std::crend(c));

            TC_ASSERT_EXPR(std::begin(c)   == std::end(c));
            TC_ASSERT_EXPR(std::rbegin(c)  == std::rend(c));
            TC_ASSERT_EXPR(std::cbegin(c)  == std::cend(c));
            TC_ASSERT_EXPR(std::crbegin(c) == std::crend(c));
        }
    }
#endif
    return true;
}

int tc_containers_sequences_array_iterators(void) {
    tests();
#if TEST_STD_VER >= 17
    static_assert(tests(), "");
#endif
  return 0;
}
