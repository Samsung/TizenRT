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
// <map>

// class value_compare

// REQUIRES: c++98 || c++03 || c++11 || c++14

#include <map>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_value_compare_types(void) {
    typedef std::map<int, std::string> map_type;
    typedef map_type::value_compare value_compare;
    typedef map_type::value_type value_type;

    ASSERT_SAME_TYPE(value_compare::result_type, bool);
    ASSERT_SAME_TYPE(value_compare::first_argument_type, value_type);
    ASSERT_SAME_TYPE(value_compare::second_argument_type, value_type);

    return 0;
}
