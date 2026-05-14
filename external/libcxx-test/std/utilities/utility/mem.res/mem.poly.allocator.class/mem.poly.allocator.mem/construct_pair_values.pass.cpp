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
// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

// <memory_resource>

// template <class T> class polymorphic_allocator

// template <class P1, class P2, class U1, class U2>
// void polymorphic_allocator<T>::construct(pair<P1, P2>*, U1&&, U2&&)

#include <memory_resource>
#include <type_traits>
#include <utility>
#include <tuple>
#include <cassert>
#include <cstdlib>

#include "test_macros.h"
#include "test_std_memory_resource.h"
#include "uses_alloc_types.h"
#include "controlled_allocators.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

template <class UA1, class UA2, class TT, class UU>
bool doTest(UsesAllocatorType TExpect, UsesAllocatorType UExpect, TT&& t, UU&& u) {
  using P = std::pair<UA1, UA2>;
  TestResource R;
  std::pmr::memory_resource* M = &R;
  std::pmr::polymorphic_allocator<P> A(M);
  P* ptr  = (P*)std::malloc(sizeof(P));
  P* ptr2 = (P*)std::malloc(sizeof(P));

  // UNDER TEST //
  A.construct(ptr, std::forward<TT>(t), std::forward<UU>(u));
  A.construct(ptr2,
              std::piecewise_construct,
              std::forward_as_tuple(std::forward<TT>(t)),
              std::forward_as_tuple(std::forward<UU>(u)));
  // ------- //

  bool tres = checkConstruct<TT&&>(ptr->first, TExpect, M) && checkConstructionEquiv(ptr->first, ptr2->first);

  bool ures = checkConstruct<UU&&>(ptr->second, UExpect, M) && checkConstructionEquiv(ptr->second, ptr2->second);

  A.destroy(ptr);
  A.destroy(ptr2);
  std::free(ptr);
  std::free(ptr2);
  return tres && ures;
}

template <class Alloc, class TT, class UU>
void test_pmr_uses_allocator(TT&& t, UU&& u) {
  {
    using T = NotUsesAllocator<Alloc, 1>;
    using U = NotUsesAllocator<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV1<Alloc, 1>;
    using U = UsesAllocatorV2<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocArg, UA_AllocLast, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV2<Alloc, 1>;
    using U = UsesAllocatorV3<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocLast, UA_AllocArg, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV3<Alloc, 1>;
    using U = NotUsesAllocator<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocArg, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
}

template <class Alloc, class TT, class UU>
void test_pmr_not_uses_allocator(TT&& t, UU&& u) {
  {
    using T = NotUsesAllocator<Alloc, 1>;
    using U = NotUsesAllocator<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV1<Alloc, 1>;
    using U = UsesAllocatorV2<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV2<Alloc, 1>;
    using U = UsesAllocatorV3<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
  {
    using T = UsesAllocatorV3<Alloc, 1>;
    using U = NotUsesAllocator<Alloc, 1>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::forward<TT>(t), std::forward<UU>(u))));
  }
}

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_pair_values(void) {
  using PMR = std::pmr::memory_resource*;
  using PMA = std::pmr::polymorphic_allocator<char>;
  {
    int x = 42;
    int y = 42;
    test_pmr_not_uses_allocator<PMR>(x, std::move(y));
    test_pmr_uses_allocator<PMA>(x, std::move(y));
  }
  {
    int x       = 42;
    const int y = 42;
    test_pmr_not_uses_allocator<PMR>(std::move(x), y);
    test_pmr_uses_allocator<PMA>(std::move(x), y);
  }

  return 0;
}
