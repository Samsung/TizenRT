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
//   constexpr T make_obj_using_allocator(const Alloc& alloc, Args&&... args);

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
    std::same_as<UsesAllocArgT> auto ret = std::make_obj_using_allocator<UsesAllocArgT>(a);
    TC_ASSERT_EXPR(ret.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.alloc_ == &a);
  }
  {
    std::same_as<UsesAllocLast> auto ret = std::make_obj_using_allocator<UsesAllocLast>(a);
    TC_ASSERT_EXPR(ret.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.alloc_ == &a);
  }
  {
    std::same_as<NotAllocatorAware> auto ret = std::make_obj_using_allocator<NotAllocatorAware>(a);
    TC_ASSERT_EXPR(!ret.allocator_constructed_);
  }
  {
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(
            a, std::piecewise_construct, std::tuple<>{}, std::tuple<>{});
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
  }
  {
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a);
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
  }
  {
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a, 0, 0);
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
  }
#if TEST_STD_VER >= 23
  {
    std::pair p{0, 0};

    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a, p);
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.first.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret.first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.ref_type_ == RefType::LValue);
    TC_ASSERT_EXPR(ret.second.val_ptr_ == &p.second);
  }
#endif
  {
    std::pair p{0, 0};
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a, std::as_const(p));
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.first.ref_type_ == RefType::ConstLValue);
    TC_ASSERT_EXPR(ret.first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.ref_type_ == RefType::ConstLValue);
    TC_ASSERT_EXPR(ret.second.val_ptr_ == &p.second);
  }
  {
    std::pair p{0, 0};
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a, std::move(p));
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.first.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret.first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.ref_type_ == RefType::RValue);
    TC_ASSERT_EXPR(ret.second.val_ptr_ == &p.second);
  }
#if TEST_STD_VER >= 23
  {
    std::pair p{0, 0};
    std::same_as<std::pair<UsesAllocArgT, UsesAllocLast>> auto ret =
        std::make_obj_using_allocator<std::pair<UsesAllocArgT, UsesAllocLast>>(a, std::move(std::as_const(p)));
    TC_ASSERT_EXPR(ret.first.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.first.alloc_ == &a);
    TC_ASSERT_EXPR(ret.first.ref_type_ == RefType::ConstRValue);
    TC_ASSERT_EXPR(ret.first.val_ptr_ == &p.first);
    TC_ASSERT_EXPR(ret.second.allocator_constructed_);
    TC_ASSERT_EXPR(&ret.second.alloc_ == &a);
    TC_ASSERT_EXPR(ret.second.ref_type_ == RefType::ConstRValue);
    TC_ASSERT_EXPR(ret.second.val_ptr_ == &p.second);
  }
#endif
  {
    ConvertibleToPair ctp;
    std::same_as<std::pair<int, int>> auto ret = std::make_obj_using_allocator<std::pair<int, int>>(a, ctp);
    TC_ASSERT_EXPR(ret.first == 1);
    TC_ASSERT_EXPR(ret.second == 2);
  }
  {
    ConvertibleToPair ctp;
    std::same_as<std::pair<int, int>> auto ret = std::make_obj_using_allocator<std::pair<int, int>>(a, std::move(ctp));
    TC_ASSERT_EXPR(ret.first == 1);
    TC_ASSERT_EXPR(ret.second == 2);
  }

  return true;
}

int tc_utilities_memory_allocator_uses_allocator_uses_construction_make_obj_using_allocator(void) {
  test();
  static_assert(test());
}
