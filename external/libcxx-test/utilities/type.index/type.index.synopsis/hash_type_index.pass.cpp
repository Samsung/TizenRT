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
// <typeindex>

// struct hash<type_index>
// {
//     size_t operator()(type_index index) const;
// };

// UNSUPPORTED: no-rtti

#include <typeindex>
#include <type_traits>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "poisoned_hash_helper.h"
#include "libcxx_tc_common.h"
#endif

int tc_utilities_type_index_type_index_synopsis_hash_type_index(void) {
  {
#if TEST_STD_VER <= 14
    typedef std::hash<std::type_index> H;
    static_assert((std::is_same<typename H::argument_type, std::type_index>::value), "" );
    static_assert((std::is_same<typename H::result_type, std::size_t>::value), "" );
#endif
  }
#if TEST_STD_VER >= 11
  {
    test_hash_enabled_for_type<std::type_index>(std::type_index(typeid(int)));
  }
#endif

  return 0;
}
