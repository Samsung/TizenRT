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

// template <class... Types, class Alloc>
//   struct uses_allocator<tuple<Types...>, Alloc> : true_type { };

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};

int tc_utilities_tuple_tuple_tuple_tuple_traits_uses_allocator(void) {
    {
        typedef std::tuple<> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<char, int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }
    {
        typedef std::tuple<double&, char, int> T;
        static_assert((std::is_base_of<std::true_type,
                                       std::uses_allocator<T, A>>::value), "");
    }

  return 0;
}
