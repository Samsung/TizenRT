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
// type_traits

// enable_if

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_meta_meta_trans_meta_trans_other_enable_if(void) {
    ASSERT_SAME_TYPE(void, std::enable_if<true>::type);
    ASSERT_SAME_TYPE(int,  std::enable_if<true, int>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(void, std::enable_if_t<true, void>);
    ASSERT_SAME_TYPE(int,  std::enable_if_t<true, int>);
#endif

  return 0;
}
