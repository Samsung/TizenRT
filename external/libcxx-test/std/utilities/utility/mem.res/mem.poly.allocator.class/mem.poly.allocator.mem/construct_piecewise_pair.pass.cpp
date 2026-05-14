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

// template <class U1, class U2, class ...Args1, class ...Args2>
// void polymorphic_allocator<T>::construct(pair<U1, U2>*, piecewise_construct_t
//                                          tuple<Args1...>, tuple<Args2...>)

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

template <class T, class U, class... TTuple, class... UTuple>
bool doTest(
    UsesAllocatorType TExpect, UsesAllocatorType UExpect, std::tuple<TTuple...> ttuple, std::tuple<UTuple...> utuple) {
  using P = std::pair<T, U>;
  TestResource R;
  std::pmr::memory_resource* M = &R;
  std::pmr::polymorphic_allocator<P> A(M);
  P* ptr = A.allocate(1);

  // UNDER TEST //
  A.construct(ptr, std::piecewise_construct, std::move(ttuple), std::move(utuple));
  // ------- //
  bool tres = checkConstruct<TTuple&&...>(ptr->first, TExpect, M);
  bool ures = checkConstruct<UTuple&&...>(ptr->second, UExpect, M);

  A.destroy(ptr);
  A.deallocate(ptr, 1);
  return tres && ures;
}

template <class Alloc, class... TTypes, class... UTypes>
void test_pmr_uses_allocator(std::tuple<TTypes...> ttuple, std::tuple<UTypes...> utuple) {
  {
    using T = NotUsesAllocator<Alloc, sizeof...(TTypes)>;
    using U = NotUsesAllocator<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV1<Alloc, sizeof...(TTypes)>;
    using U = UsesAllocatorV2<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocArg, UA_AllocLast, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV2<Alloc, sizeof...(TTypes)>;
    using U = UsesAllocatorV3<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocLast, UA_AllocArg, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV3<Alloc, sizeof...(TTypes)>;
    using U = NotUsesAllocator<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_AllocArg, UA_None, std::move(ttuple), std::move(utuple))));
  }
}

template <class Alloc, class... TTypes, class... UTypes>
void test_pmr_not_uses_allocator(std::tuple<TTypes...> ttuple, std::tuple<UTypes...> utuple) {
  {
    using T = NotUsesAllocator<Alloc, sizeof...(TTypes)>;
    using U = NotUsesAllocator<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV1<Alloc, sizeof...(TTypes)>;
    using U = UsesAllocatorV2<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV2<Alloc, sizeof...(TTypes)>;
    using U = UsesAllocatorV3<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::move(ttuple), std::move(utuple))));
  }
  {
    using T = UsesAllocatorV3<Alloc, sizeof...(TTypes)>;
    using U = NotUsesAllocator<Alloc, sizeof...(UTypes)>;
    TC_ASSERT_EXPR((doTest<T, U>(UA_None, UA_None, std::move(ttuple), std::move(utuple))));
  }
}

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_piecewise_pair(void) {
  using PMR = std::pmr::memory_resource*;
  using PMA = std::pmr::polymorphic_allocator<char>;
  {
    std::tuple<> t1;
    test_pmr_not_uses_allocator<PMR>(t1, t1);
    test_pmr_uses_allocator<PMA>(t1, t1);
  }
  {
    std::tuple<int> t1(42);
    std::tuple<> t2;
    test_pmr_not_uses_allocator<PMR>(t1, t2);
    test_pmr_not_uses_allocator<PMR>(t2, t1);
    test_pmr_uses_allocator<PMA>(t1, t2);
    test_pmr_uses_allocator<PMA>(t2, t1);
  }
  {
    std::tuple<int> t1(42);
    int x     = 55;
    double dx = 42.42;
    std::tuple<int&, double&&> t2(x, std::move(dx));
    test_pmr_not_uses_allocator<PMR>(t1, std::move(t2));
    test_pmr_not_uses_allocator<PMR>(std::move(t2), t1);
    test_pmr_uses_allocator<PMA>(t1, std::move(t2));
    test_pmr_uses_allocator<PMA>(std::move(t2), t1);
  }
  {
    void* xptr = nullptr;
    long y     = 4242;
    std::tuple<int, long const&, void*&> t1(42, y, xptr);
    int x         = 55;
    double dx     = 42.42;
    const char* s = "hello World";
    std::tuple<int&, double&&, const char*> t2(x, std::move(dx), s);
    test_pmr_not_uses_allocator<PMR>(t1, std::move(t2));
    test_pmr_not_uses_allocator<PMR>(std::move(t2), t1);
    test_pmr_uses_allocator<PMA>(t1, std::move(t2));
    test_pmr_uses_allocator<PMA>(std::move(t2), t1);
  }

  return 0;
}
