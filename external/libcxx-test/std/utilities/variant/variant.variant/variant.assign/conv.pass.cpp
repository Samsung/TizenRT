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

// <variant>

// template <class ...Types> class variant;

// template <class T>
// variant& operator=(T&&) noexcept(see below);

#include <variant>
#include <string>
#include <memory>

#include "variant_test_helpers.h"
#include "libcxx_tc_common.h"

int tc_utilities_variant_variant_variant_variant_assign_conv(void) {
  static_assert(!std::is_assignable<std::variant<int, int>, int>::value, "");
  static_assert(!std::is_assignable<std::variant<long, long long>, int>::value, "");
  static_assert(std::is_assignable<std::variant<char>, int>::value == VariantAllowsNarrowingConversions, "");

  static_assert(std::is_assignable<std::variant<std::string, float>, int>::value == VariantAllowsNarrowingConversions, "");
  static_assert(std::is_assignable<std::variant<std::string, double>, int>::value == VariantAllowsNarrowingConversions, "");
  static_assert(!std::is_assignable<std::variant<std::string, bool>, int>::value, "");

  static_assert(!std::is_assignable<std::variant<int, bool>, decltype("meow")>::value, "");
  static_assert(!std::is_assignable<std::variant<int, const bool>, decltype("meow")>::value, "");
  static_assert(!std::is_assignable<std::variant<int, const volatile bool>, decltype("meow")>::value, "");

  static_assert(!std::is_assignable<std::variant<bool>, std::true_type>::value, "");
  static_assert(!std::is_assignable<std::variant<bool>, std::unique_ptr<char> >::value, "");
  static_assert(!std::is_assignable<std::variant<bool>, decltype(nullptr)>::value, "");

  return 0;
}
