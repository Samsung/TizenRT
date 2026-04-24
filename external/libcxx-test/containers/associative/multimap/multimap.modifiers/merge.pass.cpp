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

// <map>

// class multimap

// template <class C2>
//   void merge(map<key_type, value_type, C2, allocator_type>& source);
// template <class C2>
//   void merge(map<key_type, value_type, C2, allocator_type>&& source);
// template <class C2>
//   void merge(multimap<key_type, value_type, C2, allocator_type>& source);
// template <class C2>
//   void merge(multimap<key_type, value_type, C2, allocator_type>&& source);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

template <class Map>
bool map_equal(const Map& map, Map other)
{
    return map == other;
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

int tc_containers_associative_multimap_multimap_modifiers_merge(void) {
    {
        std::multimap<int, int> src{{1, 0}, {3, 0}, {5, 0}};
        std::multimap<int, int> dst{{2, 0}, {4, 0}, {5, 0}};
        dst.merge(src);
        TC_ASSERT_EXPR(map_equal(src, {}));
        TC_ASSERT_EXPR(map_equal(dst, {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}}));
    }

#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        bool do_throw = false;
        typedef std::multimap<Counter<int>, int, throw_comparator> map_type;
        map_type src({{1, 0}, {3, 0}, {5, 0}}, throw_comparator(do_throw));
        map_type dst({{2, 0}, {4, 0}, {5, 0}}, throw_comparator(do_throw));

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
        TC_ASSERT_EXPR(map_equal(src, map_type({{1, 0}, {3, 0}, {5, 0}}, throw_comparator(do_throw))));
        TC_ASSERT_EXPR(map_equal(dst, map_type({{2, 0}, {4, 0}, {5, 0}}, throw_comparator(do_throw))));
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
        typedef std::multimap<Counter<int>, int, std::less<Counter<int>>> first_map_type;
        typedef std::multimap<Counter<int>, int, comparator> second_map_type;
        typedef std::map<Counter<int>, int, comparator> third_map_type;

        {
            first_map_type first{{1, 0}, {2, 0}, {3, 0}};
            second_map_type second{{2, 0}, {3, 0}, {4, 0}};
            third_map_type third{{1, 0}, {3, 0}};

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);

            first.merge(second);
            first.merge(third);

            TC_ASSERT_EXPR(map_equal(first, {{1, 0}, {1, 0}, {2, 0}, {2, 0}, {3, 0}, {3, 0}, {3, 0}, {4, 0}}));
            TC_ASSERT_EXPR(map_equal(second, {}));
            TC_ASSERT_EXPR(map_equal(third, {}));

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
        {
            first_map_type first{{1, 0}, {2, 0}, {3, 0}};
            second_map_type second{{2, 0}, {3, 0}, {4, 0}};
            third_map_type third{{1, 0}, {3, 0}};

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);

            first.merge(std::move(second));
            first.merge(std::move(third));

            TC_ASSERT_EXPR(map_equal(first, {{1, 0}, {1, 0}, {2, 0}, {2, 0}, {3, 0}, {3, 0}, {3, 0}, {4, 0}}));
            TC_ASSERT_EXPR(map_equal(second, {}));
            TC_ASSERT_EXPR(map_equal(third, {}));

            TC_ASSERT_EXPR(Counter_base::gConstructed == 8);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }
    TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    {
        std::multimap<int, int> first;
        {
            std::multimap<int, int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
        {
            std::multimap<int, int> second;
            first.merge(second);
            first.merge(std::move(second));
        }
    }
    return 0;
}
