/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/arch/arm/src/amebasmart/amebasmart_pmhelpers.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <string.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/aon_rtc_types.h>
#include <driver/aon_rtc.h>
#include "modules/pm.h"
#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>

static void pm_timer_wakeup_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	pm_ap_core_msg_t msg = {0};
    msg.event= PM_CALLBACK_HANDLE_MSG;
    msg.param1 = PM_MODE_NORMAL_SLEEP;
    msg.param2 = PM_WAKEUP_SOURCE_INT_RTC;
    msg.param3 = BK_PM_WAKEUP_HW_TIMER;
    if(bk_pm_send_msg(&msg) != BK_OK)
    {
        BK_LOGE("PM","Error: send message to pm failed\r\n");
    }
}
void up_set_pm_timer(unsigned int interval_us) 
{
	#if defined(CONFIG_AON_RTC) || defined(CONFIG_ANA_RTC)
	uint32_t sleep_count = 1; //one count
	alarm_info_t sleep_alarm = {0};

	strncpy((char *)sleep_alarm.name, PM_APP_RTC_ALARM_NAME, sizeof(sleep_alarm.name) - 1);
	sleep_alarm.name[sizeof(sleep_alarm.name) - 1] = '\0';
	sleep_alarm.period_tick = interval_us * AON_RTC_MS_TICK_CNT / 1000;
	sleep_alarm.period_cnt = 1;
	sleep_alarm.callback = pm_timer_wakeup_callback;
	sleep_alarm.param_p = &sleep_count;

	//force unregister previous if doesn't finish.
	bk_alarm_unregister(AON_RTC_ID_1, sleep_alarm.name);
	bk_alarm_register(AON_RTC_ID_1, &sleep_alarm);
	#endif //CONFIG_AON_RTC
}

void bsp_pm_domain_register(char *domain_name, int bsp_drv_id)
{
}

void bsp_pm_domain_control(int bsp_drv_id, bool is_suspend)
{
}

/* Interrupt callback from wifi-keepalive, which LP received designated packet*/
void SOCPS_LPWAP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum)
{

}


#endif
