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

// template <class... UTypes>
//   tuple& operator=(tuple<UTypes...>&& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct B {
    int id_;
    explicit B(int i = 0) : id_(i) {}
    virtual ~B() {}
};

struct D : B {
    explicit D(int i) : B(i) {}
};

struct E {
  constexpr E() = default;
  TEST_CONSTEXPR_CXX14 E& operator=(int) {
      return *this;
  }
};

struct NothrowMoveAssignable {
    NothrowMoveAssignable& operator=(NothrowMoveAssignable&&) noexcept { return *this; }
};

struct PotentiallyThrowingMoveAssignable {
    PotentiallyThrowingMoveAssignable& operator=(PotentiallyThrowingMoveAssignable&&) { return *this; }
};

struct NonAssignable {
  NonAssignable& operator=(NonAssignable const&) = delete;
  NonAssignable& operator=(NonAssignable&&) = delete;
};

struct MoveAssignable {
  MoveAssignable& operator=(MoveAssignable const&) = delete;
  MoveAssignable& operator=(MoveAssignable&&) = default;
};

struct CopyAssignable {
  CopyAssignable& operator=(CopyAssignable const&) = default;
  CopyAssignable& operator=(CopyAssignable&&) = delete;
};

struct TrackMove
{
    TrackMove() : value(0), moved_from(false) { }
    explicit TrackMove(int v) : value(v), moved_from(false) { }
    TrackMove(TrackMove const& other) : value(other.value), moved_from(false) { }
    TrackMove(TrackMove&& other) : value(other.value), moved_from(false) {
        other.moved_from = true;
    }
    TrackMove& operator=(TrackMove const& other) {
        value = other.value;
        moved_from = false;
        return *this;
    }
    TrackMove& operator=(TrackMove&& other) {
        value = other.value;
        moved_from = false;
        other.moved_from = true;
        return *this;
    }

    int value;
    bool moved_from;
};

