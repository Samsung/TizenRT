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
// type_traits

// integral_constant

#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_meta_meta_help_integral_constant(void) {
    typedef std::integral_constant<int, 5> _5;
    static_assert(_5::value == 5, "");
    static_assert((std::is_same<_5::value_type, int>::value), "");
    static_assert((std::is_same<_5::type, _5>::value), "");
#if TEST_STD_VER >= 11
    static_assert((_5() == 5), "");
#endif
    TC_ASSERT_EXPR(_5() == 5);


#if TEST_STD_VER > 11
    static_assert ( _5{}() == 5, "" );
    static_assert ( std::true_type{}(), "" );
#endif

    static_assert(std::false_type::value == false, "");
    static_assert((std::is_same<std::false_type::value_type, bool>::value), "");
    static_assert((std::is_same<std::false_type::type, std::false_type>::value), "");

    static_assert(std::true_type::value == true, "");
    static_assert((std::is_same<std::true_type::value_type, bool>::value), "");
    static_assert((std::is_same<std::true_type::type, std::true_type>::value), "");

    std::false_type f1;
    std::false_type f2 = f1;
    TC_ASSERT_EXPR(!f2);

    std::true_type t1;
    std::true_type t2 = t1;
    TC_ASSERT_EXPR(t2);

  return 0;
}
