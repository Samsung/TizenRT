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
// UNSUPPORTED: c++03

// <queue>

// queue()
//        noexcept(is_nothrow_default_constructible<container_type>::value);

// This tests a conforming extension

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_cons_default_noexcept(void) {
#if defined(_LIBCPP_VERSION)
    {
        typedef std::queue<MoveOnly> C;
        static_assert(std::is_nothrow_default_constructible<C>::value, "");
    }
#endif

  return 0;
}
