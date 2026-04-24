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

// decay

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_decay()
{
    ASSERT_SAME_TYPE(U, typename std::decay<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U,        std::decay_t<T>);
#endif
}

int tc_utilities_meta_meta_trans_meta_trans_other_decay(void) {
    test_decay<void, void>();
    test_decay<int, int>();
    test_decay<const volatile int, int>();
    test_decay<int*, int*>();
    test_decay<int&, int>();
    test_decay<const volatile int&, int>();
    test_decay<int[3], int*>();
    test_decay<const int[3], const int*>();
    test_decay<void(), void (*)()>();
#if TEST_STD_VER > 11
    test_decay<int&&, int>();
    test_decay<const volatile int&&, int>();
    test_decay<int(int) const, int(int) const>();
    test_decay<int(int) volatile, int(int) volatile>();
    test_decay<int(int)  &, int(int)  &>();
    test_decay<int(int) &&, int(int) &&>();
#endif

  return 0;
}
