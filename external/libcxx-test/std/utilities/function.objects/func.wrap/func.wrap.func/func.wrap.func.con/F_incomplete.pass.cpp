//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// template<class F> function(F);

// Allow incomplete argument types in the __is_callable check

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct X{
    typedef std::function<void(X&)> callback_type;
    virtual ~X() {}
private:
    callback_type _cb;
};

struct IncompleteReturnType {
  std::function<IncompleteReturnType ()> fn;
};


int called = 0;
IncompleteReturnType test_fn() {
  ++called;
  IncompleteReturnType I;
  return I;
}

// See llvm.org/PR34298
void test_pr34298()
{
  static_assert(std::is_copy_constructible<IncompleteReturnType>::value, "");
  static_assert(std::is_copy_assignable<IncompleteReturnType>::value, "");
  {
    IncompleteReturnType X;
    X.fn = test_fn;
    const IncompleteReturnType& CX = X;
    IncompleteReturnType X2 = CX;
    TC_ASSERT_EXPR(X2.fn);
    TC_ASSERT_EXPR(called == 0);
    X2.fn();
    TC_ASSERT_EXPR(called == 1);
  }
  {
    IncompleteReturnType Empty;
    IncompleteReturnType X2 = Empty;
    TC_ASSERT_EXPR(!X2.fn);
  }
}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_incomplete(void) {
  test_pr34298();

  return 0;
}
