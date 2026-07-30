//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// reference(const reference&)

#include <cassert>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {
  std::vector<bool> vec;
  typedef std::vector<bool>::reference Ref;
  vec.push_back(true);
  Ref ref = vec[0];
  Ref ref2 = ref;
  TC_ASSERT_EXPR(ref == ref2 && ref2);
  ref.flip();
  TC_ASSERT_EXPR(ref == ref2 && !ref2);

  return true;
}

int tc_containers_sequences_vector_bool_reference_ctor_copy(void) {
  test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
