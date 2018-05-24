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
#include "tc_thread.hpp"

extern "C"
{
	int tc_thread_main(void)
	{
		tc_libcxx_thread_thread_lock_algorithm_lock();
		tc_libcxx_thread_thread_lock_algorithm_try_lock();
		tc_libcxx_thread_thread_lock_guard_adopt_lock();
		tc_libcxx_thread_thread_lock_guard_mutex();
		tc_libcxx_thread_thread_lock_guard_types();
		tc_libcxx_thread_thread_lock_unique_cons_default();
		tc_libcxx_thread_thread_lock_unique_cons_move_assign();
		tc_libcxx_thread_thread_lock_unique_cons_move_ctor();
		tc_libcxx_thread_thread_lock_unique_cons_mutex();
		tc_libcxx_thread_thread_lock_unique_cons_mutex_adopt_lock();
		tc_libcxx_thread_thread_lock_unique_cons_mutex_defer_lock();
		tc_libcxx_thread_thread_lock_unique_cons_mutex_duration();
		tc_libcxx_thread_thread_lock_unique_cons_mutex_time_point();
		tc_libcxx_thread_thread_lock_unique_locking_lock();
		tc_libcxx_thread_thread_lock_unique_locking_try_lock();
		tc_libcxx_thread_thread_lock_unique_locking_try_lock_for();
		tc_libcxx_thread_thread_lock_unique_locking_try_lock_until();
		tc_libcxx_thread_thread_lock_unique_locking_unlock();
		tc_libcxx_thread_thread_lock_unique_mod_member_swap();
		tc_libcxx_thread_thread_lock_unique_mod_nonmember_swap();
		tc_libcxx_thread_thread_lock_unique_mod_release();
		tc_libcxx_thread_thread_lock_unique_obs_mutex();
		tc_libcxx_thread_thread_lock_unique_obs_op_bool();
		tc_libcxx_thread_thread_lock_unique_obs_owns_lock();
		tc_libcxx_thread_thread_lock_unique_types();
		tc_libcxx_thread_thread_lock_types();
		tc_libcxx_thread_thread_mutex_class_default();
		tc_libcxx_thread_thread_mutex_class_lock();
		tc_libcxx_thread_thread_mutex_recursive_default();
		tc_libcxx_thread_thread_mutex_recursive_lock();
		tc_libcxx_thread_thread_timedmutex_class_default();
		tc_libcxx_thread_thread_timedmutex_class_lock();
		tc_libcxx_thread_thread_timedmutex_class_try_lock_for();
		tc_libcxx_thread_thread_timedmutex_class_try_lock_until();
		tc_libcxx_thread_thread_timedmutex_recursive_default();
		tc_libcxx_thread_thread_timedmutex_recursive_lock();
		tc_libcxx_thread_thread_timedmutex_recursive_try_lock_for();
		tc_libcxx_thread_thread_timedmutex_recursive_try_lock_until();
		tc_libcxx_thread_thread_once_callonce_call_once();
		tc_libcxx_thread_thread_once_onceflag_default();
		return 0;
	}
}
