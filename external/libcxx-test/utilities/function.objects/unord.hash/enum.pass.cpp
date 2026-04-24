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
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <functional>

// Make sure that we can hash enumeration values.

#include "test_macros.h"

#include <functional>
#include <cassert>
#include <type_traits>
#include <limits>
#include "libcxx_tc_common.h"

enum class Colors { red, orange, yellow, green, blue, indigo, violet };
enum class Cardinals { zero, one, two, three, five=5 };
enum class LongColors : short { red, orange, yellow, green, blue, indigo, violet };
enum class ShortColors : long { red, orange, yellow, green, blue, indigo, violet };
enum class EightBitColors : std::uint8_t { red, orange, yellow, green, blue, indigo, violet };

enum Fruits { apple, pear, grape, mango, cantaloupe };

template <class T>
void
test()
{
    typedef std::hash<T> H;
#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename H::argument_type, T>::value), "");
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "");
#endif
    ASSERT_NOEXCEPT(H()(T()));
    typedef typename std::underlying_type<T>::type under_type;

    H h1;
    std::hash<under_type> h2;
    for (int i = 0; i <= 5; ++i)
    {
        T t(static_cast<T> (i));
        const bool small = std::integral_constant<bool, sizeof(T) <= sizeof(std::size_t)>::value; // avoid compiler warnings
        if (small)
            TC_ASSERT_EXPR(h1(t) == h2(static_cast<under_type>(i)));
    }
}

int tc_utilities_function_objects_unord_hash_enum(void) {
    test<Cardinals>();

    test<Colors>();
    test<ShortColors>();
    test<LongColors>();
    test<EightBitColors>();

    test<Fruits>();

  return 0;
}
