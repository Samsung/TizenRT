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
// default ctor

#include <bitset>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_default_ctor()
{
    {
        TEST_CONSTEXPR std::bitset<N> v1;
        TC_ASSERT_EXPR(v1.size() == N);
        for (std::size_t i = 0; i < v1.size(); ++i)
            TC_ASSERT_EXPR(v1[i] == false);
    }
#if TEST_STD_VER >= 11
    {
        constexpr std::bitset<N> v1;
        static_assert(v1.size() == N, "");
    }
#endif
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_default_ctor<0>();
  test_default_ctor<1>();
  test_default_ctor<31>();
  test_default_ctor<32>();
  test_default_ctor<33>();
  test_default_ctor<63>();
  test_default_ctor<64>();
  test_default_ctor<65>();
  test_default_ctor<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_cons_default(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
