//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "libcxx_tc_common.h"
#include "tc_language_support.hpp"
#include <stdio.h>

extern "C"
{
	int tc_language_support_main(void)
	{
		printf("\nStarting [LANGUAGE_SUPPORT] Tests\n");
		fflush(stdout);
		tc_libcxx_language_support_support_runtime_cstdbool();
		return 0;
	}
}
