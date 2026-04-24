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
// <algorithm>

// template<ShuffleIterator Iter>
//   Iter
//   rotate(Iter first, Iter middle, Iter last);

#include <algorithm>
#include <cassert>
#include <memory>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class Iter>
TEST_CONSTEXPR_CXX17 bool
test()
{
    int ia[] = {0};
    const int sa = static_cast<int>(sizeof(ia)/sizeof(ia[0]));
    Iter r = std::rotate(Iter(ia), Iter(ia), Iter(ia));
    TC_ASSERT_EXPR(base(r) == ia);
    TC_ASSERT_EXPR(ia[0] == 0);
    r = std::rotate(Iter(ia), Iter(ia), Iter(ia+sa));
    TC_ASSERT_EXPR(base(r) == ia+sa);
    TC_ASSERT_EXPR(ia[0] == 0);
    r = std::rotate(Iter(ia), Iter(ia+sa), Iter(ia+sa));
    TC_ASSERT_EXPR(base(r) == ia);
    TC_ASSERT_EXPR(ia[0] == 0);

    int ib[] = {0, 1};
    const int sb = static_cast<int>(sizeof(ib)/sizeof(ib[0]));
    r = std::rotate(Iter(ib), Iter(ib), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib+sb);
    TC_ASSERT_EXPR(ib[0] == 0);
    TC_ASSERT_EXPR(ib[1] == 1);
    r = std::rotate(Iter(ib), Iter(ib+1), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib+1);
    TC_ASSERT_EXPR(ib[0] == 1);
    TC_ASSERT_EXPR(ib[1] == 0);
    r = std::rotate(Iter(ib), Iter(ib+sb), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib);
    TC_ASSERT_EXPR(ib[0] == 1);
    TC_ASSERT_EXPR(ib[1] == 0);

    int ic[] = {0, 1, 2};
    const int sc = static_cast<int>(sizeof(ic)/sizeof(ic[0]));
    r = std::rotate(Iter(ic), Iter(ic), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+sc);
    TC_ASSERT_EXPR(ic[0] == 0);
    TC_ASSERT_EXPR(ic[1] == 1);
    TC_ASSERT_EXPR(ic[2] == 2);
    r = std::rotate(Iter(ic), Iter(ic+1), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+2);
    TC_ASSERT_EXPR(ic[0] == 1);
    TC_ASSERT_EXPR(ic[1] == 2);
    TC_ASSERT_EXPR(ic[2] == 0);
    r = std::rotate(Iter(ic), Iter(ic+2), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+1);
    TC_ASSERT_EXPR(ic[0] == 0);
    TC_ASSERT_EXPR(ic[1] == 1);
    TC_ASSERT_EXPR(ic[2] == 2);
    r = std::rotate(Iter(ic), Iter(ic+sc), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic);
    TC_ASSERT_EXPR(ic[0] == 0);
    TC_ASSERT_EXPR(ic[1] == 1);
    TC_ASSERT_EXPR(ic[2] == 2);

    int id[] = {0, 1, 2, 3};
    const int sd = static_cast<int>(sizeof(id)/sizeof(id[0]));
    r = std::rotate(Iter(id), Iter(id), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+sd);
    TC_ASSERT_EXPR(id[0] == 0);
    TC_ASSERT_EXPR(id[1] == 1);
    TC_ASSERT_EXPR(id[2] == 2);
    TC_ASSERT_EXPR(id[3] == 3);
    r = std::rotate(Iter(id), Iter(id+1), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+3);
    TC_ASSERT_EXPR(id[0] == 1);
    TC_ASSERT_EXPR(id[1] == 2);
    TC_ASSERT_EXPR(id[2] == 3);
    TC_ASSERT_EXPR(id[3] == 0);
    r = std::rotate(Iter(id), Iter(id+2), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+2);
    TC_ASSERT_EXPR(id[0] == 3);
    TC_ASSERT_EXPR(id[1] == 0);
    TC_ASSERT_EXPR(id[2] == 1);
    TC_ASSERT_EXPR(id[3] == 2);
    r = std::rotate(Iter(id), Iter(id+3), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+1);
    TC_ASSERT_EXPR(id[0] == 2);
    TC_ASSERT_EXPR(id[1] == 3);
    TC_ASSERT_EXPR(id[2] == 0);
    TC_ASSERT_EXPR(id[3] == 1);
    r = std::rotate(Iter(id), Iter(id+sd), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id);
    TC_ASSERT_EXPR(id[0] == 2);
    TC_ASSERT_EXPR(id[1] == 3);
    TC_ASSERT_EXPR(id[2] == 0);
    TC_ASSERT_EXPR(id[3] == 1);

    int ie[] = {0, 1, 2, 3, 4};
    const int se = static_cast<int>(sizeof(ie)/sizeof(ie[0]));
    r = std::rotate(Iter(ie), Iter(ie), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+se);
    TC_ASSERT_EXPR(ie[0] == 0);
    TC_ASSERT_EXPR(ie[1] == 1);
    TC_ASSERT_EXPR(ie[2] == 2);
    TC_ASSERT_EXPR(ie[3] == 3);
    TC_ASSERT_EXPR(ie[4] == 4);
    r = std::rotate(Iter(ie), Iter(ie+1), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+4);
    TC_ASSERT_EXPR(ie[0] == 1);
    TC_ASSERT_EXPR(ie[1] == 2);
    TC_ASSERT_EXPR(ie[2] == 3);
    TC_ASSERT_EXPR(ie[3] == 4);
    TC_ASSERT_EXPR(ie[4] == 0);
    r = std::rotate(Iter(ie), Iter(ie+2), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+3);
    TC_ASSERT_EXPR(ie[0] == 3);
    TC_ASSERT_EXPR(ie[1] == 4);
    TC_ASSERT_EXPR(ie[2] == 0);
    TC_ASSERT_EXPR(ie[3] == 1);
    TC_ASSERT_EXPR(ie[4] == 2);
    r = std::rotate(Iter(ie), Iter(ie+3), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+2);
    TC_ASSERT_EXPR(ie[0] == 1);
    TC_ASSERT_EXPR(ie[1] == 2);
    TC_ASSERT_EXPR(ie[2] == 3);
    TC_ASSERT_EXPR(ie[3] == 4);
    TC_ASSERT_EXPR(ie[4] == 0);
    r = std::rotate(Iter(ie), Iter(ie+4), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+1);
    TC_ASSERT_EXPR(ie[0] == 0);
    TC_ASSERT_EXPR(ie[1] == 1);
    TC_ASSERT_EXPR(ie[2] == 2);
    TC_ASSERT_EXPR(ie[3] == 3);
    TC_ASSERT_EXPR(ie[4] == 4);
    r = std::rotate(Iter(ie), Iter(ie+se), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie);
    TC_ASSERT_EXPR(ie[0] == 0);
    TC_ASSERT_EXPR(ie[1] == 1);
    TC_ASSERT_EXPR(ie[2] == 2);
    TC_ASSERT_EXPR(ie[3] == 3);
    TC_ASSERT_EXPR(ie[4] == 4);

    int ig[] = {0, 1, 2, 3, 4, 5};
    const int sg = static_cast<int>(sizeof(ig)/sizeof(ig[0]));
    r = std::rotate(Iter(ig), Iter(ig), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+sg);
    TC_ASSERT_EXPR(ig[0] == 0);
    TC_ASSERT_EXPR(ig[1] == 1);
    TC_ASSERT_EXPR(ig[2] == 2);
    TC_ASSERT_EXPR(ig[3] == 3);
    TC_ASSERT_EXPR(ig[4] == 4);
    TC_ASSERT_EXPR(ig[5] == 5);
    r = std::rotate(Iter(ig), Iter(ig+1), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+5);
    TC_ASSERT_EXPR(ig[0] == 1);
    TC_ASSERT_EXPR(ig[1] == 2);
    TC_ASSERT_EXPR(ig[2] == 3);
    TC_ASSERT_EXPR(ig[3] == 4);
    TC_ASSERT_EXPR(ig[4] == 5);
    TC_ASSERT_EXPR(ig[5] == 0);
    r = std::rotate(Iter(ig), Iter(ig+2), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+4);
    TC_ASSERT_EXPR(ig[0] == 3);
    TC_ASSERT_EXPR(ig[1] == 4);
    TC_ASSERT_EXPR(ig[2] == 5);
    TC_ASSERT_EXPR(ig[3] == 0);
    TC_ASSERT_EXPR(ig[4] == 1);
    TC_ASSERT_EXPR(ig[5] == 2);
    r = std::rotate(Iter(ig), Iter(ig+3), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+3);
    TC_ASSERT_EXPR(ig[0] == 0);
    TC_ASSERT_EXPR(ig[1] == 1);
    TC_ASSERT_EXPR(ig[2] == 2);
    TC_ASSERT_EXPR(ig[3] == 3);
    TC_ASSERT_EXPR(ig[4] == 4);
    TC_ASSERT_EXPR(ig[5] == 5);
    r = std::rotate(Iter(ig), Iter(ig+4), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+2);
    TC_ASSERT_EXPR(ig[0] == 4);
    TC_ASSERT_EXPR(ig[1] == 5);
    TC_ASSERT_EXPR(ig[2] == 0);
    TC_ASSERT_EXPR(ig[3] == 1);
    TC_ASSERT_EXPR(ig[4] == 2);
    TC_ASSERT_EXPR(ig[5] == 3);
    r = std::rotate(Iter(ig), Iter(ig+5), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+1);
    TC_ASSERT_EXPR(ig[0] == 3);
    TC_ASSERT_EXPR(ig[1] == 4);
    TC_ASSERT_EXPR(ig[2] == 5);
    TC_ASSERT_EXPR(ig[3] == 0);
    TC_ASSERT_EXPR(ig[4] == 1);
    TC_ASSERT_EXPR(ig[5] == 2);
    r = std::rotate(Iter(ig), Iter(ig+sg), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig);
    TC_ASSERT_EXPR(ig[0] == 3);
    TC_ASSERT_EXPR(ig[1] == 4);
    TC_ASSERT_EXPR(ig[2] == 5);
    TC_ASSERT_EXPR(ig[3] == 0);
    TC_ASSERT_EXPR(ig[4] == 1);
    TC_ASSERT_EXPR(ig[5] == 2);

    return true;
}

