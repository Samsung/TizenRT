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

// <map>

// class map

// map();

#include <map>
#include "libcxx_tc_common.h"
struct X
{
    std::map<int, X> m;
    std::map<int, X>::iterator i;
    std::map<int, X>::const_iterator ci;
    std::map<int, X>::reverse_iterator ri;
    std::map<int, X>::const_reverse_iterator cri;
};

int tc_libcxx_containers_map_cons_default_recursive(void)
{
    TC_SUCCESS_RESULT();
    return 0;
}
