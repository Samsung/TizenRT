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

// template <size_t I, class... Types>
//   typename tuple_element<I, tuple<Types...> >::type&&
//   get(tuple<Types...>&& t);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_elem_get_rv(void) {
    {
        typedef std::tuple<std::unique_ptr<int> > T;
        T t(std::unique_ptr<int>(new int(3)));
        std::unique_ptr<int> p = std::get<0>(std::move(t));
        TC_ASSERT_EXPR(*p == 3);
    }

  return 0;
}
