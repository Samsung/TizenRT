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
// test ratio typedef's

#include <ratio>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_ratio_typedefs(void) {
    static_assert(std::atto::num == 1 && std::atto::den == 1000000000000000000ULL, "");
    static_assert(std::femto::num == 1 && std::femto::den == 1000000000000000ULL, "");
    static_assert(std::pico::num == 1 && std::pico::den == 1000000000000ULL, "");
    static_assert(std::nano::num == 1 && std::nano::den == 1000000000ULL, "");
    static_assert(std::micro::num == 1 && std::micro::den == 1000000ULL, "");
    static_assert(std::milli::num == 1 && std::milli::den == 1000ULL, "");
    static_assert(std::centi::num == 1 && std::centi::den == 100ULL, "");
    static_assert(std::deci::num == 1 && std::deci::den == 10ULL, "");
    static_assert(std::deca::num == 10ULL && std::deca::den == 1, "");
    static_assert(std::hecto::num == 100ULL && std::hecto::den == 1, "");
    static_assert(std::kilo::num == 1000ULL && std::kilo::den == 1, "");
    static_assert(std::mega::num == 1000000ULL && std::mega::den == 1, "");
    static_assert(std::giga::num == 1000000000ULL && std::giga::den == 1, "");
    static_assert(std::tera::num == 1000000000000ULL && std::tera::den == 1, "");
    static_assert(std::peta::num == 1000000000000000ULL && std::peta::den == 1, "");
    static_assert(std::exa::num == 1000000000000000000ULL && std::exa::den == 1, "");

  return 0;
}
