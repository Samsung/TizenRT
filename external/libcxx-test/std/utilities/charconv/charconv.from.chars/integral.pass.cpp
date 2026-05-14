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
// UNSUPPORTED: c++03, c++11, c++14

// <charconv>

// constexpr from_chars_result from_chars(const char* first, const char* last,
//                                        Integral& value, int base = 10)

#include <charconv>
#include "test_macros.h"
#include "charconv_test_helpers.h"
#include "libcxx_tc_common.h"

template <typename T>
struct test_basics
{
    TEST_CONSTEXPR_CXX23 void operator()()
    {
        std::from_chars_result r;
        T x;

        {
            char s[] = "001x";

            // the expected form of the subject sequence is a sequence of
            // letters and digits representing an integer with the radix
            // specified by base (C11 7.22.1.4/3)
            r = std::from_chars(s, s + sizeof(s), x);
            TC_ASSERT_EXPR(r.ec == std::errc{});
            TC_ASSERT_EXPR(r.ptr == s + 3);
            TC_ASSERT_EXPR(x == 1);
        }

        {
            // The string has more characters than valid in an 128-bit value.
            char s[] = "0X7BAtSGHDkEIXZgQRfYChLpOzRnM ";

            // The letters from a (or A) through z (or Z) are ascribed the
            // values 10 through 35; (C11 7.22.1.4/3)
            r = std::from_chars(s, s + sizeof(s), x, 36);
            TC_ASSERT_EXPR(r.ec == std::errc::result_out_of_range);
            // The member ptr of the return value points to the first character
            // not matching the pattern
            TC_ASSERT_EXPR(r.ptr == s + sizeof(s) - 2);
            TC_ASSERT_EXPR(x == 1);

            // no "0x" or "0X" prefix shall appear if the value of base is 16
            r = std::from_chars(s, s + sizeof(s), x, 16);
            TC_ASSERT_EXPR(r.ec == std::errc{});
            TC_ASSERT_EXPR(r.ptr == s + 1);
            TC_ASSERT_EXPR(x == 0);

            // only letters and digits whose ascribed values are less than that
            // of base are permitted. (C11 7.22.1.4/3)
            r = std::from_chars(s + 2, s + sizeof(s), x, 12);
            // If the parsed value is not in the range representable by the type
            // of value,
            if (!fits_in<T>(1150))
            {
                // value is unmodified and
                TC_ASSERT_EXPR(x == 0);
                // the member ec of the return value is equal to
                // errc::result_out_of_range
                TC_ASSERT_EXPR(r.ec == std::errc::result_out_of_range);
            }
            else
            {
                // Otherwise, value is set to the parsed value,
                TC_ASSERT_EXPR(x == 1150);
                // and the member ec is value-initialized.
                TC_ASSERT_EXPR(r.ec == std::errc{});
            }
            TC_ASSERT_EXPR(r.ptr == s + 5);
        }
    }
};

template <typename T>
struct test_signed
{
    TEST_CONSTEXPR_CXX23 void operator()()
    {
        std::from_chars_result r;
        T x = 42;

        {
            // If the pattern allows for an optional sign,
            // but the string has no digit characters following the sign,
            char s[] = "- 9+12";
            r = std::from_chars(s, s + sizeof(s), x);
            // value is unmodified,
            TC_ASSERT_EXPR(x == 42);
            // no characters match the pattern.
            TC_ASSERT_EXPR(r.ptr == s);
            TC_ASSERT_EXPR(r.ec == std::errc::invalid_argument);
        }

        {
            char s[] = "9+12";
            r = std::from_chars(s, s + sizeof(s), x);
            TC_ASSERT_EXPR(r.ec == std::errc{});
            // The member ptr of the return value points to the first character
            // not matching the pattern,
            TC_ASSERT_EXPR(r.ptr == s + 1);
            TC_ASSERT_EXPR(x == 9);
        }

        {
            char s[] = "12";
            r = std::from_chars(s, s + 2, x);
            TC_ASSERT_EXPR(r.ec == std::errc{});
            // or has the value last if all characters match.
            TC_ASSERT_EXPR(r.ptr == s + 2);
            TC_ASSERT_EXPR(x == 12);
        }

        {
            // '-' is the only sign that may appear
            char s[] = "+30";
            // If no characters match the pattern,
            r = std::from_chars(s, s + sizeof(s), x);
            // value is unmodified,
            TC_ASSERT_EXPR(x == 12);
            // the member ptr of the return value is first and
            TC_ASSERT_EXPR(r.ptr == s);
            // the member ec is equal to errc::invalid_argument.
            TC_ASSERT_EXPR(r.ec == std::errc::invalid_argument);
        }
    }
};

TEST_CONSTEXPR_CXX23 bool test()
{
    run<test_basics>(integrals);
    run<test_signed>(all_signed);

    return true;
}

int tc_utilities_charconv_charconv_from_chars_integral(void) {
    test();
#if TEST_STD_VER > 20
    static_assert(test());
#endif

    return 0;
}
