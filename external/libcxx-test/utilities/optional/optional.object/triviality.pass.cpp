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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <optional>

// The following special member functions should propagate the triviality of
// the element held in the optional (see P0602R4):
//
// constexpr optional(const optional& rhs);
// constexpr optional(optional&& rhs) noexcept(see below);
// constexpr optional<T>& operator=(const optional& rhs);
// constexpr optional<T>& operator=(optional&& rhs) noexcept(see below);


#include <optional>
#include <type_traits>

#include "archetypes.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"


constexpr bool implies(bool p, bool q) {
    return !p || q;
}

template <class T>
struct SpecialMemberTest {
    using O = std::optional<T>;

    static_assert(implies(std::is_trivially_copy_constructible_v<T>,
                          std::is_trivially_copy_constructible_v<O>),
        "optional<T> is trivially copy constructible if T is trivially copy constructible.");

    static_assert(implies(std::is_trivially_move_constructible_v<T>,
                          std::is_trivially_move_constructible_v<O>),
        "optional<T> is trivially move constructible if T is trivially move constructible");

    static_assert(implies(std::is_trivially_copy_constructible_v<T> &&
                          std::is_trivially_copy_assignable_v<T> &&
                          std::is_trivially_destructible_v<T>,

                          std::is_trivially_copy_assignable_v<O>),
        "optional<T> is trivially copy assignable if T is "
        "trivially copy constructible, "
        "trivially copy assignable, and "
        "trivially destructible");

    static_assert(implies(std::is_trivially_move_constructible_v<T> &&
                          std::is_trivially_move_assignable_v<T> &&
                          std::is_trivially_destructible_v<T>,

                          std::is_trivially_move_assignable_v<O>),
        "optional<T> is trivially move assignable if T is "
        "trivially move constructible, "
        "trivially move assignable, and"
        "trivially destructible.");
};

template <class ...Args> static void sink(Args&&...) {}

template <class ...TestTypes>
struct DoTestsMetafunction {
    DoTestsMetafunction() { sink(SpecialMemberTest<TestTypes>{}...); }
};

struct TrivialMoveNonTrivialCopy {
    TrivialMoveNonTrivialCopy() = default;
    TrivialMoveNonTrivialCopy(const TrivialMoveNonTrivialCopy&) {}
    TrivialMoveNonTrivialCopy(TrivialMoveNonTrivialCopy&&) = default;
    TrivialMoveNonTrivialCopy& operator=(const TrivialMoveNonTrivialCopy&) { return *this; }
    TrivialMoveNonTrivialCopy& operator=(TrivialMoveNonTrivialCopy&&) = default;
};

struct TrivialCopyNonTrivialMove {
    TrivialCopyNonTrivialMove() = default;
    TrivialCopyNonTrivialMove(const TrivialCopyNonTrivialMove&) = default;
    TrivialCopyNonTrivialMove(TrivialCopyNonTrivialMove&&) {}
    TrivialCopyNonTrivialMove& operator=(const TrivialCopyNonTrivialMove&) = default;
    TrivialCopyNonTrivialMove& operator=(TrivialCopyNonTrivialMove&&) { return *this; }
};

int tc_utilities_optional_optional_object_triviality(void) {
    sink(
        ImplicitTypes::ApplyTypes<DoTestsMetafunction>{},
        ExplicitTypes::ApplyTypes<DoTestsMetafunction>{},
        NonLiteralTypes::ApplyTypes<DoTestsMetafunction>{},
        NonTrivialTypes::ApplyTypes<DoTestsMetafunction>{},
        DoTestsMetafunction<TrivialMoveNonTrivialCopy, TrivialCopyNonTrivialMove>{}
    );
    return 0;
}
