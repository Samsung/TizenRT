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

#ifndef __EXTERNAL_LIBCXX_TEST_TC_THREAD_H
#define __EXTERNAL_LIBCXX_TEST_TC_THREAD_H
int tc_libcxx_thread_thread_lock_algorithm_lock(void);
int tc_libcxx_thread_thread_lock_algorithm_try_lock(void);
int tc_libcxx_thread_thread_lock_guard_adopt_lock(void);
int tc_libcxx_thread_thread_lock_guard_mutex(void);
int tc_libcxx_thread_thread_lock_guard_types(void);
int tc_libcxx_thread_thread_lock_unique_cons_default(void);
int tc_libcxx_thread_thread_lock_unique_cons_move_assign(void);
int tc_libcxx_thread_thread_lock_unique_cons_move_ctor(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex_adopt_lock(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex_defer_lock(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex_duration(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex_time_point(void);
int tc_libcxx_thread_thread_lock_unique_cons_mutex_try_to_lock(void);
int tc_libcxx_thread_thread_lock_unique_locking_lock(void);
int tc_libcxx_thread_thread_lock_unique_locking_try_lock(void);
int tc_libcxx_thread_thread_lock_unique_locking_try_lock_for(void);
int tc_libcxx_thread_thread_lock_unique_locking_try_lock_until(void);
int tc_libcxx_thread_thread_lock_unique_locking_unlock(void);
int tc_libcxx_thread_thread_lock_unique_mod_member_swap(void);
int tc_libcxx_thread_thread_lock_unique_mod_nonmember_swap(void);
int tc_libcxx_thread_thread_lock_unique_mod_release(void);
int tc_libcxx_thread_thread_lock_unique_obs_mutex(void);
int tc_libcxx_thread_thread_lock_unique_obs_op_bool(void);
int tc_libcxx_thread_thread_lock_unique_obs_owns_lock(void);
int tc_libcxx_thread_thread_lock_unique_types(void);
int tc_libcxx_thread_thread_lock_types(void);
int tc_libcxx_thread_thread_mutex_class_default(void);
int tc_libcxx_thread_thread_mutex_class_lock(void);
int tc_libcxx_thread_thread_mutex_class_try_lock(void);
int tc_libcxx_thread_thread_mutex_recursive_default(void);
int tc_libcxx_thread_thread_mutex_recursive_lock(void);
int tc_libcxx_thread_thread_mutex_recursive_try_lock(void);
int tc_libcxx_thread_thread_timedmutex_class_default(void);
int tc_libcxx_thread_thread_timedmutex_class_lock(void);
int tc_libcxx_thread_thread_timedmutex_class_try_lock(void);
int tc_libcxx_thread_thread_timedmutex_class_try_lock_for(void);
int tc_libcxx_thread_thread_timedmutex_class_try_lock_until(void);
int tc_libcxx_thread_thread_timedmutex_recursive_default(void);
int tc_libcxx_thread_thread_timedmutex_recursive_lock(void);
int tc_libcxx_thread_thread_timedmutex_recursive_try_lock(void);
int tc_libcxx_thread_thread_timedmutex_recursive_try_lock_for(void);
int tc_libcxx_thread_thread_timedmutex_recursive_try_lock_until(void);
int tc_libcxx_thread_thread_once_callonce_call_once(void);
int tc_libcxx_thread_thread_once_onceflag_default(void);
#endif /* __EXTERNAL_LIBCXX_TEST_TC_THREAD_H */
