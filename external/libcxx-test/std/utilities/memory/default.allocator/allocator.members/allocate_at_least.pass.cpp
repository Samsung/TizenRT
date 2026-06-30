//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <memory>

// allocation_result<T*> allocate_at_least(size_t n)

#include <cassert>
#include <concepts>
#include <memory>

#include "count_new.h"
#include "libcxx_tc_common.h"

#ifdef TEST_HAS_NO_ALIGNED_ALLOCATION
static const bool UsingAlignedNew = false;
#else
static const bool UsingAlignedNew = true;
#endif

#ifdef __STDCPP_DEFAULT_NEW_ALIGNMENT__
static const std::size_t MaxAligned = __STDCPP_DEFAULT_NEW_ALIGNMENT__;
#else
static const std::size_t MaxAligned = std::alignment_of<std::max_align_t>::value;
#endif

static const std::size_t OverAligned = MaxAligned * 2;

template <std::size_t Align>
struct alignas(Align) AlignedType {
  char data;
  static int constructed;
  AlignedType() { ++constructed; }
  AlignedType(AlignedType const&) { ++constructed; }
  ~AlignedType() { --constructed; }
};
template <std::size_t Align>
int AlignedType<Align>::constructed = 0;


template <std::size_t Align>
void test_aligned() {
  typedef AlignedType<Align> T;
  T::constructed = 0;
  globalMemCounter.reset();
  std::allocator<T> a;
  const bool IsOverAlignedType = Align > MaxAligned;
  const bool ExpectAligned = IsOverAlignedType && UsingAlignedNew;
  {
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    TC_ASSERT_EXPR(T::constructed == 0);
    globalMemCounter.last_new_size = 0;
    globalMemCounter.last_new_align = 0;
    std::same_as<std::allocation_result<T*>> decltype(auto) ap = a.allocate_at_least(3);
    TC_ASSERT_EXPR(ap.count >= 3);
    DoNotOptimize(ap);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(1));
    TC_ASSERT_EXPR(globalMemCounter.checkAlignedNewCalledEq(ExpectAligned));
    TC_ASSERT_EXPR(globalMemCounter.checkLastNewSizeEq(3 * sizeof(T)));
    TC_ASSERT_EXPR(globalMemCounter.checkLastNewAlignEq(ExpectAligned ? Align : 0));
    TC_ASSERT_EXPR(T::constructed == 0);
    globalMemCounter.last_delete_align = 0;
    a.deallocate(ap.ptr, 3);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(1));
    TC_ASSERT_EXPR(globalMemCounter.checkAlignedDeleteCalledEq(ExpectAligned));
    TC_ASSERT_EXPR(globalMemCounter.checkLastDeleteAlignEq(ExpectAligned ? Align : 0));
    TC_ASSERT_EXPR(T::constructed == 0);
  }
}

template <std::size_t Align>
constexpr bool test_aligned_constexpr() {
    typedef AlignedType<Align> T;
    std::allocator<T> a;
    std::same_as<std::allocation_result<T*>> decltype(auto) ap = a.allocate_at_least(3);
    TC_ASSERT_EXPR(ap.count >= 3);
    a.deallocate(ap.ptr, 3);

    return true;
}

int tc_utilities_memory_default_allocator_allocator_members_allocate_at_least(void) {
    test_aligned<1>();
    test_aligned<2>();
    test_aligned<4>();
    test_aligned<8>();
    test_aligned<16>();
    test_aligned<MaxAligned>();
    test_aligned<OverAligned>();
    test_aligned<OverAligned * 2>();

    static_assert(test_aligned_constexpr<1>());
    static_assert(test_aligned_constexpr<2>());
    static_assert(test_aligned_constexpr<4>());
    static_assert(test_aligned_constexpr<8>());
    static_assert(test_aligned_constexpr<16>());
    static_assert(test_aligned_constexpr<MaxAligned>());
    static_assert(test_aligned_constexpr<OverAligned>());
    static_assert(test_aligned_constexpr<OverAligned * 2>());
  return 0;
}
