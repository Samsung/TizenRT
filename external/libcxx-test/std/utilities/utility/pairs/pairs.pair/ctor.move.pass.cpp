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
// UNSUPPORTED: c++03

// <utility>

// template <class T1, class T2> struct pair

// pair(pair&&) = default;

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Dummy {
  Dummy(Dummy const&) = delete;
  Dummy(Dummy &&) = default;
};

struct NotCopyOrMoveConstructible {
  NotCopyOrMoveConstructible() = default;
  NotCopyOrMoveConstructible(NotCopyOrMoveConstructible const&) = delete;
  NotCopyOrMoveConstructible(NotCopyOrMoveConstructible&&) = delete;
};

int tc_utilities_utility_pairs_pairs_pair_ctor_move(void) {
    {
        typedef std::pair<int, short> P1;
        static_assert(std::is_move_constructible<P1>::value, "");
        P1 p1(3, static_cast<short>(4));
        P1 p2 = std::move(p1);
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
        using P = std::pair<Dummy, int>;
        static_assert(!std::is_copy_constructible<P>::value, "");
        static_assert(std::is_move_constructible<P>::value, "");
    }
    {
        // When constructing a pair containing a reference, we only bind the
        // reference, so it doesn't matter whether the type is or isn't
        // copy/move constructible.
        {
            using P = std::pair<NotCopyOrMoveConstructible&, int>;
            static_assert(std::is_move_constructible<P>::value, "");

            NotCopyOrMoveConstructible obj;
            P p2{obj, 3};
            P p1(std::move(p2));
            TC_ASSERT_EXPR(&p1.first == &obj);
            TC_ASSERT_EXPR(&p2.first == &obj);
        }
        {
            using P = std::pair<NotCopyOrMoveConstructible&&, int>;
            static_assert(std::is_move_constructible<P>::value, "");

            NotCopyOrMoveConstructible obj;
            P p2{std::move(obj), 3};
            P p1(std::move(p2));
            TC_ASSERT_EXPR(&p1.first == &obj);
            TC_ASSERT_EXPR(&p2.first == &obj);
        }
    }

    return 0;
}
