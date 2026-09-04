//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// reverse_iterator       rbegin();
// reverse_iterator       rend();
// const_reverse_iterator rbegin()  const;
// const_reverse_iterator rend()    const;
// const_reverse_iterator crbegin() const;
// const_reverse_iterator crend()   const;

#include <vector>
#include <cassert>
#include <iterator>

#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class Vector>
TEST_CONSTEXPR_CXX20 void check_vector_reverse_iterators() {
    {
        Vector vec;
        TC_ASSERT_EXPR(vec.rbegin() == vec.rend());
        TC_ASSERT_EXPR(vec.crbegin() == vec.crend());
    }
    {
        const int n = 10;
        Vector vec;
        const Vector& cvec = vec;
        vec.reserve(n);
        for (int i = 0; i < n; ++i)
            vec.push_back(i);
        {
            int iterations = 0;

            for (typename Vector::const_reverse_iterator it = vec.crbegin(); it != vec.crend(); ++it) {
                TC_ASSERT_EXPR(*it == (n - iterations - 1));
                ++iterations;
            }
            TC_ASSERT_EXPR(iterations == n);
        }
        {
            TC_ASSERT_EXPR(cvec.rbegin() == vec.crbegin());
            TC_ASSERT_EXPR(cvec.rend() == vec.crend());
        }
        {
            int iterations = 0;

            for (typename Vector::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) {
                TC_ASSERT_EXPR(*it == (n - iterations - 1));
                *it = 40;
                TC_ASSERT_EXPR(*it == 40);
                ++iterations;
            }
            TC_ASSERT_EXPR(iterations == n);
        }

        TC_ASSERT_EXPR(std::distance(vec.rbegin(), vec.rend()) == n);
        TC_ASSERT_EXPR(std::distance(cvec.rbegin(), cvec.rend()) == n);
        TC_ASSERT_EXPR(std::distance(vec.crbegin(), vec.crend()) == n);
        TC_ASSERT_EXPR(std::distance(cvec.crbegin(), cvec.crend()) == n);
    }
}

TEST_CONSTEXPR_CXX20 bool test() {
    check_vector_reverse_iterators<std::vector<int> >();
#if TEST_STD_VER >= 11
    check_vector_reverse_iterators<std::vector<int, min_allocator<int> > >();
#endif

    return true;
}

int tc_containers_sequences_vector_reverse_iterators(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
