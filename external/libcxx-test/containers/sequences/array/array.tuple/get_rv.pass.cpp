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
// <array>

// template <size_t I, class T, size_t N> T&& get(array<T, N>&& a);

// UNSUPPORTED: c++03

#include <array>
#include <memory>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_array_array_tuple_get_rv(void) {
    {
        typedef std::unique_ptr<double> T;
        typedef std::array<T, 1> C;
        C c = {std::unique_ptr<double>(new double(3.5))};
        T t = std::get<0>(std::move(c));
        TC_ASSERT_EXPR(*t == 3.5);
    }

    return 0;
}
