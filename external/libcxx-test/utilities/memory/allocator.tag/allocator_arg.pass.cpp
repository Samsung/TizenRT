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

// <memory>

// struct allocator_arg_t { explicit allocator_arg_t() = default; };
// const allocator_arg_t allocator_arg = allocator_arg_t();

#include <memory>

#include "test_macros.h"
#include "libcxx_tc_common.h"

void test(std::allocator_arg_t) {}

int tc_utilities_memory_allocator_tag_allocator_arg(void) {
    test(std::allocator_arg);

    return 0;
}
