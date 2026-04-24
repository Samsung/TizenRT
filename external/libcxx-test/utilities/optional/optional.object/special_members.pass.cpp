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

// <optional>

// Make sure we properly generate special member functions for optional<T>
// based on the properties of T itself.

#include <optional>
#include <type_traits>

#include "archetypes.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class T>
struct SpecialMemberTest {
    using O = std::optional<T>;

    static_assert(std::is_default_constructible_v<O>,
        "optional is always default constructible.");

    static_assert(std::is_copy_constructible_v<O> == std::is_copy_constructible_v<T>,
        "optional<T> is copy constructible if and only if T is copy constructible.");

    static_assert(std::is_move_constructible_v<O> ==
        (std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>),
        "optional<T> is move constructible if and only if T is copy or move constructible.");

    static_assert(std::is_copy_assignable_v<O> ==
        (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>),
        "optional<T> is copy assignable if and only if T is both copy "
        "constructible and copy assignable.");

    static_assert(std::is_move_assignable_v<O> ==
        ((std::is_move_constructible_v<T> && std::is_move_assignable_v<T>) ||
         (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>)),
        "optional<T> is move assignable if and only if T is both move constructible and "
        "move assignable, or both copy constructible and copy assignable.");
};

template <class ...Args> static void sink(Args&&...) {}

template <class ...TestTypes>
struct DoTestsMetafunction {
    DoTestsMetafunction() { sink(SpecialMemberTest<TestTypes>{}...); }
};

int tc_utilities_optional_optional_object_special_members(void) {
    sink(
        ImplicitTypes::ApplyTypes<DoTestsMetafunction>{},
        ExplicitTypes::ApplyTypes<DoTestsMetafunction>{},
        NonLiteralTypes::ApplyTypes<DoTestsMetafunction>{},
        NonTrivialTypes::ApplyTypes<DoTestsMetafunction>{}
    );
    return 0;
}
