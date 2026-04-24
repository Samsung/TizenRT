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

// iterator erase(const_iterator first, const_iterator last);

#include <vector>
#include <iterator>
#include <cassert>

#include "asan_testing.h"
#include "min_allocator.h"
#include "MoveOnly.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct Throws {
    Throws() : v_(0) {}
    Throws(int v) : v_(v) {}
    Throws(const Throws  &rhs) : v_(rhs.v_) { if (sThrows) throw 1; }
    Throws(      Throws &&rhs) : v_(rhs.v_) { if (sThrows) throw 1; }
    Throws& operator=(const Throws  &rhs) { v_ = rhs.v_; return *this; }
    Throws& operator=(      Throws &&rhs) { v_ = rhs.v_; return *this; }
    int v_;
    static bool sThrows;
    };

bool Throws::sThrows = false;
#endif

TEST_CONSTEXPR_CXX20 bool tests()
{
    int a1[] = {1, 2, 3};
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<int>(a1+1, a1+3));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<int>(a1+2, a1+3));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<std::vector<int> > outer(2, std::vector<int>(1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
        outer.erase(outer.begin(), outer.begin());
        TC_ASSERT_EXPR(outer.size() == 2);
        TC_ASSERT_EXPR(outer[0].size() == 1);
        TC_ASSERT_EXPR(outer[1].size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
    }
    // Make sure vector::erase works with move-only types
    {
        // When non-trivial
        {
            std::vector<MoveOnly> v;
            v.emplace_back(1); v.emplace_back(2); v.emplace_back(3);
            v.erase(v.begin(), v.begin() + 2);
            TC_ASSERT_EXPR(v.size() == 1);
            TC_ASSERT_EXPR(v[0] == MoveOnly(3));
        }
        // When trivial
        {
            std::vector<TrivialMoveOnly> v;
            v.emplace_back(1); v.emplace_back(2); v.emplace_back(3);
            v.erase(v.begin(), v.begin() + 2);
            TC_ASSERT_EXPR(v.size() == 1);
            TC_ASSERT_EXPR(v[0] == TrivialMoveOnly(3));
        }
    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int, min_allocator<int>> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR((l1 == std::vector<int, min_allocator<int>>(a1+1, a1+3)));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int, min_allocator<int>> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR((l1 == std::vector<int, min_allocator<int>>(a1+2, a1+3)));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int, min_allocator<int>> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<std::vector<int, min_allocator<int>>, min_allocator<std::vector<int, min_allocator<int>>>> outer(2, std::vector<int, min_allocator<int>>(1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
        outer.erase(outer.begin(), outer.begin());
        TC_ASSERT_EXPR(outer.size() == 2);
        TC_ASSERT_EXPR(outer[0].size() == 1);
        TC_ASSERT_EXPR(outer[1].size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_erase_iter_iter(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif

#ifndef TEST_HAS_NO_EXCEPTIONS
// Test for LWG2853:
// Throws: Nothing unless an exception is thrown by the assignment operator or move assignment operator of T.
    {
        Throws arr[] = {1, 2, 3};
        std::vector<Throws> v(arr, arr+3);
        Throws::sThrows = true;
        v.erase(v.begin(), --v.end());
        TC_ASSERT_EXPR(v.size() == 1);
        v.erase(v.begin(), v.end());
        TC_ASSERT_EXPR(v.size() == 0);
    }
#endif

    return 0;
}
