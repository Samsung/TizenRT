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
// <map>

// class multimap

// multimap();

#include <map>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct X
{
    std::multimap<int, X> m;
    std::multimap<int, X>::iterator i;
    std::multimap<int, X>::const_iterator ci;
#if TEST_STD_VER <= 17
    // These reverse_iterator specializations require X to be complete in C++20.
    std::multimap<int, X>::reverse_iterator ri;
    std::multimap<int, X>::const_reverse_iterator cri;
#endif // TEST_STD_VER <= 17
};

TC_SUCCESS_RESULT();
int tc_containers_associative_multimap_multimap_cons_default_recursive(void) { return 0; }
