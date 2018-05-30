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

#ifndef ASAN_TESTING_H
#define ASAN_TESTING_H

#include "test_macros.h"

#if TEST_HAS_FEATURE(address_sanitizer)
extern "C" int __sanitizer_verify_contiguous_container
     ( const void *beg, const void *mid, const void *end );

template <typename T, typename Alloc>
bool is_contiguous_container_asan_correct ( const std::vector<T, Alloc> &c )
{
    if ( std::is_same<Alloc, std::allocator<T> >::value && c.data() != NULL)
        return __sanitizer_verify_contiguous_container (
            c.data(), c.data() + c.size(), c.data() + c.capacity()) != 0;
    return true;
}

#else
template <typename T, typename Alloc>
bool is_contiguous_container_asan_correct ( const std::vector<T, Alloc> &)
{
    return true;
}
#endif


#endif  // ASAN_TESTING_H
