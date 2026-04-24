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

// vector& operator=(const vector& c);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        std::vector<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        std::vector<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        std::vector<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        std::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<int>(5));
    }
#if TEST_STD_VER >= 11
    {
        // Test with Allocator::propagate_on_container_copy_assignment == false_type
        using Alloc = NonPOCCAAllocator<int>;
        bool copy_assigned_into = false;
        std::vector<int, Alloc> l(3, 2, Alloc(5, nullptr));
        std::vector<int, Alloc> l2(l, Alloc(3, &copy_assigned_into));
        TC_ASSERT_EXPR(!copy_assigned_into);
        l2 = l;
        TC_ASSERT_EXPR(!copy_assigned_into);
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == Alloc(3, nullptr));
    }
    {
        // Test with Allocator::propagate_on_container_copy_assignment == true_type
        // and equal allocators
        using Alloc = POCCAAllocator<int>;
        bool copy_assigned_into = false;
        std::vector<int, Alloc> l(3, 2, Alloc(5, nullptr));
        std::vector<int, Alloc> l2(l, Alloc(5, &copy_assigned_into));
        TC_ASSERT_EXPR(!copy_assigned_into);
        l2 = l;
        TC_ASSERT_EXPR(copy_assigned_into);
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == Alloc(5, nullptr));
    }
    {
        // Test with Allocator::propagate_on_container_copy_assignment == true_type
        // and unequal allocators
        using Alloc = POCCAAllocator<int>;
        bool copy_assigned_into = false;
        std::vector<int, Alloc> l(3, 2, Alloc(5, nullptr));
        std::vector<int, Alloc> l2(l, Alloc(3, &copy_assigned_into));
        TC_ASSERT_EXPR(!copy_assigned_into);
        l2 = l;
        TC_ASSERT_EXPR(copy_assigned_into);
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == Alloc(5, nullptr));
    }
    {
        std::vector<int, min_allocator<int> > l(3, 2, min_allocator<int>());
        std::vector<int, min_allocator<int> > l2(l, min_allocator<int>());
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == min_allocator<int>());
    }
    {
      std::vector<int, safe_allocator<int> > l(3, 2, safe_allocator<int>());
      std::vector<int, safe_allocator<int> > l2(l, safe_allocator<int>());
      l2 = l;
      TC_ASSERT_EXPR(l2 == l);
      TC_ASSERT_EXPR(l2.get_allocator() == safe_allocator<int>());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_cons_assign_copy(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
