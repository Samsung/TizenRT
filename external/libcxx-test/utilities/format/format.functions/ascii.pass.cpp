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
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// Force unicode to be disabled.
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_HAS_NO_UNICODE

// TODO FMT This test should not require std::to_chars(floating-point)
// XFAIL: availability-fp_to_chars-missing

// <format>

// Tests Unicode is ignored and handled as ASCII.

#include <format>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Note the comment annotations are from the Unicode test
static void test_char() {
  //*** 1-byte code points ***
  TC_ASSERT_EXPR(std::format("{:*^3}", " ") == "* *");
  TC_ASSERT_EXPR(std::format("{:*^3}", "~") == "*~*");

  //*** 2-byte code points ***
  TC_ASSERT_EXPR(std::format("{:*^4}", "\u00a1") == "*\u00a1*"); // INVERTED EXCLAMATION MARK
  TC_ASSERT_EXPR(std::format("{:*^4}", "\u07ff") == "*\u07ff*"); // NKO TAMAN SIGN

  //*** 3-byte code points ***
  TC_ASSERT_EXPR(std::format("{:*^5}", "\u0800") == "*\u0800*"); // SAMARITAN LETTER ALAF
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufffd") == "*\ufffd*"); // REPLACEMENT CHARACTER

  // 2 column ranges
  TC_ASSERT_EXPR(std::format("{:*^5}", "\u1100") == "*\u1100*"); // HANGUL CHOSEONG KIYEOK
  TC_ASSERT_EXPR(std::format("{:*^5}", "\u115f") == "*\u115f*"); // HANGUL CHOSEONG FILLER

  TC_ASSERT_EXPR(std::format("{:*^5}", "\u2329") == "*\u2329*"); // LEFT-POINTING ANGLE BRACKET
  TC_ASSERT_EXPR(std::format("{:*^5}", "\u232a") == "*\u232a*"); // RIGHT-POINTING ANGLE BRACKET

  TC_ASSERT_EXPR(std::format("{:*^5}", "\u2e80") == "*\u2e80*"); // CJK RADICAL REPEAT
  TC_ASSERT_EXPR(std::format("{:*^5}", "\u303e") == "*\u303e*"); // IDEOGRAPHIC VARIATION INDICATOR

  TC_ASSERT_EXPR(std::format("{:*^5}", "\u3040") == "*\u3040*"); // U+3041 HIRAGANA LETTER SMALL A
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ua4cf") == "*\ua4cf*"); // U+A4D0 LISU LETTER BA

  TC_ASSERT_EXPR(std::format("{:*^5}", "\uac00") == "*\uac00*"); // <Hangul Syllable, First>
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ud7a3") == "*\ud7a3*"); // Hangul Syllable Hih

  TC_ASSERT_EXPR(std::format("{:*^5}", "\uf900") == "*\uf900*"); // CJK COMPATIBILITY IDEOGRAPH-F900
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufaff") == "*\ufaff*"); // U+FB00 LATIN SMALL LIGATURE FF

  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufe10") == "*\ufe10*"); // PRESENTATION FORM FOR VERTICAL COMMA
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufe19") == "*\ufe19*"); // PRESENTATION FORM FOR VERTICAL HORIZONTAL ELLIPSIS

  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufe30") == "*\ufe30*"); // PRESENTATION FORM FOR VERTICAL TWO DOT LEADER
  TC_ASSERT_EXPR(std::format("{:*^5}", "\ufe6f") == "*\ufe6f*"); // U+FE70 ARABIC FATHATAN ISOLATED FORM

  TC_ASSERT_EXPR(std::format("{:*^5}", "\uff00") == "*\uff00*"); // U+FF01 FULLWIDTH EXCLAMATION MARK
  TC_ASSERT_EXPR(std::format("{:*^5}", "\uff60") == "*\uff60*"); // FULLWIDTH RIGHT WHITE PARENTHESIS

  TC_ASSERT_EXPR(std::format("{:*^5}", "\uffe0") == "*\uffe0*"); // FULLWIDTH CENT SIGN
  TC_ASSERT_EXPR(std::format("{:*^5}", "\uffe6") == "*\uffe6*"); // FULLWIDTH WON SIGN

  //*** 4-byte code points ***
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U00010000") == "*\U00010000*"); // LINEAR B SYLLABLE B008 A
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0010FFFF") == "*\U0010FFFF*"); // Undefined Character

  // 2 column ranges
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0001f300") == "*\U0001f300*"); // CYCLONE
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0001f64f") == "*\U0001f64f*"); // PERSON WITH FOLDED HANDS
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0001f900") == "*\U0001f900*"); // CIRCLED CROSS FORMEE WITH FOUR DOTS
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0001f9ff") == "*\U0001f9ff*"); // NAZAR AMULET
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U00020000") == "*\U00020000*"); // <CJK Ideograph Extension B, First>
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0002fffd") == "*\U0002fffd*"); // Undefined Character
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U00030000") == "*\U00030000*"); // <CJK Ideograph Extension G, First>
  TC_ASSERT_EXPR(std::format("{:*^6}", "\U0003fffd") == "*\U0003fffd*"); // Undefined Character
}

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
// Note the comment annotations are from the Unicode test
static void test_wchar_t() {
  if constexpr (sizeof(wchar_t) == 2) {
    // TODO FMT Add these tests.
  } else {
    //*** 1-byte code points ***
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L" ") == L"* *");
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"~") == L"*~*");

    //*** 2-byte code points ***
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u00a1") == L"*\u00a1*"); // INVERTED EXCLAMATION MARK
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u07ff") == L"*\u07ff*"); // NKO TAMAN SIGN

    //*** 3-byte code points ***
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u0800") == L"*\u0800*"); // SAMARITAN LETTER ALAF
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufffd") == L"*\ufffd*"); // REPLACEMENT CHARACTER

    // 2 column ranges
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u1100") == L"*\u1100*"); // HANGUL CHOSEONG KIYEOK
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u115f") == L"*\u115f*"); // HANGUL CHOSEONG FILLER

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u2329") == L"*\u2329*"); // LEFT-POINTING ANGLE BRACKET
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u232a") == L"*\u232a*"); // RIGHT-POINTING ANGLE BRACKET

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u2e80") == L"*\u2e80*"); // CJK RADICAL REPEAT
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u303e") == L"*\u303e*"); // IDEOGRAPHIC VARIATION INDICATOR

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\u3040") == L"*\u3040*"); // U+3041 HIRAGANA LETTER SMALL A
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ua4cf") == L"*\ua4cf*"); // U+A4D0 LISU LETTER BA

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uac00") == L"*\uac00*"); // <Hangul Syllable, First>
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ud7a3") == L"*\ud7a3*"); // Hangul Syllable Hih

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uf900") == L"*\uf900*"); // CJK COMPATIBILITY IDEOGRAPH-F900
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufaff") == L"*\ufaff*"); // U+FB00 LATIN SMALL LIGATURE FF

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufe10") == L"*\ufe10*"); // PRESENTATION FORM FOR VERTICAL COMMA
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufe19") == L"*\ufe19*"); // PRESENTATION FORM FOR VERTICAL HORIZONTAL ELLIPSIS

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufe30") == L"*\ufe30*"); // PRESENTATION FORM FOR VERTICAL TWO DOT LEADER
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\ufe6f") == L"*\ufe6f*"); // U+FE70 ARABIC FATHATAN ISOLATED FORM

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uff00") == L"*\uff00*"); // U+FF01 FULLWIDTH EXCLAMATION MARK
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uff60") == L"*\uff60*"); // FULLWIDTH RIGHT WHITE PARENTHESIS

    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uffe0") == L"*\uffe0*"); // FULLWIDTH CENT SIGN
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\uffe6") == L"*\uffe6*"); // FULLWIDTH WON SIGN

    //*** 4-byte code points ***
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U00010000") == L"*\U00010000*"); // LINEAR B SYLLABLE B008 A
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0010FFFF") == L"*\U0010FFFF*"); // Undefined Character

    // 2 column ranges
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0001f300") == L"*\U0001f300*"); // CYCLONE
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0001f64f") == L"*\U0001f64f*"); // PERSON WITH FOLDED HANDS
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0001f900") == L"*\U0001f900*"); // CIRCLED CROSS FORMEE WITH FOUR DOTS
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0001f9ff") == L"*\U0001f9ff*"); // NAZAR AMULET
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U00020000") == L"*\U00020000*"); // <CJK Ideograph Extension B, First>
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0002fffd") == L"*\U0002fffd*"); // Undefined Character
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U00030000") == L"*\U00030000*"); // <CJK Ideograph Extension G, First>
    TC_ASSERT_EXPR(std::format(L"{:*^3}", L"\U0003fffd") == L"*\U0003fffd*"); // Undefined Character
  }
}
#endif

int tc_utilities_format_format_functions_ascii(void) {
  test_char();

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test_wchar_t();
#endif

  return 0;
}
