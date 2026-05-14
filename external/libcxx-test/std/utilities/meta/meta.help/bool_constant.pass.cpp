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

// bool_constant

#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_meta_meta_help_bool_constant(void) {
#if TEST_STD_VER > 14
    typedef std::bool_constant<true> _t;
    static_assert(_t::value, "");
    static_assert((std::is_same<_t::value_type, bool>::value), "");
    static_assert((std::is_same<_t::type, _t>::value), "");
    static_assert((_t() == true), "");

    typedef std::bool_constant<false> _f;
    static_assert(!_f::value, "");
    static_assert((std::is_same<_f::value_type, bool>::value), "");
    static_assert((std::is_same<_f::type, _f>::value), "");
    static_assert((_f() == false), "");
#endif

  return 0;
}
