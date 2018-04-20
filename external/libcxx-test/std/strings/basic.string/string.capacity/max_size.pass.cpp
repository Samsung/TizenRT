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

// UNSUPPORTED: libcpp-no-exceptions
// <string>

// size_type max_size() const;

// NOTE: asan and msan will fail for one of two reasons
// 1. If allocator_may_return_null=0 then they will fail because the allocation
//    returns null.
// 2. If allocator_may_return_null=1 then they will fail because the allocation
//    is too large to succeed.
// UNSUPPORTED: sanitizer-new-delete

#include <string>
#include <cassert>


template <class S>
void
test1(const S& s)
{
    S s2(s);
    const size_t sz = s2.max_size() - 1;
    try { s2.resize(sz, 'x'); }
    catch ( const std::bad_alloc & ) { return ; }
    assert ( s2.size() ==  sz );
}

template <class S>
void
test2(const S& s)
{
    S s2(s);
    const size_t sz = s2.max_size();
    try { s2.resize(sz, 'x'); }
    catch ( const std::bad_alloc & ) { return ; }
    assert ( s.size() ==  sz );
}

template <class S>
void
test(const S& s)
{
    assert(s.max_size() >= s.size());
    test1(s);
    test2(s);
}

int main()
{
    {
    typedef std::string S;
    test(S());
    test(S("123"));
    test(S("12345678901234567890123456789012345678901234567890"));
    }
}
