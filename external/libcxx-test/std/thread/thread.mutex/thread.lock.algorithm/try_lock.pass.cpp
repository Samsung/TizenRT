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

// template <class L1, class L2, class... L3>
//   int try_lock(L1&, L2&, L3&...);

#include <mutex>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

class L0
{
    bool locked_;

public:
    L0() : locked_(false) {}

    bool try_lock()
    {
        locked_ = true;
        return locked_;
    }

    void unlock() {locked_ = false;}

    bool locked() const {return locked_;}
};

class L1
{
    bool locked_;

public:
    L1() : locked_(false) {}

    bool try_lock()
    {
        locked_ = false;
        return locked_;
    }

    void unlock() {locked_ = false;}

    bool locked() const {return locked_;}
};

class L2
{
    bool locked_;

public:
    L2() : locked_(false) {}

    bool try_lock()
    {
        TEST_THROW(1);
        return locked_;
    }

    void unlock() {locked_ = false;}

    bool locked() const {return locked_;}
};

int tc_libcxx_thread_thread_lock_algorithm_try_lock(void)
{
    {
        L0 l0;
        L0 l1;
        TC_ASSERT_EXPR(std::try_lock(l0, l1) == -1);
        TC_ASSERT_EXPR(l0.locked());
        TC_ASSERT_EXPR(l1.locked());
    }
    {
        L0 l0;
        L1 l1;
        TC_ASSERT_EXPR(std::try_lock(l0, l1) == 1);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
    }
    {
        L1 l0;
        L0 l1;
        TC_ASSERT_EXPR(std::try_lock(l0, l1) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        L0 l0;
        L2 l1;
        try
        {
            (void)std::try_lock(l0, l1);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
        }
    }
    {
        L2 l0;
        L0 l1;
        try
        {
            (void)std::try_lock(l0, l1);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
        }
    }
#endif
#if TEST_STD_VER >= 11
    {
        L0 l0;
        L0 l1;
        L0 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == -1);
        TC_ASSERT_EXPR(l0.locked());
        TC_ASSERT_EXPR(l1.locked());
        TC_ASSERT_EXPR(l2.locked());
    }
    {
        L1 l0;
        L1 l1;
        L1 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        L2 l0;
        L2 l1;
        L2 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L0 l0;
        L1 l1;
        L2 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 1);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
#endif
    {
        L0 l0;
        L0 l1;
        L1 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 2);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L0 l0;
        L1 l1;
        L0 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 1);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L1 l0;
        L0 l1;
        L0 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        L0 l0;
        L0 l1;
        L2 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L0 l0;
        L2 l1;
        L0 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L0 l1;
        L0 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
#endif
    {
        L1 l0;
        L1 l1;
        L0 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L1 l0;
        L0 l1;
        L1 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L0 l0;
        L1 l1;
        L1 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 1);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        L1 l0;
        L1 l1;
        L2 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L1 l0;
        L2 l1;
        L1 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L2 l0;
        L1 l1;
        L1 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L2 l1;
        L0 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L0 l1;
        L2 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L0 l0;
        L2 l1;
        L2 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L2 l1;
        L1 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L1 l1;
        L2 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L1 l0;
        L2 l1;
        L2 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L0 l0;
        L2 l1;
        L1 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L1 l0;
        L0 l1;
        L2 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L1 l0;
        L2 l1;
        L0 l2;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
    }
    {
        L2 l0;
        L0 l1;
        L1 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
    {
        L2 l0;
        L1 l1;
        L0 l2;
        try
        {
            (void)std::try_lock(l0, l1, l2);
            TC_ASSERT_EXPR(false);
        }
        catch (int)
        {
            TC_ASSERT_EXPR(!l0.locked());
            TC_ASSERT_EXPR(!l1.locked());
            TC_ASSERT_EXPR(!l2.locked());
        }
    }
#endif  // TEST_HAS_NO_EXCEPTIONS
    {
        L0 l0;
        L0 l1;
        L0 l2;
        L0 l3;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2, l3) == -1);
        TC_ASSERT_EXPR(l0.locked());
        TC_ASSERT_EXPR(l1.locked());
        TC_ASSERT_EXPR(l2.locked());
        TC_ASSERT_EXPR(l3.locked());
    }
    {
        L1 l0;
        L0 l1;
        L0 l2;
        L0 l3;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2, l3) == 0);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
        TC_ASSERT_EXPR(!l3.locked());
    }
    {
        L0 l0;
        L1 l1;
        L0 l2;
        L0 l3;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2, l3) == 1);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
        TC_ASSERT_EXPR(!l3.locked());
    }
    {
        L0 l0;
        L0 l1;
        L1 l2;
        L0 l3;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2, l3) == 2);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
        TC_ASSERT_EXPR(!l3.locked());
    }
    {
        L0 l0;
        L0 l1;
        L0 l2;
        L1 l3;
        TC_ASSERT_EXPR(std::try_lock(l0, l1, l2, l3) == 3);
        TC_ASSERT_EXPR(!l0.locked());
        TC_ASSERT_EXPR(!l1.locked());
        TC_ASSERT_EXPR(!l2.locked());
        TC_ASSERT_EXPR(!l3.locked());
    }
#endif  // TEST_STD_VER >= 11
    TC_SUCCESS_RESULT();
    return 0;
}
