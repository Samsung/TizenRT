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

// <memory>

// unique_ptr

// The following constructors should not be selected by class template argument
// deduction:
//
// constexpr explicit unique_ptr(pointer p) // constexpr since C++23
// constexpr unique_ptr(pointer p, const D& d) noexcept // constexpr since C++23
// constexpr unique_ptr(pointer p, remove_reference_t<D>&& d) noexcept // constexpr since C++23

#include <memory>

#include "deduction_guides_sfinae_checks.h"
#include "libcxx_tc_common.h"

struct Deleter {
  void operator()(int* p) const { delete p; }
};

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_ctor_deduct(void) {
  // Cannot deduce from (ptr).
  static_assert(SFINAEs_away<std::unique_ptr, int*>);
  // Cannot deduce from (array).
  static_assert(SFINAEs_away<std::unique_ptr, int[]>);
  // Cannot deduce from (ptr, Deleter&&).
  static_assert(SFINAEs_away<std::unique_ptr, int*, Deleter&&>);
  // Cannot deduce from (array, Deleter&&).
  static_assert(SFINAEs_away<std::unique_ptr, int[], Deleter&&>);
  // Cannot deduce from (ptr, const Deleter&).
  static_assert(SFINAEs_away<std::unique_ptr, int*, const Deleter&>);
  // Cannot deduce from (array, const Deleter&).
  static_assert(SFINAEs_away<std::unique_ptr, int[], const Deleter&>);

  return 0;
}
