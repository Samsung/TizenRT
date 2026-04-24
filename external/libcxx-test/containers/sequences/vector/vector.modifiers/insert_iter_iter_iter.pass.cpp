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

// template <class Iter>
//   iterator insert(const_iterator position, Iter first, Iter last);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

namespace adl {
struct S {};
void make_move_iterator(S*) {}
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef std::vector<int> V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, cpp17_input_iterator<const int*>(a),
                                 cpp17_input_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int> V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const int*>(a),
                                 forward_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int> V;
        V v(100);
        while(v.size() < v.capacity()) v.push_back(0); // force reallocation
        std::size_t sz = v.size();
        int a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const int*>(a),
                                 forward_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == sz + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int> V;
        V v(100);
        v.reserve(128); // force no reallocation
        std::size_t sz = v.size();
        int a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const int*>(a),
                                 forward_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == sz + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int, limited_allocator<int, 308> > V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, cpp17_input_iterator<const int*>(a),
                                 cpp17_input_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int, limited_allocator<int, 300> > V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const int*>(a),
                                 forward_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::vector<int, min_allocator<int> > V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, cpp17_input_iterator<const int*>(a),
                                 cpp17_input_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        typedef std::vector<int, min_allocator<int> > V;
        V v(100);
        int a[] = {1, 2, 3, 4, 5};
        const int N = sizeof(a)/sizeof(a[0]);
        V::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const int*>(a),
                                 forward_iterator<const int*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
#endif

    {
        std::vector<adl::S> s;
        s.insert(s.end(), cpp17_input_iterator<adl::S*>(nullptr), cpp17_input_iterator<adl::S*>(nullptr));
    }

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_insert_iter_iter_iter(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
