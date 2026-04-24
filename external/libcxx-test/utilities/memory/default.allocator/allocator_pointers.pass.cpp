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

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// <memory>
//
// template <class Alloc>
// struct allocator_traits
// {
//     typedef Alloc                        allocator_type;
//     typedef typename allocator_type::value_type
//                                          value_type;
//
//     typedef Alloc::pointer | value_type* pointer;
//     typedef Alloc::const_pointer
//           | pointer_traits<pointer>::rebind<const value_type>
//                                          const_pointer;
//     typedef Alloc::void_pointer
//           | pointer_traits<pointer>::rebind<void>
//                                          void_pointer;
//     typedef Alloc::const_void_pointer
//           | pointer_traits<pointer>::rebind<const void>
//                                          const_void_pointer;

template <typename Alloc>
void test_pointer()
{
     typename std::allocator_traits<Alloc>::pointer        vp;
     typename std::allocator_traits<Alloc>::const_pointer cvp;

     ((void)vp); // Prevent unused warning
     ((void)cvp); // Prevent unused warning

     static_assert(std::is_same<bool, decltype( vp ==  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp !=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <=  vp)>::value, "");

     static_assert(std::is_same<bool, decltype( vp == cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp ==  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp != cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp !=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <=  vp)>::value, "");

     static_assert(std::is_same<bool, decltype(cvp == cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp != cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <= cvp)>::value, "");
}

template <typename Alloc>
void test_void_pointer()
{
     typename std::allocator_traits<Alloc>::void_pointer        vp;
     typename std::allocator_traits<Alloc>::const_void_pointer cvp;

     ((void)vp); // Prevent unused warning
     ((void)cvp); // Prevent unused warning

     static_assert(std::is_same<bool, decltype( vp ==  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp !=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <=  vp)>::value, "");

     static_assert(std::is_same<bool, decltype( vp == cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp ==  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp != cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp !=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp >= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >=  vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <   vp)>::value, "");
     static_assert(std::is_same<bool, decltype( vp <= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <=  vp)>::value, "");

     static_assert(std::is_same<bool, decltype(cvp == cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp != cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp >= cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <  cvp)>::value, "");
     static_assert(std::is_same<bool, decltype(cvp <= cvp)>::value, "");
}

struct Foo { int x; };

int tc_utilities_memory_default_allocator_allocator_pointers(void) {
    test_pointer<std::allocator<char>> ();
    test_pointer<std::allocator<int>> ();
    test_pointer<std::allocator<Foo>> ();

    test_void_pointer<std::allocator<char>> ();
    test_void_pointer<std::allocator<int>> ();
    test_void_pointer<std::allocator<Foo>> ();

  return 0;
}
