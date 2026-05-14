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
// UNSUPPORTED: c++03, c++11
// <utility>

// class make_integer_sequence

#include <tuple>
#include <utility>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename AtContainer, typename T, T... I>
auto extract ( const AtContainer &t, const std::integer_sequence<T, I...> )
-> decltype ( std::make_tuple ( std::get<I>(t)... ))
{     return  std::make_tuple ( std::get<I>(t)... ); }

int tc_utilities_intseq_intseq_general_integer_seq(void) {
//  Make a couple of sequences
    using int3    = std::make_integer_sequence<int, 3>;     // generates int:    0,1,2
    using size7   = std::make_integer_sequence<std::size_t, 7>;  // generates size_t: 0,1,2,3,4,5,6
    using size4   = std::make_index_sequence<4>;            // generates size_t: 0,1,2,3
    using size2   = std::index_sequence_for<int, std::size_t>;   // generates size_t: 0,1
    using intmix  = std::integer_sequence<int, 9, 8, 7, 2>; // generates int:    9,8,7,2
    using sizemix = std::index_sequence<1, 1, 2, 3, 5>;     // generates size_t: 1,1,2,3,5

//  Make sure they're what we expect
    static_assert ( std::is_same<int3::value_type, int>::value, "int3 type wrong" );
    static_assert ( int3::size () == 3, "int3 size wrong" );

    static_assert ( std::is_same<size7::value_type, std::size_t>::value, "size7 type wrong" );
    static_assert ( size7::size () == 7, "size7 size wrong" );

    static_assert ( std::is_same<size4::value_type, std::size_t>::value, "size4 type wrong" );
    static_assert ( size4::size () == 4, "size4 size wrong" );

    static_assert ( std::is_same<size2::value_type, std::size_t>::value, "size2 type wrong" );
    static_assert ( size2::size () == 2, "size2 size wrong" );

    static_assert ( std::is_same<intmix::value_type, int>::value, "intmix type wrong" );
    static_assert ( intmix::size () == 4, "intmix size wrong" );

    static_assert ( std::is_same<sizemix::value_type, std::size_t>::value, "sizemix type wrong" );
    static_assert ( sizemix::size () == 5, "sizemix size wrong" );

    auto tup = std::make_tuple ( 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 );

//  Use them
    auto t3 = extract ( tup, int3() );
    static_assert ( std::tuple_size<decltype(t3)>::value == int3::size (), "t3 size wrong");
    TC_ASSERT_EXPR( t3 == std::make_tuple ( 10, 11, 12 ));

    auto t7 = extract ( tup, size7 ());
    static_assert ( std::tuple_size<decltype(t7)>::value == size7::size (), "t7 size wrong");
    TC_ASSERT_EXPR( t7 == std::make_tuple ( 10, 11, 12, 13, 14, 15, 16 ));

    auto t4 = extract ( tup, size4 ());
    static_assert ( std::tuple_size<decltype(t4)>::value == size4::size (), "t4 size wrong");
    TC_ASSERT_EXPR( t4 == std::make_tuple ( 10, 11, 12, 13 ));

    auto t2 = extract ( tup, size2 ());
    static_assert ( std::tuple_size<decltype(t2)>::value == size2::size (), "t2 size wrong");
    TC_ASSERT_EXPR( t2 == std::make_tuple ( 10, 11 ));

    auto tintmix = extract ( tup, intmix ());
    static_assert ( std::tuple_size<decltype(tintmix)>::value == intmix::size (), "tintmix size wrong");
    TC_ASSERT_EXPR( tintmix == std::make_tuple ( 19, 18, 17, 12 ));

    auto tsizemix = extract ( tup, sizemix ());
    static_assert ( std::tuple_size<decltype(tsizemix)>::value == sizemix::size (), "tsizemix size wrong");
    TC_ASSERT_EXPR( tsizemix == std::make_tuple ( 11, 11, 12, 13, 15 ));

  return 0;
}
