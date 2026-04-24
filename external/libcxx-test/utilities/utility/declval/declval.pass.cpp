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
// <utility>

// template <class T> typename add_rvalue_reference<T>::type declval() noexcept;

#include <utility>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    A(const A&);
    A& operator=(const A&);
};

int tc_utilities_utility_declval_declval(void) {
    static_assert((std::is_same<decltype(std::declval<A>()), A&&>::value), "");

  return 0;
}
