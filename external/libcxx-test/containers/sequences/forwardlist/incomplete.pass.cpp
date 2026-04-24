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
// <forward_list>

// forward_list()
// forward_list::iterator()
// forward_list::const_iterator()

#include <forward_list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

struct A {
  std::forward_list<A> d;
  std::forward_list<A>::iterator it;
  std::forward_list<A>::const_iterator it2;
};

#if TEST_STD_VER >= 11
struct B {
  typedef std::forward_list<B, min_allocator<B>> FList;
  FList d;
  FList::iterator it;
  FList::const_iterator it2;
};
#endif

int tc_containers_sequences_forwardlist_incomplete(void) {
  {
    A a;
    TC_ASSERT_EXPR(a.d.empty());
    a.it = a.d.begin();
    a.it2 = a.d.cbefore_begin();
  }
#if TEST_STD_VER >= 11
  {
    B b;
    TC_ASSERT_EXPR(b.d.empty());
    b.it = b.d.begin();
    b.it2 = b.d.cbefore_begin();
  }
#endif

  return 0;
}
