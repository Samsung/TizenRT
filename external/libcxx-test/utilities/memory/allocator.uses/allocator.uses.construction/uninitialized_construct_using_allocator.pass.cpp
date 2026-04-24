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
// template<class T, class Alloc, class... Args>
//   constexpr T uninitialized_construct_using_allocator(const Alloc& alloc, Args&&... args);

// UNSUPPORTED: c++03, c++11, c++14, c++17

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

#include <concepts>
#include <memory>
#include <tuple>
#include <utility>

#include "common.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  Alloc a(12);
  {
    auto* ptr                             = std::allocator<UsesAllocArgT>{}.allocate(1);
    std::same_as<UsesAllocArgT*> auto ret = std::uninitialized_construct_using_allocator(ptr, a);
    TC_ASSERT_EXPR(ret == ptr);
    TC_ASSERT_EXPR(ret->allocator_constructed_);
    TC_ASSERT_EXPR(&ret->alloc_ == &a);
    std::allocator<UsesAllocArgT>{}.deallocate(ptr, 1);
  }
  {
    auto* ptr                             = std::allocator<UsesAllocLast>{}.allocate(1);
    std::same_as<UsesAllocLast*> auto ret = std::uninitialized_construct_using_allocator(ptr, a);
    TC_ASSERT_EXPR(ret->allocator_constructed_);
    TC_ASSERT_EXPR(&ret->alloc_ == &a);
    std::allocator<UsesAllocLast>{}.deallocate(ptr, 1);
  }
  {
    auto* ptr                                 = std::allocator<NotAllocatorAware>{}.allocate(1);
    std::same_as<NotAllocatorAware*> auto ret = std::uninitialized_construct_using_allocator(ptr, a);
    TC_ASSERT_EXPR(!ret->allocator_constructed_);
    std::allocator<NotAllocatorAware>{}.deallocate(ptr, 1);
  }
  {
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, std::piecewise_construct, std::tuple<>{}, std::tuple<>{});
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
  {
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a);
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
  {
    int val   = 0;
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, val, val);
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &val);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &val);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
  {
    int val   = 0;
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, std::move(val), std::move(val));
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &val);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &val);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
#if TEST_STD_VER >= 23
  {
    std::pair p{0, 0};

    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, p);
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &p.second);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
#endif
  {
    std::pair p{0, 0};
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, std::as_const(p));
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::ConstLValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::ConstLValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &p.second);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
  {
    std::pair p{0, 0};
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, std::move(p));
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &p.second);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
#if TEST_STD_VER >= 23
  {
    std::pair p{0, 0};
    auto* ptr = std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.allocate(1);
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>*> auto ret =
        std::uninitialized_construct_using_allocator(ptr, a, std::move(std::as_const(p)));
    TC_ASSERT_EXPR(ret->first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->first.alloc_ == &a);
    TC_ASSERT_EXPR(ret->first.ref_type_ == RefType::ConstRValue);
    TC_ASSERT_EXPR(ret->first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret->second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret->second.alloc_ == &a);
    TC_ASSERT_EXPR(ret->second.ref_type_ == RefType::ConstRValue);
    TC_ASSERT_EXPR(ret->second.val_ptr_ == &p.second);
    std::allocator<std::pair<UsesAllocArgT, UsesAllocLast>>{}.deallocate(ptr, 1);
  }
#endif
  {
    ConvertibleToPair ctp;
    auto* ptr                                   = std::allocator<std::pair<int, int>>{}.allocate(1);
    std::same_as<std::pair<int, int>*> auto ret = std::uninitialized_construct_using_allocator(ptr, a, ctp);
    TC_ASSERT_EXPR(ret == ptr);
    TC_ASSERT_EXPR(ret->first == 1);
    TC_ASSERT_EXPR(ret->second == 2);
    std::allocator<std::pair<int, int>>{}.deallocate(ptr, 1);
  }
  {
    ConvertibleToPair ctp;
    auto* ptr                                   = std::allocator<std::pair<int, int>>{}.allocate(1);
    std::same_as<std::pair<int, int>*> auto ret = std::uninitialized_construct_using_allocator(ptr, a, std::move(ctp));
    TC_ASSERT_EXPR(ret == ptr);
    TC_ASSERT_EXPR(ret->first == 1);
    TC_ASSERT_EXPR(ret->second == 2);
    std::allocator<std::pair<int, int>>{}.deallocate(ptr, 1);
  }

  return true;
}

int tc_utilities_memory_allocator_uses_allocator_uses_construction_uninitialized_construct_using_allocator(void) {
  test();
  static_assert(test());
}
