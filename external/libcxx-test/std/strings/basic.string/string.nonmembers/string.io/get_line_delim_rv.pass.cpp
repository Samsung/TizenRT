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

// UNSUPPORTED: c++98, c++03

// <string>

// template<class charT, class traits, class Allocator>
//   basic_istream<charT,traits>&
//   getline(basic_istream<charT,traits>&& is,
//           basic_string<charT,traits,Allocator>& str, charT delim);

#include <string>
#include <sstream>
#include <cassert>


int main()
{
    {
        std::string s("initial text");
        getline(std::istringstream(" abc*  def*   ghij"), s, '*');
        assert(s == " abc");
    }
    {
        std::wstring s(L"initial text");
        getline(std::wistringstream(L" abc*  def*   ghij"), s, L'*');
        assert(s == L" abc");
    }
}
