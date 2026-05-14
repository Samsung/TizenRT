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

// <unordered_set>

// class unordered_multiset

// template <class H2, class P2>
//   void merge(unordered_set<key_type, H2, P2, allocator_type>& source);
// template <class H2, class P2>
//   void merge(unordered_set<key_type, H2, P2, allocator_type>&& source);
// template <class H2, class P2>
//   void merge(unordered_multiset<key_type, H2, P2, allocator_type>& source);
// template <class H2, class P2>
//   void merge(unordered_multiset<key_type, H2, P2, allocator_type>&& source);

#include <unordered_set>
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
template <class T>
struct throw_hasher
{
    bool& should_throw_;

    throw_hasher(bool& should_throw) : should_throw_(should_throw) {}

    std::size_t operator()(const T& p) const
    {
        if (should_throw_)
#ifndef _LIBCPP_NO_EXCEPTIONS
            throw 0;
        return std::hash<T>()(p);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
};
#endif

int tc_containers_unord_unord_multiset_merge(void) {
    {
        std::unordered_multiset<int> src{1, 3, 5};
        std::unordered_multiset<int> dst{2, 4, 5};
        dst.merge(src);
        TC_ASSERT_EXPR(set_equal(src, {}));
        TC_ASSERT_EXPR(set_equal(dst, {1, 2, 3, 4, 5, 5}));
    }

#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        bool do_throw = false;
        typedef std::unordered_multiset<Counter<int>, throw_hasher<Counter<int>>> set_type;
        set_type src({1, 3, 5}, 0, throw_hasher<Counter<int>>(do_throw));
        set_type dst({2, 4, 5}, 0, throw_hasher<Counter<int>>(do_throw));

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
        TC_ASSERT_EXPR(set_equal(src, set_type({1, 3, 5}, 0, throw_hasher<Counter<int>>(do_throw))));
        TC_ASSERT_EXPR(set_equal(dst, set_type({2, 4, 5}, 0, throw_hasher<Counter<int>>(do_throw))));
    }
#endif
    TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    struct equal
    {
        equal() = default;

        bool operator()(const Counter<int>& lhs, const Counter<int>& rhs) const
        {
            return lhs == rhs;
        }
    };
    struct hasher
    {
        hasher() = default;
        std::size_t operator()(const Counter<int>& p) const { return std::hash<Counter<int>>()(p); }
    };
    {
        typedef std::unordered_multiset<Counter<int>, std::hash<Counter<int>>, std::equal_to<Counter<int>>> first_set_type;
        typedef std::unordered_multiset<Counter<int>, hasher, equal> second_set_type;
        typedef std::unordered_set<Counter<int>, hasher, equal> third_set_type;

        {
            first_set_type first{1, 2, 3};
            second_set_type second{2, 3, 4};
            third_set_type third{1, 3};

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);

            first.merge(second);
            first.merge(third);

            TC_ASSERT_EXPR(set_equal(first, {1, 2, 3, 4, 2, 3, 1, 3}));
            TC_ASSERT_EXPR(set_equal(second, {}));
            TC_ASSERT_EXPR(set_equal(third, {}));

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

            TC_ASSERT_EXPR(set_equal(first, {1, 2, 3, 4, 2, 3, 1, 3}));
            TC_ASSERT_EXPR(set_equal(second, {}));
            TC_ASSERT_EXPR(set_equal(third, {}));

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }
    {
        std::unordered_multiset<int> first;
        {
            std::unordered_multiset<int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
        {
            std::unordered_set<int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
    }
    return 0;
}
