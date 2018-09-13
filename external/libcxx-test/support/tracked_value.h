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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef SUPPORT_TRACKED_VALUE_H
#define SUPPORT_TRACKED_VALUE_H

#include <cassert>

#include "test_macros.h"

struct TrackedValue {
    enum State { CONSTRUCTED, MOVED_FROM, DESTROYED };
    State state;

    TrackedValue() : state(State::CONSTRUCTED) {}

    TrackedValue(TrackedValue const& t) : state(State::CONSTRUCTED) {
        assert(t.state != State::MOVED_FROM && "copying a moved-from object");
        assert(t.state != State::DESTROYED  && "copying a destroyed object");
    }

#if TEST_STD_VER >= 11
    TrackedValue(TrackedValue&& t) : state(State::CONSTRUCTED) {
        assert(t.state != State::MOVED_FROM && "double moving from an object");
        assert(t.state != State::DESTROYED  && "moving from a destroyed object");
        t.state = State::MOVED_FROM;
    }
#endif

    TrackedValue& operator=(TrackedValue const& t) {
        assert(state != State::DESTROYED && "copy assigning into destroyed object");
        assert(t.state != State::MOVED_FROM && "copying a moved-from object");
        assert(t.state != State::DESTROYED  && "copying a destroyed object");
        state = t.state;
        return *this;
    }

#if TEST_STD_VER >= 11
    TrackedValue& operator=(TrackedValue&& t) {
        assert(state != State::DESTROYED && "move assigning into destroyed object");
        assert(t.state != State::MOVED_FROM && "double moving from an object");
        assert(t.state != State::DESTROYED  && "moving from a destroyed object");
        state = t.state;
        t.state = State::MOVED_FROM;
        return *this;
    }
#endif

    ~TrackedValue() {
        assert(state != State::DESTROYED && "double-destroying an object");
        state = State::DESTROYED;
    }
};

#endif // SUPPORT_TRACKED_VALUE_H
