//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <scoped_allocator>

// template <class OtherAlloc, class ...InnerAlloc>
//   class scoped_allocator_adaptor

// template <class U1, class U2>
// void scoped_allocator_adaptor::construct(pair<U1, U2>*)

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


void test_no_inner_alloc()
{
    using VoidAlloc = CountingAllocator<void>;
    AllocController P;
    {
        using T = UsesAllocatorV1<VoidAlloc, 0>;
        using U = UsesAllocatorV2<VoidAlloc, 0>;
        using Pair = std::pair<T, U>;
        using Alloc = CountingAllocator<Pair>;
        using SA = std::scoped_allocator_adaptor<Alloc>;
        static_assert(std::uses_allocator<T, CountingAllocator<T> >::value, "");
        Pair * ptr = (Pair*)std::malloc(sizeof(Pair));
        TC_ASSERT_EXPR(ptr);
        Alloc CA(P);
        SA A(CA);
        A.construct(ptr);
        TC_ASSERT_EXPR(checkConstruct<>(ptr->first, UA_AllocArg, CA));
        TC_ASSERT_EXPR(checkConstruct<>(ptr->second, UA_AllocLast, CA));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((P.checkConstruct<std::piecewise_construct_t&&,
                                 std::tuple<std::allocator_arg_t, const SA&>&&,
                                 std::tuple<const SA&>&&
              >(CA, ptr)));
#else
        TC_ASSERT_EXPR((P.checkConstruct<std::piecewise_construct_t const&,
                                 std::tuple<std::allocator_arg_t, SA&>&&,
                                 std::tuple<SA&>&&
              >(CA, ptr)));
#endif
        A.destroy(ptr);
        std::free(ptr);

    }
    P.reset();
    {
        using T = UsesAllocatorV3<VoidAlloc, 0>;
        using U = NotUsesAllocator<VoidAlloc, 0>;
        using Pair = std::pair<T, U>;
        using Alloc = CountingAllocator<Pair>;
        using SA = std::scoped_allocator_adaptor<Alloc>;
        static_assert(std::uses_allocator<T, CountingAllocator<T> >::value, "");
        Pair * ptr = (Pair*)std::malloc(sizeof(Pair));
        TC_ASSERT_EXPR(ptr);
        Alloc CA(P);
        SA A(CA);
        A.construct(ptr);
        TC_ASSERT_EXPR(checkConstruct<>(ptr->first, UA_AllocArg, CA));
        TC_ASSERT_EXPR(checkConstruct<>(ptr->second, UA_None));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((P.checkConstruct<std::piecewise_construct_t&&,
                                 std::tuple<std::allocator_arg_t, const SA&>&&,
                                 std::tuple<>&&
                   >(CA, ptr)));
#else
        TC_ASSERT_EXPR((P.checkConstruct<std::piecewise_construct_t const&,
                                 std::tuple<std::allocator_arg_t, SA&>&&,
                                 std::tuple<>&&
                   >(CA, ptr)));
#endif
        A.destroy(ptr);
        std::free(ptr);
    }
}

void test_with_inner_alloc()
{
    using VoidAlloc2 = CountingAllocator<void, 2>;

    AllocController POuter;
    AllocController PInner;
    {
        using T = UsesAllocatorV1<VoidAlloc2, 0>;
        using U = UsesAllocatorV2<VoidAlloc2, 0>;
        using Pair = std::pair<T, U>;
        using Outer = CountingAllocator<Pair, 1>;
        using Inner = CountingAllocator<Pair, 2>;
        using SA = std::scoped_allocator_adaptor<Outer, Inner>;
        using SAInner = std::scoped_allocator_adaptor<Inner>;
        static_assert(!std::uses_allocator<T, Outer>::value, "");
        static_assert(std::uses_allocator<T, Inner>::value, "");
        Pair * ptr = (Pair*)std::malloc(sizeof(Pair));
        TC_ASSERT_EXPR(ptr);
        Outer O(POuter);
        Inner I(PInner);
        SA A(O, I);
        A.construct(ptr);
        TC_ASSERT_EXPR(checkConstruct<>(ptr->first, UA_AllocArg, I));
        TC_ASSERT_EXPR(checkConstruct<>(ptr->second, UA_AllocLast));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((POuter.checkConstruct<std::piecewise_construct_t&&,
                                 std::tuple<std::allocator_arg_t, const SAInner&>&&,
                                 std::tuple<const SAInner&>&&
              >(O, ptr)));
#else
        TC_ASSERT_EXPR((POuter.checkConstruct<std::piecewise_construct_t const&,
                                 std::tuple<std::allocator_arg_t, SAInner&>&&,
                                 std::tuple<SAInner&>&&
              >(O, ptr)));
#endif
        A.destroy(ptr);
        std::free(ptr);
    }
    PInner.reset();
    POuter.reset();
    {
        using T = UsesAllocatorV3<VoidAlloc2, 0>;
        using U = NotUsesAllocator<VoidAlloc2, 0>;
        using Pair = std::pair<T, U>;
        using Outer = CountingAllocator<Pair, 1>;
        using Inner = CountingAllocator<Pair, 2>;
        using SA = std::scoped_allocator_adaptor<Outer, Inner>;
        using SAInner = std::scoped_allocator_adaptor<Inner>;
        static_assert(!std::uses_allocator<T, Outer>::value, "");
        static_assert(std::uses_allocator<T, Inner>::value, "");
        Pair * ptr = (Pair*)std::malloc(sizeof(Pair));
        TC_ASSERT_EXPR(ptr);
        Outer O(POuter);
        Inner I(PInner);
        SA A(O, I);
        A.construct(ptr);
        TC_ASSERT_EXPR(checkConstruct<>(ptr->first, UA_AllocArg, I));
        TC_ASSERT_EXPR(checkConstruct<>(ptr->second, UA_None));
#if TEST_STD_VER >= 20
        TC_ASSERT_EXPR((POuter.checkConstruct<std::piecewise_construct_t&&,
                                 std::tuple<std::allocator_arg_t, const SAInner&>&&,
                                 std::tuple<>&&
              >(O, ptr)));
#else
        TC_ASSERT_EXPR((POuter.checkConstruct<std::piecewise_construct_t const&,
                                 std::tuple<std::allocator_arg_t, SAInner&>&&,
                                 std::tuple<>&&
              >(O, ptr)));
#endif
        A.destroy(ptr);
        std::free(ptr);
    }
}
int tc_utilities_allocator_adaptor_allocator_adaptor_members_construct_pair(void) {
    test_no_inner_alloc();
    test_with_inner_alloc();

  return 0;
}
