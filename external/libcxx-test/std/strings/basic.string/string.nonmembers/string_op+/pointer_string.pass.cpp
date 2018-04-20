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
//   operator+(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(const charT* lhs, basic_string<charT,traits,Allocator>&& rhs);

#include <string>
#include <utility>
#include <cassert>

#include "test_macros.h"

template <class S>
void test0(const typename S::value_type* lhs, const S& rhs, const S& x) {
  assert(lhs + rhs == x);
}


int main() {
  {
    typedef std::string S;
    test0("", S(""), S(""));
    test0("", S("12345"), S("12345"));
    test0("", S("1234567890"), S("1234567890"));
    test0("", S("12345678901234567890"), S("12345678901234567890"));
    test0("abcde", S(""), S("abcde"));
    test0("abcde", S("12345"), S("abcde12345"));
    test0("abcde", S("1234567890"), S("abcde1234567890"));
    test0("abcde", S("12345678901234567890"), S("abcde12345678901234567890"));
    test0("abcdefghij", S(""), S("abcdefghij"));
    test0("abcdefghij", S("12345"), S("abcdefghij12345"));
    test0("abcdefghij", S("1234567890"), S("abcdefghij1234567890"));
    test0("abcdefghij", S("12345678901234567890"),
          S("abcdefghij12345678901234567890"));
    test0("abcdefghijklmnopqrst", S(""), S("abcdefghijklmnopqrst"));
    test0("abcdefghijklmnopqrst", S("12345"), S("abcdefghijklmnopqrst12345"));
    test0("abcdefghijklmnopqrst", S("1234567890"),
          S("abcdefghijklmnopqrst1234567890"));
    test0("abcdefghijklmnopqrst", S("12345678901234567890"),
          S("abcdefghijklmnopqrst12345678901234567890"));
  }

}