TEST_CONSTEXPR_CXX20
bool test()
{
    {
        typedef std::tuple<long> T0;
        typedef std::tuple<long long> T1;
        T0 t0(2);
        T1 t1;
        t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
    }
    {
        typedef std::tuple<long, char> T0;
        typedef std::tuple<long long, int> T1;
        T0 t0(2, 'a');
        T1 t1;
        t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
    }
    {
        // Test that tuple evaluates correctly applies an lvalue reference
        // before evaluating is_assignable (i.e. 'is_assignable<int&, int&&>')
        // instead of evaluating 'is_assignable<int&&, int&&>' which is false.
        int x = 42;
        int y = 43;
        std::tuple<int&&, E> t(std::move(x), E{});
        std::tuple<int&&, int> t2(std::move(y), 44);
        t = std::move(t2);
        TC_ASSERT_EXPR(std::get<0>(t) == 43);
        TC_ASSERT_EXPR(&std::get<0>(t) == &x);
    }

    return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_assign_convert_move(void) {
    test();
#if TEST_STD_VER >= 20
    static_assert(test());
#endif

    {
        typedef std::tuple<long, char, D> T0;
        typedef std::tuple<long long, int, B> T1;
        T0 t0(2, 'a', D(3));
        T1 t1;
        t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 3);
    }
    {
        D d(3);
        D d2(2);
        typedef std::tuple<long, char, D&> T0;
        typedef std::tuple<long long, int, B&> T1;
        T0 t0(2, 'a', d2);
        T1 t1(1, 'b', d);
        t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 2);
    }
    {
        typedef std::tuple<long, char, std::unique_ptr<D>> T0;
        typedef std::tuple<long long, int, std::unique_ptr<B>> T1;
        T0 t0(2, 'a', std::unique_ptr<D>(new D(3)));
        T1 t1;
        t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1)->id_ == 3);
    }

    {
        using T = std::tuple<int, NonAssignable>;
        using U = std::tuple<NonAssignable, int>;
        static_assert(!std::is_assignable<T&, U&&>::value, "");
        static_assert(!std::is_assignable<U&, T&&>::value, "");
    }
    {
        typedef std::tuple<NothrowMoveAssignable, long> T0;
        typedef std::tuple<NothrowMoveAssignable, int> T1;
        static_assert(std::is_nothrow_assignable<T0&, T1&&>::value, "");
    }
    {
        typedef std::tuple<PotentiallyThrowingMoveAssignable, long> T0;
        typedef std::tuple<PotentiallyThrowingMoveAssignable, int> T1;
        static_assert(std::is_assignable<T0&, T1&&>::value, "");
        static_assert(!std::is_nothrow_assignable<T0&, T1&&>::value, "");
    }
    {
        // We assign through the reference and don't move out of the incoming ref,
        // so this doesn't work (but would if the type were CopyAssignable).
        {
            using T1 = std::tuple<MoveAssignable&, long>;
            using T2 = std::tuple<MoveAssignable&, int>;
            static_assert(!std::is_assignable<T1&, T2&&>::value, "");
        }

        // ... works if it's CopyAssignable
        {
            using T1 = std::tuple<CopyAssignable&, long>;
            using T2 = std::tuple<CopyAssignable&, int>;
            static_assert(std::is_assignable<T1&, T2&&>::value, "");
        }

        // For rvalue-references, we can move-assign if the type is MoveAssignable
        // or CopyAssignable (since in the worst case the move will decay into a copy).
        {
            using T1 = std::tuple<MoveAssignable&&, long>;
            using T2 = std::tuple<MoveAssignable&&, int>;
            static_assert(std::is_assignable<T1&, T2&&>::value, "");

            using T3 = std::tuple<CopyAssignable&&, long>;
            using T4 = std::tuple<CopyAssignable&&, int>;
            static_assert(std::is_assignable<T3&, T4&&>::value, "");
        }

        // In all cases, we can't move-assign if the types are not assignable,
        // since we assign through the reference.
        {
            using T1 = std::tuple<NonAssignable&, long>;
            using T2 = std::tuple<NonAssignable&, int>;
            static_assert(!std::is_assignable<T1&, T2&&>::value, "");

            using T3 = std::tuple<NonAssignable&&, long>;
            using T4 = std::tuple<NonAssignable&&, int>;
            static_assert(!std::is_assignable<T3&, T4&&>::value, "");
        }
    }
    {
        // Make sure that we don't incorrectly move out of the source's reference.
        using Dest = std::tuple<TrackMove, long>;
        using Source = std::tuple<TrackMove&, int>;
        TrackMove track{3};
        Source src(track, 4);
        TC_ASSERT_EXPR(!track.moved_from);

        Dest dst;
        dst = std::move(src); // here we should make a copy
        TC_ASSERT_EXPR(!track.moved_from);
        TC_ASSERT_EXPR(std::get<0>(dst).value == 3);
    }
    {
        // But we do move out of the source's reference if it's a rvalue ref
        using Dest = std::tuple<TrackMove, long>;
        using Source = std::tuple<TrackMove&&, int>;
        TrackMove track{3};
        Source src(std::move(track), 4);
        TC_ASSERT_EXPR(!track.moved_from); // we just took a reference

        Dest dst;
        dst = std::move(src);
        TC_ASSERT_EXPR(track.moved_from);
        TC_ASSERT_EXPR(std::get<0>(dst).value == 3);
    }
    {
        // If the source holds a value, then we move out of it too
        using Dest = std::tuple<TrackMove, long>;
        using Source = std::tuple<TrackMove, int>;
        Source src(TrackMove{3}, 4);
        Dest dst;
        dst = std::move(src);
        TC_ASSERT_EXPR(std::get<0>(src).moved_from);
        TC_ASSERT_EXPR(std::get<0>(dst).value == 3);
    }

    return 0;
}
