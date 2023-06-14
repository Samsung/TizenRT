/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_TASK_H_
#define __OSDEP_SERVICE_TASK_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
  * @brief  Definitions returned by xTaskGetSchedulerState().
  */

#define OS_SCHEDULER_NOT_STARTED	0
#define OS_SCHEDULER_RUNNING		1
#define OS_SCHEDULER_SUSPENDED		2

#define TASK_PRORITY_LOW  				1
#define TASK_PRORITY_MIDDLE   			2
#define TASK_PRORITY_HIGH    			3
#define TASK_PRORITY_SUPER    			4


typedef void			    *_thread_hdl_;
typedef void			    thread_return;
typedef void			    *thread_context;

typedef thread_return(*thread_func_t)(thread_context context);

#define CONFIG_THREAD_COMM_SEMA
struct task_struct {
	const char *task_name;
	pid_t task;//Feng: change back to pid_t struct for fixing warnings//_thread_hdl_ task;	/* I: workqueue thread */

#ifdef CONFIG_THREAD_COMM_SIGNAL
	const char *name;	/* I: workqueue thread name */
	uint32_t queue_num;		/* total signal num */
	uint32_t cur_queue_num;	/* cur signal num should < queue_num */
#elif defined(CONFIG_THREAD_COMM_SEMA)
	_sema wakeup_sema;    /* for internal use only */
	_sema terminate_sema; /* for internal use only */
#endif
	uint32_t blocked;          /* for internal use only */
	uint32_t callback_running; /* for internal use only */
};


/*********************************** Thread related *****************************************/

/**
 * @brief  This function creates a new task and adds it to the list of tasks that are ready to run.
 * @param[in] task:  The task stucture which will store the task related infomation.
 * @param[in] name: A descriptive name for the task.
 * @param[in] stack_size: The size of the task stack specified as the variables the stack can hold.
 * @param[in] priority: The priority at which the task should run.
 * @param[in] func: The task entry function.
 * @param[in] thctx: The pointer that will be used as the parameter for the task being created.
 * @return  pdPASS: The task was successfully created and added to a ready list.
 * @return  other error code defined in the file errors.h.
 * @note  For the task name, please do not use "rtw_little_wifi_mcu_thread", "rtw_check_in_req_state_thread",
 		  "rtw_TDMA_change_state_thread", "xmit_thread", "recv_thread", "rtw_recv_tasklet", "rtw_xmit_tasklet",
 		  "rtw_interrupt_thread", "cmd_thread", "usb_init", "MSC_BULK_CMD" and "MSC_BULK_DATA".
 */
int	rtw_create_task(struct task_struct *task, const char *name, uint32_t  stack_size, uint32_t priority, thread_func_t func, void *thctx);

/**
 * @brief  This function deletes a task.
 * @param[in] task:  The task stucture which will be deleted.
 * @return  None
 */
void rtw_delete_task(struct task_struct *task);

void rtw_set_priority_task(void *task, uint32_t NewPriority);

int rtw_get_priority_task(void *task);

void rtw_suspend_task(void *task);

void rtw_resume_task(void *task);

void rtw_suspend_task_all(void);

void rtw_resume_task_all(void);

/**
 * @brief  This function prints the name of the thread in DBG_INFO.
 * @param[in] name:  The name of the thread.
 * @return  None
 */
void	rtw_thread_enter(char *name);

/**
 * @brief  This function exits the calling thread.
 * @return  None
 */
void	rtw_thread_exit(void);

/**
 * @brief  This function gets the scheduler state of the calling thread.
 * @return  OS_SCHEDULER_NOT_STARTED
 * @return  OS_SCHEDULER_RUNNING
 * @return  OS_SCHEDULER_SUSPENDED
 */
uint8_t		rtw_get_scheduler_state(void);

/**
 * @brief  This function creats secure stack for threads.
 * @param[in] secure_stack_size:  The size of the secure stack for this thread, the unit is byte.
 * @return  None
 */
void rtw_create_secure_context(uint32_t secure_stack_size);

void *rtw_get_current_TaskHandle(void);

/*************************** End Threads *******************************/
#ifdef __cplusplus
}
#endif

#endif
