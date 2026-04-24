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

// template <class U1, class U2>
//   tuple& operator=(const pair<U1, U2>& u);

// UNSUPPORTED: c++03

#include <cassert>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

struct NothrowCopyAssignable {
    NothrowCopyAssignable(NothrowCopyAssignable const&) = delete;
    NothrowCopyAssignable& operator=(NothrowCopyAssignable const&) noexcept { return *this; }
};
struct PotentiallyThrowingCopyAssignable {
    PotentiallyThrowingCopyAssignable(PotentiallyThrowingCopyAssignable const&) = delete;
    PotentiallyThrowingCopyAssignable& operator=(PotentiallyThrowingCopyAssignable const&) { return *this; }
};

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20
bool test()
{
    {
        typedef std::pair<long, char> T0;
        typedef std::tuple<long long, short> T1;
        T0 t0(2, 'a');
        T1 t1;
        t1 = t0;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == short('a'));
    }
    return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_assign_const_pair(void) {
    test();
#if TEST_STD_VER >= 20
    static_assert(test());
#endif

    {
        // test that the implicitly generated copy assignment operator
        // is properly deleted
        using T = std::tuple<int, int>;
        using P = std::tuple<std::unique_ptr<int>, std::unique_ptr<int>>;
        static_assert(!std::is_assignable<T&, const P &>::value, "");
    }
    {
        typedef std::tuple<NothrowCopyAssignable, long> Tuple;
        typedef std::pair<NothrowCopyAssignable, int> Pair;
        static_assert(std::is_nothrow_assignable<Tuple&, Pair const&>::value, "");
        static_assert(std::is_nothrow_assignable<Tuple&, Pair&>::value, "");
        static_assert(std::is_nothrow_assignable<Tuple&, Pair const&&>::value, "");
    }
    {
        typedef std::tuple<PotentiallyThrowingCopyAssignable, long> Tuple;
        typedef std::pair<PotentiallyThrowingCopyAssignable, int> Pair;
        static_assert(std::is_assignable<Tuple&, Pair const&>::value, "");
        static_assert(!std::is_nothrow_assignable<Tuple&, Pair const&>::value, "");

        static_assert(std::is_assignable<Tuple&, Pair&>::value, "");
        static_assert(!std::is_nothrow_assignable<Tuple&, Pair&>::value, "");

        static_assert(std::is_assignable<Tuple&, Pair const&&>::value, "");
        static_assert(!std::is_nothrow_assignable<Tuple&, Pair const&&>::value, "");
    }

    return 0;
}
