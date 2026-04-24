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
// <deque>

// Test nested types and default template args:

// template <class T, class Allocator = allocator<T> >
// class deque
// {
// public:
//     typedef T                                        value_type;
//     typedef Allocator                                allocator_type;
//     typedef typename allocator_type::reference       reference;
//     typedef typename allocator_type::const_reference const_reference;
//     typedef implementation-defined                   iterator;
//     typedef implementation-defined                   const_iterator;
//     typedef typename allocator_type::size_type       size_type;
//     typedef typename allocator_type::difference_type difference_type;
//     typedef typename allocator_type::pointer         pointer;
//     typedef typename allocator_type::const_pointer   const_pointer;
//     typedef std::reverse_iterator<iterator>          reverse_iterator;
//     typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
// };

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_ENABLE_CXX20_REMOVED_ALLOCATOR_MEMBERS
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <deque>
#include <iterator>
#include <type_traits>

#include "test_macros.h"
#include "test_allocator.h"
#include "../../Copyable.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class T, class Allocator>
void
test()
{
    typedef std::deque<T, Allocator> C;

    static_assert((std::is_same<typename C::value_type, T>::value), "");
    static_assert((std::is_same<typename C::value_type, typename Allocator::value_type>::value), "");
    static_assert((std::is_same<typename C::allocator_type, Allocator>::value), "");
    static_assert((std::is_same<typename C::size_type, typename Allocator::size_type>::value), "");
    static_assert((std::is_same<typename C::difference_type, typename Allocator::difference_type>::value), "");
    static_assert((std::is_same<typename C::reference, typename Allocator::reference>::value), "");
    static_assert((std::is_same<typename C::const_reference, typename Allocator::const_reference>::value), "");
    static_assert((std::is_same<typename C::pointer, typename Allocator::pointer>::value), "");
    static_assert((std::is_same<typename C::const_pointer, typename Allocator::const_pointer>::value), "");
    static_assert((std::is_same<
        typename std::iterator_traits<typename C::iterator>::iterator_category,
        std::random_access_iterator_tag>::value), "");
    static_assert((std::is_same<
        typename std::iterator_traits<typename C::const_iterator>::iterator_category,
        std::random_access_iterator_tag>::value), "");
    static_assert((std::is_same<
        typename C::reverse_iterator,
        std::reverse_iterator<typename C::iterator> >::value), "");
    static_assert((std::is_same<
        typename C::const_reverse_iterator,
        std::reverse_iterator<typename C::const_iterator> >::value), "");
    static_assert((std::is_signed<typename C::difference_type>::value), "");
    static_assert((std::is_unsigned<typename C::size_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::iterator>::difference_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::const_iterator>::difference_type>::value), "");
}

int tc_containers_sequences_deque_types(void) {
    test<int, test_allocator<int> >();
    test<int*, std::allocator<int*> >();
    test<Copyable, test_allocator<Copyable> >();
    static_assert((std::is_same<std::deque<char>::allocator_type,
                                std::allocator<char> >::value), "");

#if TEST_STD_VER >= 11
    {
        typedef std::deque<short, min_allocator<short>> C;
        static_assert((std::is_same<C::value_type, short>::value), "");
        static_assert((std::is_same<C::allocator_type, min_allocator<C::value_type> >::value), "");
        static_assert((std::is_same<C::reference, C::value_type&>::value), "");
        static_assert((std::is_same<C::const_reference, const C::value_type&>::value), "");
        static_assert((std::is_same<C::pointer, min_pointer<C::value_type>>::value), "");
        static_assert((std::is_same<C::const_pointer, min_pointer<const C::value_type>>::value), "");
//  min_allocator doesn't have a size_type, so one gets synthesized
        static_assert((std::is_same<C::size_type, std::make_unsigned<C::difference_type>::type>::value), "");
        static_assert((std::is_same<C::difference_type, std::ptrdiff_t>::value), "");

        static_assert((std::is_signed<typename C::difference_type>::value), "");
        static_assert((std::is_unsigned<typename C::size_type>::value), "");
        static_assert((std::is_same<typename C::difference_type,
            typename std::iterator_traits<typename C::iterator>::difference_type>::value), "");
        static_assert((std::is_same<typename C::difference_type,
            typename std::iterator_traits<typename C::const_iterator>::difference_type>::value), "");
    }
#endif

  return 0;
}
