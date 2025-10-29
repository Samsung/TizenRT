// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <sdkconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <common/bk_include.h>
#include "bk_private/components_init.h"
#include <os/os.h>
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/wdt.h>
#include <bk_wdt.h>

#if (defined(CONFIG_SYS_CPU0))
#include <modules/pm.h>
#include <modules/ota.h>
#endif
#include <driver/flash_partition.h>

#include "boot.h"

#include "stack_base.h"
#include "soc/soc.h"

// static beken_thread_function_t s_user_app_entry = NULL;
// beken_semaphore_t user_app_sema = NULL;

// void rtos_set_user_app_entry(beken_thread_function_t entry)
// {
// 	s_user_app_entry = entry;
// }

// void rtos_user_app_preinit(void)
// {
//     int ret = rtos_init_semaphore(&user_app_sema, 1);
// 	if(ret < 0){
// 		os_printf("init queue failed");
// 	}
// }

// void rtos_user_app_launch_over(void)
// {
// 	int ret;

// 	ret = rtos_set_semaphore(&user_app_sema);
// 	if(ret < 0){
// 		os_printf("set sema failed");
// 	}
// }

// void rtos_user_app_waiting_for_launch(void)
// {
// 	int ret;

// 	ret = rtos_get_semaphore(&user_app_sema, BEKEN_WAIT_FOREVER);
// 	if(ret < 0){
// 		os_printf("get sema failed");
// 	}

// #if CONFIG_SAVE_BOOT_TIME_POINT
// 	save_mtime_point(CPU_APP_ENTRY_TIME);
// #endif
// }


void bk_set_jtag_mode(uint32_t cpu_id, uint32_t group_id) {
	if (cpu_id == 0) {
		(void)sys_drv_set_jtag_mode(0);
	} else if (cpu_id == 1) {
		(void)sys_drv_set_jtag_mode(1);
	} else if (cpu_id == 2) {
		(void)sys_drv_set_jtag_mode(2);
	} else {
		os_printf("Unsupported cpu id(%d).\r\n", cpu_id);
		return;
	}

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_120M);

	/*close watchdog*/
#if defined(CONFIG_WATCHDOG)
	bk_wdt_stop();
#endif

	if (group_id == 0) {
		gpio_jtag_sel(0);
	} else if (group_id == 1) {
		gpio_jtag_sel(1);
	} else {
		os_printf("Unsupported group id(%d).\r\n", group_id);
		return;
	}
}

// static void user_app_thread( void *arg )
// {
// 	rtos_user_app_waiting_for_launch();
// 	/* add your user_main*/
// 	os_printf("user app entry(0x%0x)\r\n", s_user_app_entry);
// 	if(NULL != s_user_app_entry) {
// 		s_user_app_entry(0);
// 	}

// 	rtos_deinit_semaphore(&user_app_sema);

// 	rtos_delete_thread( NULL );
// }

// static void start_user_app_thread(void)
// {
// 	os_printf("start user app thread.\r\n");
// 	rtos_create_thread(NULL,
// 					BEKEN_APPLICATION_PRIORITY,
// 					"app",
// 					(beken_thread_function_t)user_app_thread,
// 					CONFIG_APP_MAIN_TASK_STACK_SIZE,
// 					(beken_thread_arg_t)0);
// }

#if defined(CONFIG_SUPPORT_MATTER)
beken_thread_t matter_thread_handle = NULL;
extern void ChipTest(void);
static void matter_thread( void *arg ) {
    ChipTest();
    rtos_delete_thread(NULL);
}

void start_matter(void) {
    os_printf("start matter\r\n");
    rtos_create_thread(&matter_thread_handle,
        BEKEN_DEFAULT_WORKER_PRIORITY,
         "matter",
        (beken_thread_function_t)matter_thread,
        8192,
        0);
}
#endif //#if CONFIG_SUPPORT_MATTER


extern int main(void);
extern bool ate_is_enabled(void);

static void app_main_thread(void *arg)
{

#if (defined(CONFIG_SYS_CPU0))
	rtos_user_app_preinit();
#endif

#if defined(CONFIG_TFM_FWU)
	int ret = bk_ota_accept_image();
    if(ret != 0)
    {
		os_printf("OTA accept fail[ret:%d]\r\n",ret);
    }
#endif
	main();

#if defined(CONFIG_MATTER_START) && defined(CONFIG_SUPPORT_MATTER)
	start_matter();
#endif //#if CONFIG_MATTER_START && CONFIG_SUPPORT_MATTER

#if defined(CONFIG_ATE)
    if(ate_is_enabled())
    {
        os_printf("ATE enabled = 1\r\n");
    }
#endif

	rtos_delete_thread(NULL);
}

void start_app_main_thread(void)
{
	rtos_create_thread(NULL, CONFIG_APP_MAIN_TASK_PRIO,
		"main",
		(beken_thread_function_t)app_main_thread,
		CONFIG_APP_MAIN_TASK_STACK_SIZE,
		(beken_thread_arg_t)0);
}

void entry_main(void)
{
#if defined(CONFIG_GCOV)
	__gcov_call_constructors();
#endif

#if (defined(CONFIG_ATE_TEST))
	bk_set_printf_enable(0);
#endif

	if(components_init())
		return;

	//start_app_main_thread();
	//start_user_app_thread();

}
// eof

