// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Unified IBM limits support header
// Routes to appropriate platform-specific implementation based on platform detection
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___SUPPORT_IBM_LIMITS_H
#define _LIBCPP___SUPPORT_IBM_LIMITS_H

// Platform detection and routing
#if defined(_AIX)
#  include <__support/ibm/aix/limits.h>
#elif defined(__MVS__)
// z/OS uses system limits.h, no special handling needed
#  include <limits.h>
#  include <math.h>
#  include <float.h>
#else
// Fallback - include AIX version if available
#  if defined(_AIX)
#    include <__support/ibm/aix/limits.h>
#  endif
#endif

#endif // _LIBCPP___SUPPORT_IBM_LIMITS_H