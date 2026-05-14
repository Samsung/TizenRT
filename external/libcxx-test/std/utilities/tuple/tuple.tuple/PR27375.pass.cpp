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
// Regression test for https://llvm.org/PR27375

// UNSUPPORTED: c++03

#include <tuple>
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_PR27375(void) {
    std::tuple<int&>(std::tuple<int&&>(42));

    return 0;
}
