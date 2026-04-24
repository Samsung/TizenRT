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

// <scoped_allocator>

// template <class OtherAlloc, class ...InnerAlloc>
//   class scoped_allocator_adaptor

// template <class T, class ...Args>
// void scoped_allocator_adaptor::construct(T*, Args&&...)

#include <scoped_allocator>
#include <type_traits>
#include <utility>
#include <tuple>
#include <cassert>
#include <cstdlib>
#include "uses_alloc_types.h"
#include "controlled_allocators.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

// - If uses_allocator_v<T, inner_allocator_type> is false and
//   is_constructible_v<T, Args...> is true, calls
//   OUTERMOST_ALLOC_TRAITS(*this)::construct(
//      OUTERMOST (*this), p, std::forward<Args>(args)...).
void test_bullet_one() {
    using VoidAlloc2 = CountingAllocator<void, 2>;

    AllocController POuter;
    AllocController PInner;
    {
        using T = NotUsesAllocator<VoidAlloc2, 3>;
        using Outer = CountingAllocator<T, 1>;
        using Inner = CountingAllocator<T, 2>;
        using SA = std::scoped_allocator_adaptor<Outer, Inner>;
        static_assert(!std::uses_allocator<T, Outer>::value, "");
        static_assert(!std::uses_allocator<T, Inner>::value, "");
        T* ptr = (T*)::operator new(sizeof(T));
        Outer O(POuter);
        Inner I(PInner);
        SA A(O, I);
        int x = 42;
        int const& cx = x;
        A.construct(ptr, x, cx, std::move(x));
        TC_ASSERT_EXPR((checkConstruct<int&, int const&, int&&>(*ptr, UA_None)));
        TC_ASSERT_EXPR((POuter.checkConstruct<int&, int const&, int&&>(O, ptr)));
        A.destroy(ptr);
        ::operator delete((void*)ptr);
    }
    PInner.reset();
    POuter.reset();
}


// Otherwise, if uses_allocator_v<T, inner_allocator_type> is true and
// is_constructible_v<T, allocator_arg_t, inner_allocator_type&, Args...> is
// true, calls OUTERMOST_ALLOC_TRAITS(*this)::construct(OUTERMOST (*this), p,
//     allocator_arg, inner_allocator(), std::forward<Args>(args)...).
void test_bullet_two() {
    using VoidAlloc2 = CountingAllocator<void, 2>;

    AllocController POuter;
    AllocController PInner;
    {
        using T = UsesAllocatorV1<VoidAlloc2, 3>;
        using Outer = CountingAllocator<T, 1>;
        using Inner = CountingAllocator<T, 2>;
        using SA = std::scoped_allocator_adaptor<Outer, Inner>;
        static_assert(!std::uses_allocator<T, Outer>::value, "");
        static_assert(std::uses_allocator<T, Inner>::value, "");
        T* ptr = (T*)::operator new(sizeof(T));
        Outer O(POuter);
        Inner I(PInner);
        SA A(O, I);
        int x = 42;
        int const& cx = x;
        A.construct(ptr, x, cx, std::move(x));
        TC_ASSERT_EXPR((checkConstruct<int&, int const&, int&&>(*ptr, UA_AllocArg, I)));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((POuter.checkConstruct<std::allocator_arg_t&&,
                   const SA::inner_allocator_type&, int&, int const&, int&&>(O, ptr)));
#else
        TC_ASSERT_EXPR((POuter.checkConstruct<std::allocator_arg_t const&,
                   SA::inner_allocator_type&, int&, int const&, int&&>(O, ptr)));
#endif
        A.destroy(ptr);
        ::operator delete((void*)ptr);
    }
    PInner.reset();
    POuter.reset();
}

// Otherwise, if uses_allocator_v<T, inner_allocator_type> is true and
// is_constructible_v<T, Args..., inner_allocator_type&> is true, calls
// OUTERMOST_ALLOC_TRAITS(*this)::construct(OUTERMOST (*this), p,
//   std::forward<Args>(args)..., inner_allocator()).
void test_bullet_three() {
    using VoidAlloc2 = CountingAllocator<void, 2>;

    AllocController POuter;
    AllocController PInner;
    {
        using T = UsesAllocatorV2<VoidAlloc2, 3>;
        using Outer = CountingAllocator<T, 1>;
        using Inner = CountingAllocator<T, 2>;
        using SA = std::scoped_allocator_adaptor<Outer, Inner>;
        static_assert(!std::uses_allocator<T, Outer>::value, "");
        static_assert(std::uses_allocator<T, Inner>::value, "");
        T* ptr = (T*)::operator new(sizeof(T));
        Outer O(POuter);
        Inner I(PInner);
        SA A(O, I);
        int x = 42;
        int const& cx = x;
        A.construct(ptr, x, cx, std::move(x));
        TC_ASSERT_EXPR((checkConstruct<int&, int const&, int&&>(*ptr, UA_AllocLast, I)));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((POuter.checkConstruct<
                   int&, int const&, int&&,
                   const SA::inner_allocator_type&>(O, ptr)));
#else
        TC_ASSERT_EXPR((POuter.checkConstruct<
                   int&, int const&, int&&,
                   SA::inner_allocator_type&>(O, ptr)));
#endif
        A.destroy(ptr);
        ::operator delete((void*)ptr);
    }
    PInner.reset();
    POuter.reset();
}

int tc_utilities_allocator_adaptor_allocator_adaptor_members_construct_type(void) {
    test_bullet_one();
    test_bullet_two();
    test_bullet_three();

  return 0;
}
