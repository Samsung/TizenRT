//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "libcxx_tc_common.h"
#include "tc_depr.hpp"
#include <stdio.h>

extern "C"
{
	int tc_depr_main(void)
	{
		printf("\nStarting [DEPR] Tests\n");
		fflush(stdout);
		tc_libcxx_depr_depr_c_headers_stdbool_h();
		return 0;
	}
}
