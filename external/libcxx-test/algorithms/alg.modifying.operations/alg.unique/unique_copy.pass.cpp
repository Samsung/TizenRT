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

// template<InputIterator InIter, class OutIter>
//   requires OutputIterator<OutIter, RvalueOf<InIter::value_type>::type>
//         && EqualityComparable<InIter::value_type>
//         && HasAssign<InIter::value_type, InIter::reference>
//         && Constructible<InIter::value_type, InIter::reference>
//   constexpr OutIter        // constexpr after C++17
//   unique_copy(InIter first, InIter last, OutIter result);

#include <algorithm>
#include <cassert>

#include "MoveOnly.h"
#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct AssignableFromMoveOnly {
  AssignableFromMoveOnly(int i) : data(i) {}
  AssignableFromMoveOnly() : data(0) {}
  int data;
  AssignableFromMoveOnly& operator=(MoveOnly const& m) {
    data = m.get();
    return *this;
  }
  bool operator==(AssignableFromMoveOnly const& rhs) const { return data == rhs.data; }
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
          int ia[]       = {0, 1, 2, 2, 4};
          int ib[]       = {0, 0, 0, 0, 0};
    const int expected[] = {0, 1, 2, 4};

    auto it = std::unique_copy(std::begin(ia), std::end(ia), std::begin(ib));
    return it == (std::begin(ib) + std::size(expected))
        && *it == 0 // don't overwrite final value in output
        && std::equal(std::begin(ib), it, std::begin(expected), std::end(expected))
        ;
    }
#endif

template <class InIter, class OutIter>
void
test()
{
    const int ia[] = {0};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    int ja[sa] = {-1};
    OutIter r = std::unique_copy(InIter(ia), InIter(ia+sa), OutIter(ja));
    TC_ASSERT_EXPR(base(r) == ja + sa);
    TC_ASSERT_EXPR(ja[0] == 0);

    const int ib[] = {0, 1};
    const unsigned sb = sizeof(ib)/sizeof(ib[0]);
    int jb[sb] = {-1};
    r = std::unique_copy(InIter(ib), InIter(ib+sb), OutIter(jb));
    TC_ASSERT_EXPR(base(r) == jb + sb);
    TC_ASSERT_EXPR(jb[0] == 0);
    TC_ASSERT_EXPR(jb[1] == 1);

    const int ic[] = {0, 0};
    const unsigned sc = sizeof(ic)/sizeof(ic[0]);
    int jc[sc] = {-1};
    r = std::unique_copy(InIter(ic), InIter(ic+sc), OutIter(jc));
    TC_ASSERT_EXPR(base(r) == jc + 1);
    TC_ASSERT_EXPR(jc[0] == 0);

    const int id[] = {0, 0, 1};
    const unsigned sd = sizeof(id)/sizeof(id[0]);
    int jd[sd] = {-1};
    r = std::unique_copy(InIter(id), InIter(id+sd), OutIter(jd));
    TC_ASSERT_EXPR(base(r) == jd + 2);
    TC_ASSERT_EXPR(jd[0] == 0);
    TC_ASSERT_EXPR(jd[1] == 1);

    const int ie[] = {0, 0, 1, 0};
    const unsigned se = sizeof(ie)/sizeof(ie[0]);
    int je[se] = {-1};
    r = std::unique_copy(InIter(ie), InIter(ie+se), OutIter(je));
    TC_ASSERT_EXPR(base(r) == je + 3);
    TC_ASSERT_EXPR(je[0] == 0);
    TC_ASSERT_EXPR(je[1] == 1);
    TC_ASSERT_EXPR(je[2] == 0);

    const int ig[] = {0, 0, 1, 1};
    const unsigned sg = sizeof(ig)/sizeof(ig[0]);
    int jg[sg] = {-1};
    r = std::unique_copy(InIter(ig), InIter(ig+sg), OutIter(jg));
    TC_ASSERT_EXPR(base(r) == jg + 2);
    TC_ASSERT_EXPR(jg[0] == 0);
    TC_ASSERT_EXPR(jg[1] == 1);

    const int ih[] = {0, 1, 1};
    const unsigned sh = sizeof(ih)/sizeof(ih[0]);
    int jh[sh] = {-1};
    r = std::unique_copy(InIter(ih), InIter(ih+sh), OutIter(jh));
    TC_ASSERT_EXPR(base(r) == jh + 2);
    TC_ASSERT_EXPR(jh[0] == 0);
    TC_ASSERT_EXPR(jh[1] == 1);

    const int ii[] = {0, 1, 1, 1, 2, 2, 2};
    const unsigned si = sizeof(ii)/sizeof(ii[0]);
    int ji[si] = {-1};
    r = std::unique_copy(InIter(ii), InIter(ii+si), OutIter(ji));
    TC_ASSERT_EXPR(base(r) == ji + 3);
    TC_ASSERT_EXPR(ji[0] == 0);
    TC_ASSERT_EXPR(ji[1] == 1);
    TC_ASSERT_EXPR(ji[2] == 2);
}

int tc_algorithms_alg_modifying_operations_alg_unique_unique_copy(void) {
    test<cpp17_input_iterator<const int*>, cpp17_input_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, forward_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, bidirectional_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, random_access_iterator<int*> >();
    test<cpp17_input_iterator<const int*>, int*>();

    test<forward_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<forward_iterator<const int*>, forward_iterator<int*> >();
    test<forward_iterator<const int*>, bidirectional_iterator<int*> >();
    test<forward_iterator<const int*>, random_access_iterator<int*> >();
    test<forward_iterator<const int*>, int*>();

    test<bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<bidirectional_iterator<const int*>, forward_iterator<int*> >();
    test<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
    test<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
    test<bidirectional_iterator<const int*>, int*>();

    test<random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
    test<random_access_iterator<const int*>, forward_iterator<int*> >();
    test<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
    test<random_access_iterator<const int*>, random_access_iterator<int*> >();
    test<random_access_iterator<const int*>, int*>();

    test<const int*, cpp17_output_iterator<int*> >();
    test<const int*, forward_iterator<int*> >();
    test<const int*, bidirectional_iterator<int*> >();
    test<const int*, random_access_iterator<int*> >();
    test<const int*, int*>();

    // Move only inputs
    {
      MoveOnly in[5]                     = {1, 3, 3, 3, 1};
      AssignableFromMoveOnly out[3]      = {};
      auto result                        = std::unique_copy(in, in + 5, out);
      AssignableFromMoveOnly expected[3] = {1, 3, 1};
      TC_ASSERT_EXPR(std::equal(out, out + 3, expected));
      TC_ASSERT_EXPR(result == out + 3);
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
