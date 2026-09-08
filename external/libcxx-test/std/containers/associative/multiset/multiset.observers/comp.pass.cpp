//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// key_compare key_comp() const;
// value_compare value_comp() const;

#include <set>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_observers_comp(void) {
    typedef std::multiset<int> set_type;

    set_type s;
    set_type::iterator i1 = s.insert(1);
    set_type::iterator i2 = s.insert(2);

    const set_type& cs = s;

    TC_ASSERT_EXPR(cs.key_comp()(*i1, *i2));
    TC_ASSERT_EXPR(!cs.key_comp()(*i2, *i1));

    TC_ASSERT_EXPR(cs.value_comp()(*i1, *i2));
    TC_ASSERT_EXPR(!cs.value_comp()(*i2, *i1));

    return 0;
}
