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

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
// class scoped_allocator_adaptor
//     : public OuterAlloc
// {
// public:
//     typedef OuterAlloc outer_allocator_type;
//     typedef typename OuterTraits::size_type size_type;
//     typedef typename OuterTraits::difference_type difference_type;
//     typedef typename OuterTraits::pointer pointer;
//     typedef typename OuterTraits::const_pointer const_pointer;
//     typedef typename OuterTraits::void_pointer void_pointer;
//     typedef typename OuterTraits::const_void_pointer const_void_pointer;
// };

#include <scoped_allocator>
#include <type_traits>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_types(void) {
    static_assert((std::is_base_of<
        A1<int>,
        std::scoped_allocator_adaptor<A1<int>>
        >::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::outer_allocator_type,
        A1<int>>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::size_type,
        std::make_unsigned<std::ptrdiff_t>::type>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::difference_type,
        std::ptrdiff_t>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::pointer,
        int*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::const_pointer,
        const int*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::void_pointer,
        void*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A1<int>>::const_void_pointer,
        const void*>::value), "");

    static_assert((std::is_base_of<
        A2<int>,
        std::scoped_allocator_adaptor<A2<int>, A1<int>>
        >::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::outer_allocator_type,
        A2<int>>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::size_type,
        unsigned>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::difference_type,
        int>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::pointer,
        int*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::const_pointer,
        const int*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::void_pointer,
        void*>::value), "");

    static_assert((std::is_same<
        std::scoped_allocator_adaptor<A2<int>, A1<int>>::const_void_pointer,
        const void*>::value), "");

  return 0;
}
