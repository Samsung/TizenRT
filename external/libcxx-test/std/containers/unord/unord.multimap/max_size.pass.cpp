//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// class unordered_multimap

// size_type max_size() const;

#include <cassert>
#include "libcxx_tc_common.h"
#include <limits>
#include <type_traits>
#include <unordered_map>

#include "test_allocator.h"
#include "test_macros.h"

int tc_libcxx_containers_unord_multimap_max_size(void)
{
  typedef std::pair<const int, int> KV;
  {
    typedef limited_allocator<KV, 10> A;
    typedef std::unordered_multimap<int, int, std::hash<int>,
                                    std::equal_to<int>, A>
        C;
    C c;
    TC_ASSERT_EXPR(c.max_size() <= 10);
    LIBCPP_ASSERT(c.max_size() == 10);
  }
  {
    typedef limited_allocator<KV, (size_t)-1> A;
    typedef std::unordered_multimap<int, int, std::hash<int>,
                                    std::equal_to<int>, A>
        C;
    C c;
    TC_ASSERT_EXPR(c.max_size() <= (size_t)-1);
    LIBCPP_ASSERT(c.max_size() == (size_t)-1);
    }
    {
      typedef std::unordered_multimap<char, int> C;
      const C::difference_type max_dist =
          std::numeric_limits<C::difference_type>::max();
      C c;
      TC_ASSERT_EXPR(c.max_size() <= max_dist);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
