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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

/* Its workaround implementation to provide to per task tls data.
 * current libc++ uses static storage which resides in global space
 * and it failes because of globals symbols cannot be reset upon task exit
 * and even though we tried to reset only seleted symbols during exit,
 * concurrent c++ taks using the same global symbols would crash.
 * Hence we store the tls data pointer in the task group on per task basis
 * Note : It will work for flat memory model. For protected & kernel
 * mode it will break the ABI(application interface)
 */
#if !defined(CONFIG_BUILD_PROTECTED) || !defined(CONFIG_BUILD_KERNEL)
/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
/* Fix ME: */
extern volatile dq_queue_t g_readytorun;
/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: __libcpp_get_tls_pointer
 *
 * Description:
 *    Returns tls data pointer
 ****************************************************************************/

void *__libcpp_get_tls_pointer(void)
{
	struct pthread_tcb_s *rtcb = (struct pthread_tcb_s *)((FAR struct tcb_s *)g_readytorun.head);
	struct task_group_s *group = rtcb->cmn.group;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	return group->__libcpp_tls_pointer;
}

/****************************************************************************
 * Name: __libcpp_set_tls_pointer
 *
 * Description:
 *    set tls data pointer
 ****************************************************************************/
void __libcpp_set_tls_pointer(void *__p)
{
	struct pthread_tcb_s *rtcb = (struct pthread_tcb_s *)((FAR struct tcb_s *)g_readytorun.head);
	struct task_group_s *group = rtcb->cmn.group;

	DEBUGASSERT(group && (rtcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_PTHREAD);

	group->__libcpp_tls_pointer = __p;
}
#endif	/* !defined(CONFIG_BUILD_PROTECTED) || !defined(CONFIG_BUILD_KERNEL) */
