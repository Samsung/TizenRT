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
// <vector>

// iterator       begin();
// iterator       end();
// const_iterator begin()  const;
// const_iterator end()    const;
// const_iterator cbegin() const;
// const_iterator cend()   const;

#include <vector>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    using IterRefT = std::iterator_traits<std::vector<bool>::iterator>::reference;
    ASSERT_SAME_TYPE(IterRefT, std::vector<bool>::reference);

    using ConstIterRefT = std::iterator_traits<std::vector<bool>::const_iterator>::reference;
#if !defined(_LIBCPP_VERSION) || defined(_LIBCPP_ABI_BITSET_VECTOR_BOOL_CONST_SUBSCRIPT_RETURN_BOOL)
    ASSERT_SAME_TYPE(ConstIterRefT, bool);
#else
    ASSERT_SAME_TYPE(ConstIterRefT, std::__bit_const_reference<std::vector<bool> >);
#endif
    {
        typedef bool T;
        typedef std::vector<T> C;
        C c;
        C::iterator i = c.begin();
        C::iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        const C c;
        C::const_iterator i = c.begin();
        C::const_iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        C c;
        C::const_iterator i = c.cbegin();
        C::const_iterator j = c.cend();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
        TC_ASSERT_EXPR(i == c.end());
    }
    {
        typedef bool T;
        typedef std::vector<T> C;
        C::iterator i;
        C::const_iterator j;
        (void) i;
        (void) j;
    }
#if TEST_STD_VER >= 11
    {
        typedef bool T;
        typedef std::vector<T, min_allocator<T>> C;
        C c;
        C::iterator i = c.begin();
        C::iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T, min_allocator<T>> C;
        const C c;
        C::const_iterator i = c.begin();
        C::const_iterator j = c.end();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
    }
    {
        typedef bool T;
        typedef std::vector<T, min_allocator<T>> C;
        C c;
        C::const_iterator i = c.cbegin();
        C::const_iterator j = c.cend();
        TC_ASSERT_EXPR(std::distance(i, j) == 0);
        TC_ASSERT_EXPR(i == j);
        TC_ASSERT_EXPR(i == c.end());
    }
    {
        typedef bool T;
        typedef std::vector<T, min_allocator<T>> C;
        C::iterator i;
        C::const_iterator j;
        (void) i;
        (void) j;
    }
#endif
#if TEST_STD_VER > 11
    { // N3644 testing
        std::vector<bool>::iterator ii1{}, ii2{};
        std::vector<bool>::iterator ii4 = ii1;
        std::vector<bool>::const_iterator cii{};
        TC_ASSERT_EXPR( ii1 == ii2 );
        TC_ASSERT_EXPR( ii1 == ii4 );

        TC_ASSERT_EXPR(!(ii1 != ii2 ));

        TC_ASSERT_EXPR( (ii1 == cii ));
        TC_ASSERT_EXPR( (cii == ii1 ));
        TC_ASSERT_EXPR(!(ii1 != cii ));
        TC_ASSERT_EXPR(!(cii != ii1 ));
        TC_ASSERT_EXPR(!(ii1 <  cii ));
        TC_ASSERT_EXPR(!(cii <  ii1 ));
        TC_ASSERT_EXPR( (ii1 <= cii ));
        TC_ASSERT_EXPR( (cii <= ii1 ));
        TC_ASSERT_EXPR(!(ii1 >  cii ));
        TC_ASSERT_EXPR(!(cii >  ii1 ));
        TC_ASSERT_EXPR( (ii1 >= cii ));
        TC_ASSERT_EXPR( (cii >= ii1 ));
        TC_ASSERT_EXPR(cii - ii1 == 0);
        TC_ASSERT_EXPR(ii1 - cii == 0);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_iterators(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
