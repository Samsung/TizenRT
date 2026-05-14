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
// <memory>

// class bad_weak_ptr
//     : public std::exception
// {
// public:
//     bad_weak_ptr();
// };

#include <exception>
#include <memory>
#include <type_traits>
#include <cassert>
#include <cstring>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_weakptr_bad_weak_ptr(void) {
    static_assert((std::is_base_of<std::exception, std::bad_weak_ptr>::value), "");
    std::bad_weak_ptr e;
    std::bad_weak_ptr e2 = e;
    e2 = e;
    TC_ASSERT_EXPR(std::strcmp(e.what(), "bad_weak_ptr") == 0);

  return 0;
}
