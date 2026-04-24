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

// operator bool()

#include <cassert>
#include <type_traits>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {
  std::vector<bool> vec;
  typedef std::vector<bool>::reference Ref;
  static_assert(std::is_convertible<Ref, bool>::value, "");

  vec.push_back(true);
  vec.push_back(false);
  Ref true_ref = vec[0];
  Ref false_ref = vec[1];
  bool b = true_ref;
  TC_ASSERT_EXPR(b);
  TC_ASSERT_EXPR(true_ref);
  TC_ASSERT_EXPR(!false_ref);

  return true;
}

int tc_containers_sequences_vector_bool_reference_operator_bool(void) {
  test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
