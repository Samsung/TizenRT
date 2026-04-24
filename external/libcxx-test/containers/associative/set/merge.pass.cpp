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

// <set>

// class set

// template <class C2>
//   void merge(set<key_type, C2, allocator_type>& source);
// template <class C2>
//   void merge(set<key_type, C2, allocator_type>&& source);
// template <class C2>
//   void merge(multiset<key_type, C2, allocator_type>& source);
// template <class C2>
//   void merge(multiset<key_type, C2, allocator_type>&& source);

#include <set>
#include <cassert>
#include "test_macros.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

template <class Set>
bool set_equal(const Set& set, Set other)
{
    return set == other;
}

#ifndef TEST_HAS_NO_EXCEPTIONS
struct throw_comparator
{
    bool& should_throw_;

    throw_comparator(bool& should_throw) : should_throw_(should_throw) {}

    template <class T>
    bool operator()(const T& lhs, const T& rhs) const
    {
        if (should_throw_)
#ifndef _LIBCPP_NO_EXCEPTIONS
            throw 0;
        return lhs < rhs;
    }
#endif // _LIBCPP_NO_EXCEPTIONS
};
#endif

int tc_containers_associative_set_merge(void) {
    {
        std::set<int> src{1, 3, 5};
        std::set<int> dst{2, 4, 5};
        dst.merge(src);
        TC_ASSERT_EXPR(set_equal(src, {5}));
        TC_ASSERT_EXPR(set_equal(dst, {1, 2, 3, 4, 5}));
    }

#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        bool do_throw = false;
        typedef std::set<Counter<int>, throw_comparator> set_type;
        set_type src({1, 3, 5}, throw_comparator(do_throw));
        set_type dst({2, 4, 5}, throw_comparator(do_throw));

        TC_ASSERT_EXPR(Counter_base::gConstructed == 6);

        do_throw = true;
        try
        {
            dst.merge(src);
        }
        catch (int)
        {
            do_throw = false;
        }
        TC_ASSERT_EXPR(!do_throw);
        TC_ASSERT_EXPR(set_equal(src, set_type({1, 3, 5}, throw_comparator(do_throw))));
        TC_ASSERT_EXPR(set_equal(dst, set_type({2, 4, 5}, throw_comparator(do_throw))));
    }
#endif
    TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    struct comparator
    {
        comparator() = default;

        bool operator()(const Counter<int>& lhs, const Counter<int>& rhs) const
        {
            return lhs < rhs;
        }
    };
    {
        typedef std::set<Counter<int>, std::less<Counter<int>>> first_set_type;
        typedef std::set<Counter<int>, comparator> second_set_type;
        typedef std::multiset<Counter<int>, comparator> third_set_type;

        {
            first_set_type first{1, 2, 3};
            second_set_type second{2, 3, 4};
            third_set_type third{1, 3};

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);

            first.merge(second);
            first.merge(third);

            TC_ASSERT_EXPR(set_equal(first, {1, 2, 3, 4}));
            TC_ASSERT_EXPR(set_equal(second, {2, 3}));
            TC_ASSERT_EXPR(set_equal(third, {1, 3}));

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
        {
            first_set_type first{1, 2, 3};
            second_set_type second{2, 3, 4};
            third_set_type third{1, 3};

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);

            first.merge(std::move(second));
            first.merge(std::move(third));

            TC_ASSERT_EXPR(set_equal(first, {1, 2, 3, 4}));
            TC_ASSERT_EXPR(set_equal(second, {2, 3}));
            TC_ASSERT_EXPR(set_equal(third, {1, 3}));

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }
    {
        std::set<int> first;
        {
            std::set<int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
        {
            std::multiset<int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
    }
    return 0;
}
