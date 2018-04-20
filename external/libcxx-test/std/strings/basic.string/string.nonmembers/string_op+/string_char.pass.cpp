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

// <string>

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>
//   operator+(const basic_string<charT,traits,Allocator>& lhs, charT rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(basic_string<charT,traits,Allocator>&& lhs, charT rhs);

#include <string>
#include <utility>
#include <cassert>

#include "test_macros.h"

template <class S>
void test0(const S& lhs, typename S::value_type rhs, const S& x) {
  assert(lhs + rhs == x);
}


int main() {
  {
    typedef std::string S;
    test0(S(""), '1', S("1"));
    test0(S("abcde"), '1', S("abcde1"));
    test0(S("abcdefghij"), '1', S("abcdefghij1"));
    test0(S("abcdefghijklmnopqrst"), '1', S("abcdefghijklmnopqrst1"));
  }
}
