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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <list>

// static int remove(const value_type& value);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

struct S {
  S(int i) : i_(new int(i)) {}
  S(const S &rhs) : i_(new int(*rhs.i_)) {}
  S &operator=(const S &rhs) {
    *i_ = *rhs.i_;
    return *this;
  }
  ~S() {
    delete i_;
    i_ = NULL;
  }
  bool operator==(const S &rhs) const { return *i_ == *rhs.i_; }
  int get() const { return *i_; }
  int *i_;
};

int tc_libcxx_containers_list_ops_remove(void) {
  {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {1, 2, 4};
    std::list<int> c(a1, a1 + 4);
    c.remove(3);
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2 + 3));
  }
  { // LWG issue #526
    int a1[] = {1, 2, 1, 3, 5, 8, 11};
    int a2[] = {2, 3, 5, 8, 11};
    std::list<int> c(a1, a1 + 7);
    c.remove(c.front());
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2 + 5));
  }
  {
    int a1[] = {1, 2, 1, 3, 5, 8, 11, 1};
    int a2[] = {2, 3, 5, 8, 11};
    std::list<S> c;
    for (int *ip = a1; ip < a1 + 8; ++ip)
      c.push_back(S(*ip));
    c.remove(c.front());
    std::list<S>::const_iterator it = c.begin();
    for (int *ip = a2; ip < a2 + 5; ++ip, ++it) {
      TC_ASSERT_EXPR(it != c.end());
      TC_ASSERT_EXPR(*ip == it->get());
    }
    TC_ASSERT_EXPR(it == c.end());
  }
  {
  }
    TC_SUCCESS_RESULT();
    return 0;
}
