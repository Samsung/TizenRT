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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <list>

// static int push_front(value_type&& x);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_list_modifiers_push_front_rvalue(void)
{
    {
    std::list<MoveOnly> l1;
    l1.push_front(MoveOnly(1));
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    l1.push_front(MoveOnly(2));
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(2));
    TC_ASSERT_EXPR(l1.back() == MoveOnly(1));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
