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

// <unordered_map>

// Check that std::unordered_map and its iterators can be instantiated with an incomplete
// type.

#include <unordered_map>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Tp>
struct MyHash {
  MyHash() {}
  std::size_t operator()(Tp const&) const {return 42;}
};

struct A {
    typedef std::unordered_map<A, A, MyHash<A> > Map;
    Map m;
    Map::iterator it;
    Map::const_iterator cit;
    Map::local_iterator lit;
    Map::const_local_iterator clit;
};

inline bool operator==(A const& L, A const& R) { return &L == &R; }

int tc_containers_unord_unord_map_incomplete_type(void) {
    A a;

  return 0;
}
