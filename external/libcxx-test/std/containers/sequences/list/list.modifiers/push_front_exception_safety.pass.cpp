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
// UNSUPPORTED: no-exceptions
// <list>

// void push_front(const value_type& x);

#include <list>
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

int tc_containers_sequences_list_list_modifiers_push_front_exception_safety(void) {
    CMyClass instance;
    std::list<CMyClass> vec;

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
