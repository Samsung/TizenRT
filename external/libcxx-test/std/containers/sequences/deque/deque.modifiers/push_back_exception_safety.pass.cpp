//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions
// <deque>

// void push_back(const value_type& x);

#include <deque>
#include "test_macros.h"
#include "test_allocator.h"
#include <cassert>
#include "libcxx_tc_common.h"

// Flag that makes the copy constructor for CMyClass throw an exception
static bool gCopyConstructorShouldThow = false;

class CMyClass {
    public: CMyClass(int tag);
    public: CMyClass(const CMyClass& iOther);
    public: ~CMyClass();

    bool equal(const CMyClass &rhs) const
        { return fTag == rhs.fTag && fMagicValue == rhs.fMagicValue; }

    private:
        int fMagicValue;
        int fTag;

    private: static int kStartedConstructionMagicValue;
    private: static int kFinishedConstructionMagicValue;
};

// Value for fMagicValue when the constructor has started running, but not yet finished
int CMyClass::kStartedConstructionMagicValue = 0;
// Value for fMagicValue when the constructor has finished running
int CMyClass::kFinishedConstructionMagicValue = 12345;

CMyClass::CMyClass(int tag) :
    fMagicValue(kStartedConstructionMagicValue), fTag(tag)
{
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::CMyClass(const CMyClass& iOther) :
    fMagicValue(kStartedConstructionMagicValue), fTag(iOther.fTag)
{
    // If requested, throw an exception _before_ setting fMagicValue to kFinishedConstructionMagicValue
    if (gCopyConstructorShouldThow) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        throw std::exception();
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::~CMyClass() {
    // Only instances for which the constructor has finished running should be destructed
    TC_ASSERT_EXPR(fMagicValue == kFinishedConstructionMagicValue);
}

bool operator==(const CMyClass &lhs, const CMyClass &rhs) { return lhs.equal(rhs); }

int tc_containers_sequences_deque_deque_modifiers_push_back_exception_safety(void) {
    CMyClass instance(42);
    {
    std::deque<CMyClass> vec;

    vec.push_back(instance);
    std::deque<CMyClass> vec2(vec);

    gCopyConstructorShouldThow = true;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        vec.push_back(instance);
        TC_ASSERT_EXPR(false);
    }
    catch (...) {
        gCopyConstructorShouldThow = false;
        TC_ASSERT_EXPR(vec==vec2);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    {
    test_allocator_statistics alloc_stats;
    typedef std::deque<CMyClass, test_allocator<CMyClass> > C;
    C vec((test_allocator<CMyClass>(&alloc_stats)));
    C vec2(vec, test_allocator<CMyClass>(&alloc_stats));

    alloc_stats.throw_after = 1;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        vec.push_back(instance);
        TC_ASSERT_EXPR(false);
    }
    catch (...) {
        TC_ASSERT_EXPR(vec==vec2);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

  return 0;
}
