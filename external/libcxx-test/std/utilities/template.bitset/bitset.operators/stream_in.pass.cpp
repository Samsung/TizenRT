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
// UNSUPPORTED: no-localization

// test:

// template <class charT, class traits, size_t N>
// basic_istream<charT, traits>&
// operator>>(basic_istream<charT, traits>& is, bitset<N>& x);

#include <bitset>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_template_bitset_bitset_operators_stream_in(void) {
    {
        std::istringstream in("01011010");
        std::bitset<8> b;
        in >> b;
        TC_ASSERT_EXPR(b.to_ulong() == 0x5A);
    }
    {
        // Make sure that input-streaming an empty bitset does not cause the
        // failbit to be set (LWG 3199).
        std::istringstream in("01011010");
        std::bitset<0> b;
        in >> b;
        TC_ASSERT_EXPR(b.to_string() == "");
        TC_ASSERT_EXPR(!in.bad());
        TC_ASSERT_EXPR(!in.fail());
        TC_ASSERT_EXPR(!in.eof());
        TC_ASSERT_EXPR(in.good());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::stringbuf sb;
        std::istream is(&sb);
        is.exceptions(std::ios::failbit);

        bool threw = false;
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            std::bitset<8> b;
            is >> b;
        } catch (std::ios::failure const&) {
            threw = true;
        }
#endif // _LIBCPP_NO_EXCEPTIONS

        TC_ASSERT_EXPR(!is.bad());
        TC_ASSERT_EXPR(is.fail());
        TC_ASSERT_EXPR(is.eof());
        TC_ASSERT_EXPR(threw);
    }
    {
        std::stringbuf sb;
        std::istream is(&sb);
        is.exceptions(std::ios::eofbit);

        bool threw = false;
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            std::bitset<8> b;
            is >> b;
        } catch (std::ios::failure const&) {
            threw = true;
        }
#endif // _LIBCPP_NO_EXCEPTIONS

        TC_ASSERT_EXPR(!is.bad());
        TC_ASSERT_EXPR(is.fail());
        TC_ASSERT_EXPR(is.eof());
        TC_ASSERT_EXPR(threw);
    }
#endif // TEST_HAS_NO_EXCEPTIONS

    return 0;
}
