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

// <functional>

// Test that std::compare_three_way is defined in <functional>,
// not only in <compare>.

#include <functional>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_comparisons_compare_three_way_functional(void) {
    TC_ASSERT_EXPR(std::compare_three_way()(1, 2) < 0);
    TC_ASSERT_EXPR(std::compare_three_way()(1, 1) == 0);
    TC_ASSERT_EXPR(std::compare_three_way()(2, 1) > 0);

    return 0;
}
