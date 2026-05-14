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
// <memory>

// shared_ptr(nullptr_t)

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t(void) {
  {
    std::shared_ptr<int> p(nullptr);
    TC_ASSERT_EXPR(p.use_count() == 0);
    TC_ASSERT_EXPR(p.get() == 0);
  }

  {
    std::shared_ptr<int const> p(nullptr);
    TC_ASSERT_EXPR(p.use_count() == 0);
    TC_ASSERT_EXPR(p.get() == 0);
  }

  return 0;
}
