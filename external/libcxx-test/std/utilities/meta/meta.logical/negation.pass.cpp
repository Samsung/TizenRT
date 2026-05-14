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
// type_traits

// template<class B> struct negation;                        // C++17
// template<class B>
//   constexpr bool negation_v = negation<B>::value;         // C++17

#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct True  { static constexpr bool value = true; };
struct False { static constexpr bool value = false; };

int tc_utilities_meta_meta_logical_negation(void) {
    static_assert (!std::negation<std::true_type >::value, "" );
    static_assert ( std::negation<std::false_type>::value, "" );

    static_assert (!std::negation_v<std::true_type >, "" );
    static_assert ( std::negation_v<std::false_type>, "" );

    static_assert (!std::negation<True >::value, "" );
    static_assert ( std::negation<False>::value, "" );

    static_assert (!std::negation_v<True >, "" );
    static_assert ( std::negation_v<False>, "" );

    static_assert ( std::negation<std::negation<std::true_type >>::value, "" );
    static_assert (!std::negation<std::negation<std::false_type>>::value, "" );

  return 0;
}
