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

// struct once_flag;

// template<class Callable, class ...Args>
//   static int call_once(once_flag& flag, Callable&& func, Args&&... args);

#include <mutex>
#include <thread>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

typedef std::chrono::milliseconds ms;

std::once_flag flg0;

int init0_called = 0;

static int init0()
{
    std::this_thread::sleep_for(ms(250));
    ++init0_called;
    return 0;
}

static int f0()
{
    std::call_once(flg0, init0);
    return 0;
}

std::once_flag flg3;

int init3_called = 0;
int init3_completed = 0;

static int init3()
{
    ++init3_called;
    std::this_thread::sleep_for(ms(250));
    if (init3_called == 1)
        TEST_THROW(1);
    ++init3_completed;
    return 0;
}

static int f3()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        std::call_once(flg3, init3);
    }
    catch (...)
    {
    }
#endif
    return 0;
}

#if TEST_STD_VER >= 11

struct init1
{
    static int called;

    void operator()(int i) {called += i;}
};

int init1::called = 0;

std::once_flag flg1;

static int f1()
{
    std::call_once(flg1, init1(), 1);
    return 0;
}

struct init2
{
    static int called;

    void operator()(int i, int j) const {called += i + j;}
};

int init2::called = 0;

std::once_flag flg2;

static int f2()
{
    std::call_once(flg2, init2(), 2, 3);
    std::call_once(flg2, init2(), 4, 5);
    return 0;
}

#endif  // TEST_STD_VER >= 11

std::once_flag flg41;
std::once_flag flg42;

int init41_called = 0;
int init42_called = 0;

static int init42();

static int init41()
{
    std::this_thread::sleep_for(ms(250));
    ++init41_called;
    return 0;
}

static int init42()
{
    std::this_thread::sleep_for(ms(250));
    ++init42_called;
    return 0;
}

static int f41()
{
    std::call_once(flg41, init41);
    std::call_once(flg42, init42);
    return 0;
}

static int f42()
{
    std::call_once(flg42, init42);
    std::call_once(flg41, init41);
    return 0;
}

#if TEST_STD_VER >= 11

class MoveOnly
{
#if !defined(__clang__)
   // GCC 4.8 complains about the following being private
public:
    MoveOnly(const MoveOnly&)
    {
    }
#else
    MoveOnly(const MoveOnly&);
#endif
public:
    MoveOnly() {}
    MoveOnly(MoveOnly&&) {}

    void operator()(MoveOnly&&)
    {
    }
};

class NonCopyable
{
#if !defined(__clang__)
   // GCC 4.8 complains about the following being private
public:
    NonCopyable(const NonCopyable&)
    {
    }
#else
    NonCopyable(const NonCopyable&);
#endif
public:
    NonCopyable() {}

    void operator()(int&) {}
};

// reference qualifiers on functions are a C++11 extension
struct RefQual
{
    int lv_called, rv_called;

    RefQual() : lv_called(0), rv_called(0) {}

    void operator()() & { ++lv_called; }
    void operator()() && { ++rv_called; }
};

#endif // TEST_STD_VER >= 11

int tc_libcxx_thread_thread_once_callonce_call_once(void)
{
    // check basic functionality
    {
        std::thread t0(f0);
        std::thread t1(f0);
        t0.join();
        t1.join();
        TC_ASSERT_EXPR(init0_called == 1);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    // check basic exception safety
    {
        std::thread t0(f3);
        std::thread t1(f3);
        t0.join();
        t1.join();
        TC_ASSERT_EXPR(init3_called == 2);
        TC_ASSERT_EXPR(init3_completed == 1);
    }
#endif
    // check deadlock astatic intance
    {
        std::thread t0(f41);
        std::thread t1(f42);
        t0.join();
        t1.join();
        TC_ASSERT_EXPR(init41_called == 1);
        TC_ASSERT_EXPR(init42_called == 1);
    }
#if TEST_STD_VER >= 11
    // check functors with 1 arg
    {
        std::thread t0(f1);
        std::thread t1(f1);
        t0.join();
        t1.join();
        TC_ASSERT_EXPR(init1::called == 1);
    }
    // check functors with 2 args
    {
        std::thread t0(f2);
        std::thread t1(f2);
        t0.join();
        t1.join();
        TC_ASSERT_EXPR(init2::called == 5);
    }
    {
        std::once_flag f;
        std::call_once(f, MoveOnly(), MoveOnly());
    }
    // check LWG2442: call_once() shouldn't DECAY_COPY()
    {
        std::once_flag f;
        int i = 0;
        std::call_once(f, NonCopyable(), i);
    }
// reference qualifiers on functions are a C++11 extension
    {
        std::once_flag f1, f2;
        RefQual rq;
        std::call_once(f1, rq);
        TC_ASSERT_EXPR(rq.lv_called == 1);
        std::call_once(f2, std::move(rq));
        TC_ASSERT_EXPR(rq.rv_called == 1);
    }
#endif  // TEST_STD_VER >= 11
    TC_SUCCESS_RESULT();
    return 0;
}