#if TEST_STD_VER >= 11

template <class Iter>
void
test1()
{
    std::unique_ptr<int> ia[1];
    const int sa = static_cast<int>(sizeof(ia)/sizeof(ia[0]));
    for (int i = 0; i < sa; ++i)
        ia[i].reset(new int(i));
    Iter r = std::rotate(Iter(ia), Iter(ia), Iter(ia));
    TC_ASSERT_EXPR(base(r) == ia);
    TC_ASSERT_EXPR(*ia[0] == 0);
    r = std::rotate(Iter(ia), Iter(ia), Iter(ia+sa));
    TC_ASSERT_EXPR(base(r) == ia+sa);
    TC_ASSERT_EXPR(*ia[0] == 0);
    r = std::rotate(Iter(ia), Iter(ia+sa), Iter(ia+sa));
    TC_ASSERT_EXPR(base(r) == ia);
    TC_ASSERT_EXPR(*ia[0] == 0);

    std::unique_ptr<int> ib[2];
    const int sb = static_cast<int>(sizeof(ib)/sizeof(ib[0]));
    for (int i = 0; i < sb; ++i)
        ib[i].reset(new int(i));
    r = std::rotate(Iter(ib), Iter(ib), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib+sb);
    TC_ASSERT_EXPR(*ib[0] == 0);
    TC_ASSERT_EXPR(*ib[1] == 1);
    r = std::rotate(Iter(ib), Iter(ib+1), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib+1);
    TC_ASSERT_EXPR(*ib[0] == 1);
    TC_ASSERT_EXPR(*ib[1] == 0);
    r = std::rotate(Iter(ib), Iter(ib+sb), Iter(ib+sb));
    TC_ASSERT_EXPR(base(r) == ib);
    TC_ASSERT_EXPR(*ib[0] == 1);
    TC_ASSERT_EXPR(*ib[1] == 0);

    std::unique_ptr<int> ic[3];
    const int sc = static_cast<int>(sizeof(ic)/sizeof(ic[0]));
    for (int i = 0; i < sc; ++i)
        ic[i].reset(new int(i));
    r = std::rotate(Iter(ic), Iter(ic), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+sc);
    TC_ASSERT_EXPR(*ic[0] == 0);
    TC_ASSERT_EXPR(*ic[1] == 1);
    TC_ASSERT_EXPR(*ic[2] == 2);
    r = std::rotate(Iter(ic), Iter(ic+1), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+2);
    TC_ASSERT_EXPR(*ic[0] == 1);
    TC_ASSERT_EXPR(*ic[1] == 2);
    TC_ASSERT_EXPR(*ic[2] == 0);
    r = std::rotate(Iter(ic), Iter(ic+2), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic+1);
    TC_ASSERT_EXPR(*ic[0] == 0);
    TC_ASSERT_EXPR(*ic[1] == 1);
    TC_ASSERT_EXPR(*ic[2] == 2);
    r = std::rotate(Iter(ic), Iter(ic+sc), Iter(ic+sc));
    TC_ASSERT_EXPR(base(r) == ic);
    TC_ASSERT_EXPR(*ic[0] == 0);
    TC_ASSERT_EXPR(*ic[1] == 1);
    TC_ASSERT_EXPR(*ic[2] == 2);

    std::unique_ptr<int> id[4];
    const int sd = static_cast<int>(sizeof(id)/sizeof(id[0]));
    for (int i = 0; i < sd; ++i)
        id[i].reset(new int(i));
    r = std::rotate(Iter(id), Iter(id), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+sd);
    TC_ASSERT_EXPR(*id[0] == 0);
    TC_ASSERT_EXPR(*id[1] == 1);
    TC_ASSERT_EXPR(*id[2] == 2);
    TC_ASSERT_EXPR(*id[3] == 3);
    r = std::rotate(Iter(id), Iter(id+1), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+3);
    TC_ASSERT_EXPR(*id[0] == 1);
    TC_ASSERT_EXPR(*id[1] == 2);
    TC_ASSERT_EXPR(*id[2] == 3);
    TC_ASSERT_EXPR(*id[3] == 0);
    r = std::rotate(Iter(id), Iter(id+2), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+2);
    TC_ASSERT_EXPR(*id[0] == 3);
    TC_ASSERT_EXPR(*id[1] == 0);
    TC_ASSERT_EXPR(*id[2] == 1);
    TC_ASSERT_EXPR(*id[3] == 2);
    r = std::rotate(Iter(id), Iter(id+3), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id+1);
    TC_ASSERT_EXPR(*id[0] == 2);
    TC_ASSERT_EXPR(*id[1] == 3);
    TC_ASSERT_EXPR(*id[2] == 0);
    TC_ASSERT_EXPR(*id[3] == 1);
    r = std::rotate(Iter(id), Iter(id+sd), Iter(id+sd));
    TC_ASSERT_EXPR(base(r) == id);
    TC_ASSERT_EXPR(*id[0] == 2);
    TC_ASSERT_EXPR(*id[1] == 3);
    TC_ASSERT_EXPR(*id[2] == 0);
    TC_ASSERT_EXPR(*id[3] == 1);

    std::unique_ptr<int> ie[5];
    const int se = static_cast<int>(sizeof(ie)/sizeof(ie[0]));
    for (int i = 0; i < se; ++i)
        ie[i].reset(new int(i));
    r = std::rotate(Iter(ie), Iter(ie), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+se);
    TC_ASSERT_EXPR(*ie[0] == 0);
    TC_ASSERT_EXPR(*ie[1] == 1);
    TC_ASSERT_EXPR(*ie[2] == 2);
    TC_ASSERT_EXPR(*ie[3] == 3);
    TC_ASSERT_EXPR(*ie[4] == 4);
    r = std::rotate(Iter(ie), Iter(ie+1), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+4);
    TC_ASSERT_EXPR(*ie[0] == 1);
    TC_ASSERT_EXPR(*ie[1] == 2);
    TC_ASSERT_EXPR(*ie[2] == 3);
    TC_ASSERT_EXPR(*ie[3] == 4);
    TC_ASSERT_EXPR(*ie[4] == 0);
    r = std::rotate(Iter(ie), Iter(ie+2), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+3);
    TC_ASSERT_EXPR(*ie[0] == 3);
    TC_ASSERT_EXPR(*ie[1] == 4);
    TC_ASSERT_EXPR(*ie[2] == 0);
    TC_ASSERT_EXPR(*ie[3] == 1);
    TC_ASSERT_EXPR(*ie[4] == 2);
    r = std::rotate(Iter(ie), Iter(ie+3), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+2);
    TC_ASSERT_EXPR(*ie[0] == 1);
    TC_ASSERT_EXPR(*ie[1] == 2);
    TC_ASSERT_EXPR(*ie[2] == 3);
    TC_ASSERT_EXPR(*ie[3] == 4);
    TC_ASSERT_EXPR(*ie[4] == 0);
    r = std::rotate(Iter(ie), Iter(ie+4), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie+1);
    TC_ASSERT_EXPR(*ie[0] == 0);
    TC_ASSERT_EXPR(*ie[1] == 1);
    TC_ASSERT_EXPR(*ie[2] == 2);
    TC_ASSERT_EXPR(*ie[3] == 3);
    TC_ASSERT_EXPR(*ie[4] == 4);
    r = std::rotate(Iter(ie), Iter(ie+se), Iter(ie+se));
    TC_ASSERT_EXPR(base(r) == ie);
    TC_ASSERT_EXPR(*ie[0] == 0);
    TC_ASSERT_EXPR(*ie[1] == 1);
    TC_ASSERT_EXPR(*ie[2] == 2);
    TC_ASSERT_EXPR(*ie[3] == 3);
    TC_ASSERT_EXPR(*ie[4] == 4);

    std::unique_ptr<int> ig[6];
    const int sg = static_cast<int>(sizeof(ig)/sizeof(ig[0]));
    for (int i = 0; i < sg; ++i)
        ig[i].reset(new int(i));
    r = std::rotate(Iter(ig), Iter(ig), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+sg);
    TC_ASSERT_EXPR(*ig[0] == 0);
    TC_ASSERT_EXPR(*ig[1] == 1);
    TC_ASSERT_EXPR(*ig[2] == 2);
    TC_ASSERT_EXPR(*ig[3] == 3);
    TC_ASSERT_EXPR(*ig[4] == 4);
    TC_ASSERT_EXPR(*ig[5] == 5);
    r = std::rotate(Iter(ig), Iter(ig+1), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+5);
    TC_ASSERT_EXPR(*ig[0] == 1);
    TC_ASSERT_EXPR(*ig[1] == 2);
    TC_ASSERT_EXPR(*ig[2] == 3);
    TC_ASSERT_EXPR(*ig[3] == 4);
    TC_ASSERT_EXPR(*ig[4] == 5);
    TC_ASSERT_EXPR(*ig[5] == 0);
    r = std::rotate(Iter(ig), Iter(ig+2), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+4);
    TC_ASSERT_EXPR(*ig[0] == 3);
    TC_ASSERT_EXPR(*ig[1] == 4);
    TC_ASSERT_EXPR(*ig[2] == 5);
    TC_ASSERT_EXPR(*ig[3] == 0);
    TC_ASSERT_EXPR(*ig[4] == 1);
    TC_ASSERT_EXPR(*ig[5] == 2);
    r = std::rotate(Iter(ig), Iter(ig+3), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+3);
    TC_ASSERT_EXPR(*ig[0] == 0);
    TC_ASSERT_EXPR(*ig[1] == 1);
    TC_ASSERT_EXPR(*ig[2] == 2);
    TC_ASSERT_EXPR(*ig[3] == 3);
    TC_ASSERT_EXPR(*ig[4] == 4);
    TC_ASSERT_EXPR(*ig[5] == 5);
    r = std::rotate(Iter(ig), Iter(ig+4), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+2);
    TC_ASSERT_EXPR(*ig[0] == 4);
    TC_ASSERT_EXPR(*ig[1] == 5);
    TC_ASSERT_EXPR(*ig[2] == 0);
    TC_ASSERT_EXPR(*ig[3] == 1);
    TC_ASSERT_EXPR(*ig[4] == 2);
    TC_ASSERT_EXPR(*ig[5] == 3);
    r = std::rotate(Iter(ig), Iter(ig+5), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig+1);
    TC_ASSERT_EXPR(*ig[0] == 3);
    TC_ASSERT_EXPR(*ig[1] == 4);
    TC_ASSERT_EXPR(*ig[2] == 5);
    TC_ASSERT_EXPR(*ig[3] == 0);
    TC_ASSERT_EXPR(*ig[4] == 1);
    TC_ASSERT_EXPR(*ig[5] == 2);
    r = std::rotate(Iter(ig), Iter(ig+sg), Iter(ig+sg));
    TC_ASSERT_EXPR(base(r) == ig);
    TC_ASSERT_EXPR(*ig[0] == 3);
    TC_ASSERT_EXPR(*ig[1] == 4);
    TC_ASSERT_EXPR(*ig[2] == 5);
    TC_ASSERT_EXPR(*ig[3] == 0);
    TC_ASSERT_EXPR(*ig[4] == 1);
    TC_ASSERT_EXPR(*ig[5] == 2);
}

#endif // TEST_STD_VER >= 11

int tc_algorithms_alg_modifying_operations_alg_rotate_rotate(void) {
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 11

    test1<forward_iterator<std::unique_ptr<int>*> >();
    test1<bidirectional_iterator<std::unique_ptr<int>*> >();
    test1<random_access_iterator<std::unique_ptr<int>*> >();
    test1<std::unique_ptr<int>*>();

#endif

#if TEST_STD_VER > 17
    static_assert(test<forward_iterator<int*> >());
    static_assert(test<bidirectional_iterator<int*> >());
    static_assert(test<random_access_iterator<int*> >());
    static_assert(test<int*>());
#endif // TEST_STD_VER > 17

  return 0;
}
