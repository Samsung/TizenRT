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
//

// UNSUPPORTED: c++03

//  Tuples of smart pointers; based on bug #18350
//  auto_ptr doesn't have a copy constructor that takes a const &, but tuple does.

#include <tuple>
#include <memory>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_general_tuple_smartptr(void) {
    {
    std::tuple<std::unique_ptr<char>> up;
    std::tuple<std::shared_ptr<char>> sp;
    std::tuple<std::weak_ptr  <char>> wp;
    }
    {
    std::tuple<std::unique_ptr<char[]>> up;
    std::tuple<std::shared_ptr<char[]>> sp;
    std::tuple<std::weak_ptr  <char[]>> wp;
    }
    // Smart pointers of type 'T[N]' are not tested here since they are not
    // supported by the standard nor by libc++'s implementation.
    // See https://reviews.llvm.org/D21320 for more information.

  return 0;
}
