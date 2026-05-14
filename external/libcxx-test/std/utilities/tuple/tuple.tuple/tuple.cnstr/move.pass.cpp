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

// tuple(tuple&& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <cassert>
#include <memory>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

struct ConstructsWithTupleLeaf
{
    ConstructsWithTupleLeaf() {}

    ConstructsWithTupleLeaf(ConstructsWithTupleLeaf const &) { TC_ASSERT_EXPR(false); }
    ConstructsWithTupleLeaf(ConstructsWithTupleLeaf &&) {}

    template <class T>
    ConstructsWithTupleLeaf(T) {
        static_assert(!std::is_same<T, T>::value,
                      "Constructor instantiated for type other than int");
    }
};

// move_only type which triggers the empty base optimization
struct move_only_ebo {
  move_only_ebo() = default;
  move_only_ebo(move_only_ebo&&) = default;
};

// a move_only type which does not trigger the empty base optimization
struct move_only_large final {
  move_only_large() : value(42) {}
  move_only_large(move_only_large&&) = default;
  int value;
};

template <class Elem>
void test_sfinae() {
    using Tup = std::tuple<Elem>;
    using Alloc = std::allocator<int>;
    using Tag = std::allocator_arg_t;
    // special members
    {
        static_assert(std::is_default_constructible<Tup>::value, "");
        static_assert(std::is_move_constructible<Tup>::value, "");
        static_assert(!std::is_copy_constructible<Tup>::value, "");
        static_assert(!std::is_constructible<Tup, Tup&>::value, "");
    }
    // args constructors
    {
        static_assert(std::is_constructible<Tup, Elem&&>::value, "");
        static_assert(!std::is_constructible<Tup, Elem const&>::value, "");
        static_assert(!std::is_constructible<Tup, Elem&>::value, "");
    }
    // uses-allocator special member constructors
    {
        static_assert(std::is_constructible<Tup, Tag, Alloc>::value, "");
        static_assert(std::is_constructible<Tup, Tag, Alloc, Tup&&>::value, "");
        static_assert(!std::is_constructible<Tup, Tag, Alloc, Tup const&>::value, "");
        static_assert(!std::is_constructible<Tup, Tag, Alloc, Tup &>::value, "");
    }
    // uses-allocator args constructors
    {
        static_assert(std::is_constructible<Tup, Tag, Alloc, Elem&&>::value, "");
        static_assert(!std::is_constructible<Tup, Tag, Alloc, Elem const&>::value, "");
        static_assert(!std::is_constructible<Tup, Tag, Alloc, Elem &>::value, "");
    }
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_move(void) {
    {
        typedef std::tuple<> T;
        T t0;
        T t = std::move(t0);
        ((void)t); // Prevent unused warning
    }
    {
        typedef std::tuple<MoveOnly> T;
        T t0(MoveOnly(0));
        T t = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1));
        T t = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == 1);
    }
    {
        typedef std::tuple<MoveOnly, MoveOnly, MoveOnly> T;
        T t0(MoveOnly(0), MoveOnly(1), MoveOnly(2));
        T t = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == 1);
        TC_ASSERT_EXPR(std::get<2>(t) == 2);
    }
    // A bug in tuple caused __tuple_leaf to use its explicit converting constructor
    //  as its move constructor. This tests that ConstructsWithTupleLeaf is not called
    // (w/ __tuple_leaf)
    {
        typedef std::tuple<ConstructsWithTupleLeaf> d_t;
        d_t d((ConstructsWithTupleLeaf()));
        d_t d2(static_cast<d_t &&>(d));
    }
    {
        test_sfinae<move_only_ebo>();
        test_sfinae<move_only_large>();
    }

  return 0;
}
