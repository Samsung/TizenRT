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

// <memory_resource>

// memory_resource *null_memory_resource()

#include <memory_resource>
#include <cassert>
#include <cstddef> // size_t
#include <new>
#include <type_traits>

#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct assert_on_compare : public std::pmr::memory_resource {
  void* do_allocate(std::size_t, size_t) override {
    TC_ASSERT_EXPR(false);
    return nullptr;
  }

  void do_deallocate(void*, std::size_t, size_t) override { TC_ASSERT_EXPR(false); }

  bool do_is_equal(const std::pmr::memory_resource&) const noexcept override {
    TC_ASSERT_EXPR(false);
    return true;
  }
};

void test_return() {
  { ASSERT_SAME_TYPE(decltype(std::pmr::null_memory_resource()), std::pmr::memory_resource*); }
  // Test that the returned value is not null
  { TC_ASSERT_EXPR(std::pmr::null_memory_resource()); }
  // Test the same value is returned by repeated calls.
  { TC_ASSERT_EXPR(std::pmr::null_memory_resource() == std::pmr::null_memory_resource()); }
}

void test_equality() {
  // Same object
  {
    std::pmr::memory_resource& r1 = *std::pmr::null_memory_resource();
    std::pmr::memory_resource& r2 = *std::pmr::null_memory_resource();
    // check both calls returned the same object
    TC_ASSERT_EXPR(&r1 == &r2);
    // check for proper equality semantics
    TC_ASSERT_EXPR(r1 == r2);
    TC_ASSERT_EXPR(r2 == r1);
    TC_ASSERT_EXPR(!(r1 != r2));
    TC_ASSERT_EXPR(!(r2 != r1));
    // check the is_equal method
    TC_ASSERT_EXPR(r1.is_equal(r2));
    TC_ASSERT_EXPR(r2.is_equal(r1));
  }
  // Different types
  {
    std::pmr::memory_resource& r1 = *std::pmr::null_memory_resource();
    assert_on_compare c;
    std::pmr::memory_resource& r2 = c;
    TC_ASSERT_EXPR(r1 != r2);
    TC_ASSERT_EXPR(!(r1 == r2));
    TC_ASSERT_EXPR(!r1.is_equal(r2));
  }
}

void test_allocate() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  DisableAllocationGuard g; // null_memory_resource shouldn't allocate.
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    (void)std::pmr::null_memory_resource()->allocate(1);
    TC_ASSERT_EXPR(false);
  } catch (std::bad_alloc const&) {
    // do nothing
  } catch (...) {
    TC_ASSERT_EXPR(false);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif
}

void test_deallocate() {
  globalMemCounter.reset();

  int x = 42;
  std::pmr::null_memory_resource()->deallocate(&x, 0);

  TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));
  TC_ASSERT_EXPR(globalMemCounter.checkDeleteArrayCalledEq(0));
}

int tc_utilities_utility_mem_res_mem_res_global_null_memory_resource(void) {
  test_return();
  test_equality();
  test_allocate();
  test_deallocate();

  return 0;
}
