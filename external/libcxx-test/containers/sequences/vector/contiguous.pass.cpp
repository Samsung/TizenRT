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

// An vector is a contiguous container

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void test_contiguous(const C &c)
{
    for ( std::size_t i = 0; i < c.size(); ++i )
        TC_ASSERT_EXPR( *(c.begin() + static_cast<typename C::difference_type>(i)) == *(std::addressof(*c.begin()) + i));
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
    typedef int T;
    typedef std::vector<T> C;
    test_contiguous(C());
    test_contiguous(C(3, 5));
    }

    {
    typedef double T;
    typedef test_allocator<T> A;
    typedef std::vector<T, A> C;
    test_contiguous(C(A(3)));
    test_contiguous(C(7, 9.0, A(5)));
    }
#if TEST_STD_VER >= 11
    {
    typedef double T;
    typedef min_allocator<T> A;
    typedef std::vector<T, A> C;
    test_contiguous(C(A{}));
    test_contiguous(C(9, 11.0, A{}));
    }
    {
      typedef double T;
      typedef safe_allocator<T> A;
      typedef std::vector<T, A> C;
      test_contiguous(C(A{}));
      test_contiguous(C(9, 11.0, A{}));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_contiguous(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
