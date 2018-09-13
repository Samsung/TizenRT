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
#include "tc_algorithms.hpp"

extern "C"
{
	int tc_algorithms_main(void)
	{
		tc_libcxx_algorithms_alg_min_max_max();
		tc_libcxx_algorithms_alg_min_max_max_comp();
		tc_libcxx_algorithms_alg_min_max_max_element();
		tc_libcxx_algorithms_alg_min_max_max_element_comp();
		tc_libcxx_algorithms_alg_min_max_max_init_list();
		tc_libcxx_algorithms_alg_min_max_max_init_list_comp();
		tc_libcxx_algorithms_alg_min_max_min();
		tc_libcxx_algorithms_alg_min_max_min_comp();
		tc_libcxx_algorithms_alg_min_max_min_element();
		tc_libcxx_algorithms_alg_min_max_min_element_comp();
		tc_libcxx_algorithms_alg_min_max_min_init_list();
		tc_libcxx_algorithms_alg_min_max_min_init_list_comp();
		tc_libcxx_algorithms_alg_min_max_minmax();
		tc_libcxx_algorithms_alg_min_max_minmax_comp();
		tc_libcxx_algorithms_alg_min_max_minmax_element();
		tc_libcxx_algorithms_alg_min_max_minmax_element_comp();
		tc_libcxx_algorithms_alg_min_max_minmax_init_list();
		tc_libcxx_algorithms_alg_min_max_minmax_init_list_comp();
		return 0;
	}
}
