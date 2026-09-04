//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <__config>

#if _LIBCPP_STD_VER >= 17

#include <variant>

namespace std {

const char* bad_variant_access::what() const noexcept {
  return "bad_variant_access";
}

}  // namespace std

#endif // _LIBCPP_STD_VER >= 17
