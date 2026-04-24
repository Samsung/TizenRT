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
// <vector>

//       reference operator[](size_type __i);
// const_reference operator[](size_type __i) const;
//
//       reference at(size_type __i);
// const_reference at(size_type __i) const;
//
//       reference front();
// const_reference front() const;
//
//       reference back();
// const_reference back() const;
// libc++ marks these as 'noexcept' (except 'at')

#include <vector>
#include <cassert>
#include <stdexcept>

#include "min_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 C make(int size, int start) {
    C c;
    for (int i = 0; i < size; ++i)
        c.push_back(start + i);
    return c;
}

template <class Vector>
TEST_CONSTEXPR_CXX20 void test_get_basic(Vector& c, int start_value) {
    const int n = static_cast<int>(c.size());
    for (int i = 0; i < n; ++i)
        TC_ASSERT_EXPR(c[i] == start_value + i);
    for (int i = 0; i < n; ++i)
        TC_ASSERT_EXPR(c.at(i) == start_value + i);

#ifndef TEST_HAS_NO_EXCEPTIONS
    if (!TEST_IS_CONSTANT_EVALUATED) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD c.at(n);
            TC_ASSERT_EXPR(false);
        } catch (const std::out_of_range&) {}
    }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

    TC_ASSERT_EXPR(c.front() == start_value);
    TC_ASSERT_EXPR(c.back() == start_value + n - 1);
}

template <class Vector>
TEST_CONSTEXPR_CXX20 void test_get() {
    int start_value = 35;
    Vector c = make<Vector>(10, start_value);
    const Vector& cc = c;
    test_get_basic(c, start_value);
    test_get_basic(cc, start_value);
}

template <class Vector>
TEST_CONSTEXPR_CXX20 void test_set() {
    int start_value = 35;
    const int n = 10;
    Vector c = make<Vector>(n, start_value);

    for (int i = 0; i < n; ++i) {
        TC_ASSERT_EXPR(c[i] == start_value + i);
        c[i] = start_value + i + 1;
        TC_ASSERT_EXPR(c[i] == start_value + i + 1);
    }
    for (int i = 0; i < n; ++i) {
        TC_ASSERT_EXPR(c.at(i) == start_value + i + 1);
        c.at(i) = start_value + i + 2;
        TC_ASSERT_EXPR(c.at(i) == start_value + i + 2);
    }

    TC_ASSERT_EXPR(c.front() == start_value + 2);
    c.front() = start_value + 3;
    TC_ASSERT_EXPR(c.front() == start_value + 3);

    TC_ASSERT_EXPR(c.back() == start_value + n + 1);
    c.back() = start_value + n + 2;
    TC_ASSERT_EXPR(c.back() == start_value + n + 2);
}

template <class Vector>
TEST_CONSTEXPR_CXX20 void test() {
    test_get<Vector>();
    test_set<Vector>();

    Vector c;
    const Vector& cc = c;
    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c[0]));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc[0]));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.at(0)));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.at(0)));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.front()));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.front()));

    ASSERT_SAME_TYPE(typename Vector::reference, decltype(c.back()));
    ASSERT_SAME_TYPE(typename Vector::const_reference, decltype(cc.back()));
}

TEST_CONSTEXPR_CXX20 bool tests() {
    test<std::vector<int> >();
#if TEST_STD_VER >= 11
    test<std::vector<int, min_allocator<int> > >();
    test<std::vector<int, safe_allocator<int> > >();
#endif
    return true;
}

int tc_containers_sequences_vector_access(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
