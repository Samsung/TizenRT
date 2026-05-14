//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// ~optional();

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct PODType {
  int value;
  int value2;
};

class X
{
public:
    static bool dtor_called;
    X() = default;
    X(const X&) = default;
    X& operator=(const X&) = default;
    ~X() {dtor_called = true;}
};

bool X::dtor_called = false;

int tc_utilities_optional_optional_object_optional_object_dtor_dtor(void) {
    {
        typedef int T;
        static_assert(std::is_trivially_destructible<T>::value, "");
        static_assert(std::is_trivially_destructible<optional<T>>::value, "");
    }
    {
        typedef double T;
        static_assert(std::is_trivially_destructible<T>::value, "");
        static_assert(std::is_trivially_destructible<optional<T>>::value, "");
    }
    {
        typedef PODType T;
        static_assert(std::is_trivially_destructible<T>::value, "");
        static_assert(std::is_trivially_destructible<optional<T>>::value, "");
    }
    {
        typedef X T;
        static_assert(!std::is_trivially_destructible<T>::value, "");
        static_assert(!std::is_trivially_destructible<optional<T>>::value, "");
        {
            X x;
            optional<X> opt{x};
            TC_ASSERT_EXPR(X::dtor_called == false);
        }
        TC_ASSERT_EXPR(X::dtor_called == true);
    }

  return 0;
}
