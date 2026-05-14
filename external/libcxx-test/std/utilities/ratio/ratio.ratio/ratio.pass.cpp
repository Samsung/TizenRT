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
// test ratio:  The static data members num and den shall have the common
//    divisor of the absolute values of N and D:

#include <ratio>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <long long N, long long D, long long eN, long long eD>
void test()
{
    static_assert((std::ratio<N, D>::num == eN), "");
    static_assert((std::ratio<N, D>::den == eD), "");
}

int tc_utilities_ratio_ratio_ratio_ratio(void) {
    test<1, 1, 1, 1>();
    test<1, 10, 1, 10>();
    test<10, 10, 1, 1>();
    test<10, 1, 10, 1>();
    test<12, 4, 3, 1>();
    test<12, -4, -3, 1>();
    test<-12, 4, -3, 1>();
    test<-12, -4, 3, 1>();
    test<4, 12, 1, 3>();
    test<4, -12, -1, 3>();
    test<-4, 12, -1, 3>();
    test<-4, -12, 1, 3>();
    test<222, 333, 2, 3>();
    test<222, -333, -2, 3>();
    test<-222, 333, -2, 3>();
    test<-222, -333, 2, 3>();
    test<0x7FFFFFFFFFFFFFFFLL, 127, 72624976668147841LL, 1>();
    test<-0x7FFFFFFFFFFFFFFFLL, 127, -72624976668147841LL, 1>();
    test<0x7FFFFFFFFFFFFFFFLL, -127, -72624976668147841LL, 1>();
    test<-0x7FFFFFFFFFFFFFFFLL, -127, 72624976668147841LL, 1>();

  return 0;
}
