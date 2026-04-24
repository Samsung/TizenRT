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
// <tuple>

// template <class... Types> class tuple;

// template <class U1, class U2> tuple(pair<U1, U2>&& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct B
{
    int id_;

    explicit B(int i) : id_(i) {}

    virtual ~B() {}
};

struct D
    : B
{
    explicit D(int i) : B(i) {}
};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_move_pair(void) {
    {
        typedef std::pair<long, std::unique_ptr<D>> T0;
        typedef std::tuple<long long, std::unique_ptr<B>> T1;
        T0 t0(2, std::unique_ptr<D>(new D(3)));
        T1 t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1)->id_ == 3);
    }

#if TEST_STD_VER > 11
    {
        using pair_t = std::pair<int, char>;
        constexpr std::tuple<long, long> t(pair_t(0, 'a'));
        static_assert(std::get<0>(t) == 0, "");
        static_assert(std::get<1>(t) == 'a', "");
    }
#endif

  return 0;
}
