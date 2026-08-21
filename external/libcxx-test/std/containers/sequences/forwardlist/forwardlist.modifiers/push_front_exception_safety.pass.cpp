//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions
// <forward_list>

// void push_front(const value_type& x);

#include <forward_list>
#include <cassert>
#include <exception>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Flag that makes the copy constructor for CMyClass throw an exception
static bool gCopyConstructorShouldThow = false;


class CMyClass {
    public: CMyClass();
    public: CMyClass(const CMyClass& iOther);
    public: ~CMyClass();

    private: int fMagicValue;

    private: static int kStartedConstructionMagicValue;
    private: static int kFinishedConstructionMagicValue;
};

// Value for fMagicValue when the constructor has started running, but not yet finished
int CMyClass::kStartedConstructionMagicValue = 0;
// Value for fMagicValue when the constructor has finished running
int CMyClass::kFinishedConstructionMagicValue = 12345;

CMyClass::CMyClass() :
    fMagicValue(kStartedConstructionMagicValue)
{
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::CMyClass(const CMyClass& /*iOther*/) :
    fMagicValue(kStartedConstructionMagicValue)
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

int tc_containers_sequences_forwardlist_forwardlist_modifiers_push_front_exception_safety(void) {
    CMyClass instance;
    std::forward_list<CMyClass> vec;

    vec.push_front(instance);

    gCopyConstructorShouldThow = true;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        vec.push_front(instance);
    }
    catch (...) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS

  return 0;
}
