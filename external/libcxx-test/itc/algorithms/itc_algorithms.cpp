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

#include "libcxx_tc_common.h"
#include "itc_algorithms.hpp"

extern "C"
{
	int itc_algorithms_main(void)
	{
		itc_libcxx_algorithms_alg_min_max_max();
		itc_libcxx_algorithms_alg_min_max_max_comp();
		itc_libcxx_algorithms_alg_min_max_max_init_list();

		return 0;
	}
}
