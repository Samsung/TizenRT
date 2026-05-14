//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <forward_list>

// class forward_list

// forward_list();

#include <forward_list>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct X
{
    std::forward_list<X> q;
};

int tc_containers_sequences_forwardlist_forwardlist_cons_default_recursive(void) {

  return 0;
}
