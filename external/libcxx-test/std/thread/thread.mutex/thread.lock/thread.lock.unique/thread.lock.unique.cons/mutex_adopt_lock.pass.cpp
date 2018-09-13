//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock(mutex_type& m, adopt_lock_t);

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

int tc_libcxx_thread_thread_lock_unique_cons_mutex_adopt_lock(void)
{
    {
    typedef std::mutex M;
    M m;
    m.lock();
    std::unique_lock<M> lk(m, std::adopt_lock);
    TC_ASSERT_EXPR(lk.mutex() == std::addressof(m));
    TC_ASSERT_EXPR(lk.owns_lock() == true);
    }
    {
    typedef nasty_mutex M;
    M m;
    m.lock();
    std::unique_lock<M> lk(m, std::adopt_lock);
    TC_ASSERT_EXPR(lk.mutex() == std::addressof(m));
    TC_ASSERT_EXPR(lk.owns_lock() == true);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
