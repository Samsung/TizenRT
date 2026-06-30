// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Unified IBM platform support header
// Routes to appropriate platform-specific implementation based on platform detection
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___SUPPORT_IBM_XLOCALE_H
#define _LIBCPP___SUPPORT_IBM_XLOCALE_H

// Platform detection and routing
#if defined(_AIX)
#  include <__support/ibm/aix/xlocale.h>
#elif defined(__MVS__)
#  include <__support/ibm/zos/xlocale.h>
#else
// Fallback for other IBM platforms - include both for maximum compatibility
#  if defined(_AIX)
#    include <__support/ibm/aix/xlocale.h>
#  endif
#  if defined(__MVS__)
#    include <__support/ibm/zos/xlocale.h>
#  endif
#endif

#endif // _LIBCPP___SUPPORT_IBM_XLOCALE_H