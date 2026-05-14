//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
