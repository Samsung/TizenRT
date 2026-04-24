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

// template <class T>
// struct hash
//     : public unary_function<T, size_t>
// {
//     size_t operator()(T val) const;
// };

// Not very portable

#include <functional>
#include <cassert>
#include <type_traits>
#include <limits>
#include <cmath>

#include "test_macros.h"
#include "libcxx_tc_common.h"

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
    H h;

    std::size_t t0 = h(0.);
    std::size_t tn0 = h(-0.);
    std::size_t tp1 = h(static_cast<T>(0.1));
    std::size_t t1 = h(1);
    std::size_t tn1 = h(-1);
    std::size_t pinf = h(INFINITY);
    std::size_t ninf = h(-INFINITY);
    TC_ASSERT_EXPR(t0 == tn0);
    TC_ASSERT_EXPR(t0 != tp1);
    TC_ASSERT_EXPR(t0 != t1);
    TC_ASSERT_EXPR(t0 != tn1);
    TC_ASSERT_EXPR(t0 != pinf);
    TC_ASSERT_EXPR(t0 != ninf);

    TC_ASSERT_EXPR(tp1 != t1);
    TC_ASSERT_EXPR(tp1 != tn1);
    TC_ASSERT_EXPR(tp1 != pinf);
    TC_ASSERT_EXPR(tp1 != ninf);

    TC_ASSERT_EXPR(t1 != tn1);
    TC_ASSERT_EXPR(t1 != pinf);
    TC_ASSERT_EXPR(t1 != ninf);

    TC_ASSERT_EXPR(tn1 != pinf);
    TC_ASSERT_EXPR(tn1 != ninf);

    TC_ASSERT_EXPR(pinf != ninf);
}

int tc_utilities_function_objects_unord_hash_floating(void) {
    test<float>();
    test<double>();
    test<long double>();

  return 0;
}
