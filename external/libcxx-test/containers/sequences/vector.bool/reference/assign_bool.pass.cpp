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

// reference& operator=(bool)

#include <cassert>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {
  std::vector<bool> vec;
  typedef std::vector<bool>::reference Ref;
  vec.push_back(true);
  vec.push_back(false);
  Ref ref = vec[0];
  const Ref cref = vec[0];

  TC_ASSERT_EXPR(ref);
  ref = false;
  TC_ASSERT_EXPR(!vec[0]);
  TC_ASSERT_EXPR(!vec[1]);
  ref = true;
  TC_ASSERT_EXPR(vec[0]);
  TC_ASSERT_EXPR(!vec[1]);

  TC_ASSERT_EXPR(cref);

#if TEST_STD_VER > 20
  cref = false;
  TC_ASSERT_EXPR(!vec[0]);
  TC_ASSERT_EXPR(!vec[1]);
  cref = true;
  TC_ASSERT_EXPR(vec[0]);
  TC_ASSERT_EXPR(!vec[1]);
#endif

  return true;
}

int tc_containers_sequences_vector_bool_reference_assign_bool(void) {
  test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
