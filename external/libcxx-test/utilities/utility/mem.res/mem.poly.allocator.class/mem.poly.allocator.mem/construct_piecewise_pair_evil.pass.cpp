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

// template <class T> class polymorphic_allocator

// template <class U1, class U2, class ...Args1, class ...Args2>
// void polymorphic_allocator<T>::construct(pair<U1, U2>*, piecewise_construct_t
//                                          tuple<Args1...>, tuple<Args2...>)

#include <memory_resource>
#include <tuple>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
struct EvilAlloc {
  explicit EvilAlloc() : inner_(std::pmr::null_memory_resource()) {}

  EvilAlloc(std::pmr::polymorphic_allocator<T>& a) : inner_(a) {}
  EvilAlloc(std::pmr::polymorphic_allocator<T>&& a) : inner_(a) {}
  EvilAlloc(std::pmr::polymorphic_allocator<T> const& a)  = delete;
  EvilAlloc(std::pmr::polymorphic_allocator<T> const&& a) = delete;

  using value_type = T;
  template <class U>
  EvilAlloc(EvilAlloc<U> const& rhs) : inner_(rhs.inner_) {}

  std::pmr::polymorphic_allocator<T> inner_;
};

struct WidgetV0 {
  WidgetV0(int v) : value_(v) {}

  bool holds(int v, const std::pmr::polymorphic_allocator<char>&) const { return value_ == v; }

private:
  int value_;
};

struct WidgetV1 {
  using allocator_type = EvilAlloc<char>;

  WidgetV1(int v) : value_(v), alloc_() {}
  WidgetV1(std::allocator_arg_t, EvilAlloc<char> a, int v) : value_(v), alloc_(a) {}

  bool holds(int v, const std::pmr::polymorphic_allocator<char>& a) const { return value_ == v && alloc_.inner_ == a; }

private:
  int value_;
  EvilAlloc<char> alloc_;
};

struct WidgetV2 {
  using allocator_type = EvilAlloc<char>;

  WidgetV2(int v) : value_(v), alloc_() {}
  WidgetV2(int v, EvilAlloc<char> a) : value_(v), alloc_(a) {}

  bool holds(int v, std::pmr::polymorphic_allocator<char> a) const { return value_ == v && alloc_.inner_ == a; }

private:
  int value_;
  EvilAlloc<char> alloc_;
};

struct WidgetV3 {
  using allocator_type = EvilAlloc<char>;

  WidgetV3(int v) : value_(v), alloc_() {}
  WidgetV3(std::allocator_arg_t, EvilAlloc<char> a, int v) : value_(v), alloc_(a) {}
  WidgetV3(int v, EvilAlloc<char> a) : value_(v), alloc_(a) {}

  bool holds(int v, std::pmr::polymorphic_allocator<char> a) const { return value_ == v && alloc_.inner_ == a; }

private:
  int value_;
  EvilAlloc<char> alloc_;
};

static_assert(std::uses_allocator<WidgetV1, EvilAlloc<char>>::value, "");
static_assert(std::uses_allocator<WidgetV2, EvilAlloc<char>>::value, "");
static_assert(std::uses_allocator<WidgetV3, EvilAlloc<char>>::value, "");
static_assert(std::uses_allocator<WidgetV1, std::pmr::polymorphic_allocator<char>>::value, "");
static_assert(std::uses_allocator<WidgetV2, std::pmr::polymorphic_allocator<char>>::value, "");
static_assert(std::uses_allocator<WidgetV3, std::pmr::polymorphic_allocator<char>>::value, "");

template <class W1, class W2>
void test_evil() {
  using PMA = std::pmr::polymorphic_allocator<char>;
  PMA pma(std::pmr::new_delete_resource());
  {
    using Pair  = std::pair<W1, W2>;
    alignas(Pair) char buffer[sizeof(Pair)];
    Pair* p = reinterpret_cast<Pair*>(buffer);
    pma.construct(p, std::piecewise_construct, std::make_tuple(42), std::make_tuple(42));
    TC_ASSERT_EXPR(p->first.holds(42, pma));
    TC_ASSERT_EXPR(p->second.holds(42, pma));
    pma.destroy(p);
  }
}

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_construct_piecewise_pair_evil(void) {
  test_evil<WidgetV0, WidgetV0>();
  test_evil<WidgetV0, WidgetV1>();
  test_evil<WidgetV0, WidgetV2>();
  test_evil<WidgetV0, WidgetV3>();
  test_evil<WidgetV1, WidgetV0>();
  test_evil<WidgetV1, WidgetV1>();
  test_evil<WidgetV1, WidgetV2>();
  test_evil<WidgetV1, WidgetV3>();
  test_evil<WidgetV2, WidgetV0>();
  test_evil<WidgetV2, WidgetV1>();
  test_evil<WidgetV2, WidgetV2>();
  test_evil<WidgetV2, WidgetV3>();
  test_evil<WidgetV3, WidgetV0>();
  test_evil<WidgetV3, WidgetV1>();
  test_evil<WidgetV3, WidgetV2>();
  test_evil<WidgetV3, WidgetV3>();

  return 0;
}
