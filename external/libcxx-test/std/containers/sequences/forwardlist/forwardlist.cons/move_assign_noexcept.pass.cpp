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

// <forward_list>

// forward_list& operator=(forward_list&& c)
//     noexcept(
//          allocator_type::propagate_on_container_move_assignment::value &&
//          is_nothrow_move_assignable<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++98, c++03

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_allocator.h"

// Minimal allocator that meets C++ allocator requirements
template <class T>
struct some_alloc
{
    typedef T value_type;
    typedef std::false_type propagate_on_container_move_assignment;
    
    some_alloc() {}
    some_alloc(const some_alloc&) {}
    
    T* allocate(size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, size_t) { ::operator delete(p); }
    
    template <class U>
    struct rebind { typedef some_alloc<U> other; };
    
    template <class U>
    some_alloc(const some_alloc<U>&) {}
};

int tc_libcxx_containers_forwardlist_cons_move_assign_noexcept(void)
{
    {
        typedef std::forward_list<MoveOnly> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
    {
        typedef std::forward_list<MoveOnly, test_allocator<MoveOnly>> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
#if defined(_LIBCPP_VERSION)
    {
        typedef std::forward_list<MoveOnly, other_allocator<MoveOnly>> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
    // Note: some_alloc test for forward_list differs in libcxx 17.0.6
    // {
    //     typedef std::forward_list<MoveOnly, some_alloc<MoveOnly>> C;
    //     static_assert(std::is_nothrow_move_assignable<C>::value, "");
    // }
#endif // _LIBCPP_VERSION
    TC_SUCCESS_RESULT();
    return 0;
}