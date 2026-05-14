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
// <vector>

#include <cassert>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {
  using CRefT = std::vector<bool>::const_reference;
#if !defined(_LIBCPP_VERSION) || defined(_LIBCPP_ABI_BITSET_VECTOR_BOOL_CONST_SUBSCRIPT_RETURN_BOOL)
  ASSERT_SAME_TYPE(CRefT, bool);
#else
  ASSERT_SAME_TYPE(CRefT, std::__bit_const_reference<std::vector<bool> >);
  std::vector<bool> vec;
  vec.push_back(true);
  CRefT ref = vec[0];
  TC_ASSERT_EXPR(ref);
  vec[0] = false;
  TC_ASSERT_EXPR(!ref);
#endif

  return true;
}

int tc_containers_sequences_vector_bool_const_reference(void) {
  test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
