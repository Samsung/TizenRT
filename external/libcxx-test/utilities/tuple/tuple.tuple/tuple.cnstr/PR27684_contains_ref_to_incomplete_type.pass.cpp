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
//
// UNSUPPORTED: c++03

// <tuple>

// template <class... Types> class tuple;

// template <class Alloc> tuple(allocator_arg_t, Alloc const&)

// See https://llvm.org/PR27684.

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct IncompleteType;
extern IncompleteType inc1;
extern IncompleteType inc2;
IncompleteType const& cinc1 = inc1;
IncompleteType const& cinc2 = inc2;

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_PR27684_contains_ref_to_incomplete_type(void) {
    using IT = IncompleteType;
    { // try calling tuple(Tp const&...)
        using Tup = std::tuple<const IT&, const IT&>;
        Tup t(cinc1, cinc2);
        TC_ASSERT_EXPR(&std::get<0>(t) == &inc1);
        TC_ASSERT_EXPR(&std::get<1>(t) == &inc2);
    }
    { // try calling tuple(Up&&...)
        using Tup = std::tuple<const IT&, const IT&>;
        Tup t(inc1, inc2);
        TC_ASSERT_EXPR(&std::get<0>(t) == &inc1);
        TC_ASSERT_EXPR(&std::get<1>(t) == &inc2);
    }

  return 0;
}

struct IncompleteType {};
IncompleteType inc1;
IncompleteType inc2;
