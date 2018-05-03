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

#ifndef DEFAULTONLY_H
#define DEFAULTONLY_H

#include <cassert>

class DefaultOnly
{
    int data_;

    DefaultOnly(const DefaultOnly&);
    DefaultOnly& operator=(const DefaultOnly&);
public:
    static int count;

    DefaultOnly() : data_(-1) {++count;}
    ~DefaultOnly() {data_ = 0; --count;}

    friend bool operator==(const DefaultOnly& x, const DefaultOnly& y)
        {return x.data_ == y.data_;}
    friend bool operator< (const DefaultOnly& x, const DefaultOnly& y)
        {return x.data_ < y.data_;}
};


#endif  // DEFAULTONLY_H
