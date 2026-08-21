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

#ifndef EMPLACEABLE_H
#define EMPLACEABLE_H

#include <utility>
#include "test_macros.h"

#if TEST_STD_VER >= 11

class Emplaceable
{
    Emplaceable(const Emplaceable&);
    Emplaceable& operator=(const Emplaceable&);

    int int_;
    double double_;
public:
    Emplaceable() : int_(0), double_(0) {}
    Emplaceable(int i, double d) : int_(i), double_(d) {}
    Emplaceable(Emplaceable&& x)
        : int_(x.int_), double_(x.double_)
            {x.int_ = 0; x.double_ = 0;}
    Emplaceable& operator=(Emplaceable&& x)
        {int_ = x.int_; x.int_ = 0;
         double_ = x.double_; x.double_ = 0;
         return *this;}

    bool operator==(const Emplaceable& x) const
        {return int_ == x.int_ && double_ == x.double_;}
    bool operator<(const Emplaceable& x) const
        {return int_ < x.int_ || (int_ == x.int_ && double_ < x.double_);}

    int get() const {return int_;}
};

namespace std {

template <>
struct hash<Emplaceable>
{
    typedef Emplaceable argument_type;
    typedef std::size_t result_type;

    std::size_t operator()(const Emplaceable& x) const {return x.get();}
};

}

#endif  // TEST_STD_VER >= 11
#endif  // EMPLACEABLE_H
