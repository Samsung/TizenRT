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
// UNSUPPORTED: c++03

// <algorithm>

// We optimize std::copy(_backward) and std::move(_backward) into memmove
// when the iterator is trivial and contiguous and the type in question
// is also trivially (copyable, movable). This test verifies that the
// optimization never eliminates an actually non-trivial copy or move.

#include <algorithm>
#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct TMBNTC {
    int *p;
    constexpr TMBNTC(int& copies) : p(&copies) {}
    constexpr TMBNTC(const TMBNTC&) = default;
    TEST_CONSTEXPR_CXX14 TMBNTC& operator=(TMBNTC&&) = default;
    TEST_CONSTEXPR_CXX14 TMBNTC& operator=(const TMBNTC&) { ++*p; return *this; }
};

TEST_CONSTEXPR_CXX20 bool
test_trivial_moveassign_but_no_trivial_copyassign()
{
    int copies = 0;
    TMBNTC ia[] = { copies, copies, copies, copies };
    TMBNTC ib[] = { copies, copies, copies, copies };
    std::copy(ia, ia+4, ib);
    TC_ASSERT_EXPR(copies == 4);
    copies = 0;
    std::copy_backward(ia, ia+4, ib+4);
    TC_ASSERT_EXPR(copies == 4);

    copies = 0;
    std::copy(std::make_move_iterator(ia), std::make_move_iterator(ia+4), ib);
    TC_ASSERT_EXPR(copies == 0);
    std::copy_backward(std::make_move_iterator(ia), std::make_move_iterator(ia+4), ib+4);
    TC_ASSERT_EXPR(copies == 0);

    std::move(ia, ia+4, ib);
    TC_ASSERT_EXPR(copies == 0);
    std::move_backward(ia, ia+4, ib+4);
    TC_ASSERT_EXPR(copies == 0);

    return true;
}

struct TCBNTM {
    int *p;
    constexpr TCBNTM(int& moves) : p(&moves) {}
    constexpr TCBNTM(const TCBNTM&) = default;
    TEST_CONSTEXPR_CXX14 TCBNTM& operator=(TCBNTM&&) { ++*p; return *this; }
    TEST_CONSTEXPR_CXX14 TCBNTM& operator=(const TCBNTM&) = default;
};

TEST_CONSTEXPR_CXX20 bool
test_trivial_copyassign_but_no_trivial_moveassign()
{
    int moves = 0;
    TCBNTM ia[] = { moves, moves, moves, moves };
    TCBNTM ib[] = { moves, moves, moves, moves };
    std::move(ia, ia+4, ib);
    TC_ASSERT_EXPR(moves == 4);
    moves = 0;
    std::move_backward(ia, ia+4, ib+4);
    TC_ASSERT_EXPR(moves == 4);

    moves = 0;
    std::copy(std::make_move_iterator(ia), std::make_move_iterator(ia+4), ib);
    TC_ASSERT_EXPR(moves == 4);
    moves = 0;
    std::copy_backward(std::make_move_iterator(ia), std::make_move_iterator(ia+4), ib+4);
    TC_ASSERT_EXPR(moves == 4);

    moves = 0;
    std::copy(ia, ia+4, ib);
    TC_ASSERT_EXPR(moves == 0);
    std::copy_backward(ia, ia+4, ib+4);
    TC_ASSERT_EXPR(moves == 0);

    return true;
}

int tc_algorithms_alg_modifying_operations_alg_move_contiguous_trivial_optimization(void) {
    test_trivial_moveassign_but_no_trivial_copyassign();
    test_trivial_copyassign_but_no_trivial_moveassign();

#if TEST_STD_VER > 17
    static_assert(test_trivial_moveassign_but_no_trivial_copyassign());
    static_assert(test_trivial_copyassign_but_no_trivial_moveassign());
#endif

    return 0;
}
