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

// unique_lock(mutex_type& m, defer_lock_t);

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

int tc_libcxx_thread_thread_lock_unique_cons_mutex_defer_lock(void)
{
    {
    typedef std::mutex M;
    M m;
    std::unique_lock<M> lk(m, std::defer_lock);
    TC_ASSERT_EXPR(lk.mutex() == std::addressof(m));
    TC_ASSERT_EXPR(lk.owns_lock() == false);
    }
    {
    typedef nasty_mutex M;
    M m;
    std::unique_lock<M> lk(m, std::defer_lock);
    TC_ASSERT_EXPR(lk.mutex() == std::addressof(m));
    TC_ASSERT_EXPR(lk.owns_lock() == false);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
