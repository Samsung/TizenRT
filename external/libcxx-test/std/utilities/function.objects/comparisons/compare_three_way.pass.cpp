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

// <compare>
// <functional>

// compare_three_way

#include <compare>
#include <cassert>
#include <limits>
#include <type_traits>

#include "pointer_comparison_test_helper.h"
#include "libcxx_tc_common.h"

template<class T, class U>
constexpr auto test_sfinae(T t, U u)
    -> decltype(std::compare_three_way()(t, u), std::true_type{})
    { return std::true_type{}; }

constexpr auto test_sfinae(...)
    { return std::false_type{}; }

struct NotThreeWayComparable {
    std::strong_ordering operator<=>(const NotThreeWayComparable&) const;
};
ASSERT_SAME_TYPE(std::compare_three_way_result_t<NotThreeWayComparable>, std::strong_ordering);
static_assert(!std::three_way_comparable<NotThreeWayComparable>);  // it lacks operator==

struct WeaklyOrdered {
    int i;
    friend constexpr std::weak_ordering operator<=>(const WeaklyOrdered&, const WeaklyOrdered&) = default;
};

constexpr bool test()
{
    ASSERT_SAME_TYPE(decltype(std::compare_three_way()(1, 1)), std::strong_ordering);
    TC_ASSERT_EXPR(std::compare_three_way()(1, 2) == std::strong_ordering::less);
    TC_ASSERT_EXPR(std::compare_three_way()(1, 1) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(std::compare_three_way()(2, 1) == std::strong_ordering::greater);

    ASSERT_SAME_TYPE(decltype(std::compare_three_way()(WeaklyOrdered{1}, WeaklyOrdered{2})), std::weak_ordering);
    TC_ASSERT_EXPR(std::compare_three_way()(WeaklyOrdered{1}, WeaklyOrdered{2}) == std::weak_ordering::less);
    TC_ASSERT_EXPR(std::compare_three_way()(WeaklyOrdered{1}, WeaklyOrdered{1}) == std::weak_ordering::equivalent);
    TC_ASSERT_EXPR(std::compare_three_way()(WeaklyOrdered{2}, WeaklyOrdered{1}) == std::weak_ordering::greater);

    ASSERT_SAME_TYPE(decltype(std::compare_three_way()(1.0, 1.0)), std::partial_ordering);
    double nan = std::numeric_limits<double>::quiet_NaN();
    TC_ASSERT_EXPR(std::compare_three_way()(1.0, 2.0) == std::partial_ordering::less);
    TC_ASSERT_EXPR(std::compare_three_way()(1.0, 1.0) == std::partial_ordering::equivalent);
    TC_ASSERT_EXPR(std::compare_three_way()(2.0, 1.0) == std::partial_ordering::greater);
    TC_ASSERT_EXPR(std::compare_three_way()(nan, nan) == std::partial_ordering::unordered);

    // Try heterogeneous comparison.
    ASSERT_SAME_TYPE(decltype(std::compare_three_way()(42.0, 42)), std::partial_ordering);
    TC_ASSERT_EXPR(std::compare_three_way()(42.0, 42) == std::partial_ordering::equivalent);
    ASSERT_SAME_TYPE(decltype(std::compare_three_way()(42, 42.0)), std::partial_ordering);
    TC_ASSERT_EXPR(std::compare_three_way()(42, 42.0) == std::partial_ordering::equivalent);

    return true;
}

int tc_utilities_function_objects_comparisons_compare_three_way(void) {
    test();
    static_assert(test());

    do_pointer_comparison_test(std::compare_three_way());

    static_assert(test_sfinae(1, 2));
    static_assert(!test_sfinae(1, nullptr));
    static_assert(!test_sfinae(NotThreeWayComparable(), NotThreeWayComparable()));

    return 0;
}
