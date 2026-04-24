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
