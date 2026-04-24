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
// <queue>

// void swap(priority_queue& c)
//     noexcept(__is_nothrow_swappable<container_type>::value &&
//              __is_nothrow_swappable<Compare>::value);

// This tests a conforming extension

// UNSUPPORTED: c++03

#include <queue>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_special_swap_noexcept(void) {
    {
        typedef std::priority_queue<MoveOnly> C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }

  return 0;
}
