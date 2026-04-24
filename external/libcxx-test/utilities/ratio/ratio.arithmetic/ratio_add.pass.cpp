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
// test ratio_add

#include <ratio>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_ratio_ratio_arithmetic_ratio_add(void) {
    {
    typedef std::ratio<1, 1> R1;
    typedef std::ratio<1, 1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 2 && R::den == 1, "");
    }
    {
    typedef std::ratio<1, 2> R1;
    typedef std::ratio<1, 1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 3 && R::den == 2, "");
    }
    {
    typedef std::ratio<-1, 2> R1;
    typedef std::ratio<1, 1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 1 && R::den == 2, "");
    }
    {
    typedef std::ratio<1, -2> R1;
    typedef std::ratio<1, 1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 1 && R::den == 2, "");
    }
    {
    typedef std::ratio<1, 2> R1;
    typedef std::ratio<-1, 1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == -1 && R::den == 2, "");
    }
    {
    typedef std::ratio<1, 2> R1;
    typedef std::ratio<1, -1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == -1 && R::den == 2, "");
    }
    {
    typedef std::ratio<56987354, 467584654> R1;
    typedef std::ratio<544668, 22145> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 127970191639601LL && R::den == 5177331081415LL, "");
    }
    {
    typedef std::ratio<0> R1;
    typedef std::ratio<0> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 0 && R::den == 1, "");
    }
    {
    typedef std::ratio<1> R1;
    typedef std::ratio<0> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 1 && R::den == 1, "");
    }
    {
    typedef std::ratio<0> R1;
    typedef std::ratio<1> R2;
    typedef std::ratio_add<R1, R2>::type R;
    static_assert(R::num == 1 && R::den == 1, "");
    }

  return 0;
}
