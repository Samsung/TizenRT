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
// <optional>

// template <class T> struct hash<optional<T>>;

#include <optional>
#include <string>
#include <memory>
#include <cassert>

#include "poisoned_hash_helper.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A {};
struct B {};

namespace std {

template <>
struct hash<B> {
  std::size_t operator()(B const&) noexcept(false) { return 0; }
};

}

int tc_utilities_optional_optional_hash_hash(void) {
    using std::optional;
    const std::size_t nullopt_hash =
        std::hash<optional<double>>{}(optional<double>{});


    {
        optional<B> opt;
        ASSERT_NOT_NOEXCEPT(std::hash<optional<B>>()(opt));
        ASSERT_NOT_NOEXCEPT(std::hash<optional<const B>>()(opt));
    }

    {
        typedef int T;
        optional<T> opt;
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = 2;
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::string T;
        optional<T> opt;
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::string("123");
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
        typedef std::unique_ptr<int> T;
        optional<T> opt;
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == nullopt_hash);
        opt = std::unique_ptr<int>(new int(3));
        TC_ASSERT_EXPR(std::hash<optional<T>>{}(opt) == std::hash<T>{}(*opt));
    }
    {
      test_hash_enabled_for_type<std::optional<int> >();
      test_hash_enabled_for_type<std::optional<int*> >();
      test_hash_enabled_for_type<std::optional<const int> >();
      test_hash_enabled_for_type<std::optional<int* const> >();

      test_hash_disabled_for_type<std::optional<A>>();
      test_hash_disabled_for_type<std::optional<const A>>();

      test_hash_enabled_for_type<std::optional<B>>();
      test_hash_enabled_for_type<std::optional<const B>>();
    }

  return 0;
}
