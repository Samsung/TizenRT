#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include <driver/mailbox.h>
#include "cli.h"
#include <driver/pwr_clk.h>
#include <modules/pm.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/aon_rtc_types.h>
#include <driver/aon_rtc.h>

/*=====================DEFINE  SECTION  START=====================*/
#define TAG "pm"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define PM_THREAD_STACK_SIZE              (1024)  /* Keep 1024 due to limited SRAM */
#define PM_QUEUE_NUMBER_OF_MESSAGE        (20)   /* Reduced to 20 to save SRAM (~240B queue) */
#define PM_THREAD_PRIORITY                (BEKEN_DEFAULT_WORKER_PRIORITY - 3)/* With priority 3 thread, 200ms buffer (20*10ms) is sufficient */

#define PM_UART_RXD_WAKEUP_TIME_MS        (2000)

/*=====================DEFINE  SECTION  END=====================*/
/*=====================STRUCT AND ENUM  SECTION  START==========*/

/*=====================STRUCT AND ENUM  SECTION  END=============*/
/*=====================VARIABLE  SECTION  START==================*/
static  beken_thread_t s_thd;
static  beken_queue_t  s_queue;
/* Statistics for monitoring queue status */
static volatile uint32_t s_msg_send_success           = 0;
static volatile uint32_t s_msg_send_fail              = 0;
static volatile uint32_t s_pm_thread_rtc_sleep_count  = 0;
/*=====================VARIABLE  SECTION  END==================*/

/*================FUNCTION DECLARATION  SECTION  START==========*/
bk_err_t bk_pm_send_msg(pm_ap_core_msg_t *msg);
/*================FUNCTION DECLARATION  SECTION  END===========*/
static bk_err_t pm_rtc_sleep_wakeup_callback(pm_sleep_mode_e sleep_mode,pm_wakeup_source_e wake_source,void* param_p)
{
    pm_ap_core_msg_t msg = {0};
    msg.event= PM_CALLBACK_HANDLE_MSG;
    msg.param1 = PM_MODE_LOW_VOLTAGE;
    msg.param2 = PM_WAKEUP_SOURCE_INT_RTC;
    msg.param3 = 0;
    bk_pm_send_msg(&msg);
    return BK_OK;
}
void pm_gpio_callback(gpio_id_t gpio_id)
{
    pm_ap_core_msg_t msg = {0};
    msg.event= PM_CALLBACK_HANDLE_MSG;
    msg.param1 = PM_MODE_LOW_VOLTAGE;
    msg.param2 = PM_WAKEUP_SOURCE_INT_GPIO;
    msg.param3 = gpio_id;
    bk_pm_send_msg(&msg);
}
static bk_err_t pm_sleep_wakeup_callback(void* param1,uint32_t param2)
{
    LOGD("%s\r\n",__func__);
    return BK_OK;
}

static void pm_lv_gpio_wakeup_oneshot_timer_handler(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG, 0x1, 0x0);
}

static bk_err_t pm_demo_init(void)
{
    return BK_OK;
}

bk_err_t bk_pm_send_msg(pm_ap_core_msg_t *msg)
{
    bk_err_t ret = BK_OK;
	if(msg == NULL)
	{
	    LOGE("Pm core send msg error\r\n");
		return BK_FAIL;
	}

    if (s_queue)
    {
        /* Optimization: Using BEKEN_NO_WAIT in interrupt context is correct,
         * but need to handle queue full gracefully */
        ret = rtos_push_to_queue(&s_queue, msg, BEKEN_NO_WAIT);

        if (BK_OK != ret)
        {
            /* Queue full or other error, silently drop message to avoid log flooding */
            s_msg_send_fail++;
            /* Log only once per 1000 failures to avoid log storm */
            if ((s_msg_send_fail % 1000) == 0)
            {
                LOGE("Pm msg failed (total: %u, success: %u)\n", s_msg_send_fail, s_msg_send_success);
            }
            return BK_FAIL;
        }
        s_msg_send_success++;
        return ret;
    }
    return ret;
}

static bk_err_t pm_message_handle(void)
{
    bk_err_t ret = BK_OK;
    pm_ap_core_msg_t msg;

	pm_demo_init();

    while (1)
    {
        ret = rtos_pop_from_queue(&s_queue, &msg, BEKEN_WAIT_FOREVER);
        if (kNoErr == ret)
        {
            switch (msg.event)
            {
                case PM_ENTER_LOW_VOLTAGE_MSG:
                {
                    /* Reserved for future use */
                }
                break;

                case PM_ENTER_DEEP_SLEEP_MSG:
                {
                    /* Reserved for future use */
                }
                break;

                case PM_CALLBACK_HANDLE_MSG:
                {
                    /* Fast path: Handle RTC periodic callbacks efficiently */
                    if (msg.param2 == PM_WAKEUP_SOURCE_INT_RTC)
                    {
                        /* RTC periodic callback (10ms) - minimal processing */
                        if (msg.param1 == PM_MODE_LOW_VOLTAGE)
                        {
                            s_pm_thread_rtc_sleep_count++;
                            /* For periodic callbacks (param3 == 0), use fast path */
                            if (msg.param3 == 0) {
                                /* Just maintain the vote state, no delay */
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x1, 0x0);
                            } else {
                                /* State changed - log and process */
                                LOGI("LV RTC wakeup[reason:%d][Count:%d][Current:%d]\r\n",bk_pm_sleep_wakeup_reason_get(),msg.param4,s_pm_thread_rtc_sleep_count);
                                if(s_pm_thread_rtc_sleep_count == msg.param4)
                                {
                                    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                                    s_pm_thread_rtc_sleep_count = 0;
                                }
                                else
                                {
                                    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x1, 0x0);
                                }
                            }
                        }
                        else if (msg.param1 == PM_MODE_NORMAL_SLEEP)
                        {
                            s_pm_thread_rtc_sleep_count++;
                            /* Handle based on param3 flag from interrupt */
                            if (msg.param3 == BK_PM_WAKEUP_HW_TIMER)
                            {
                                /* Hardware timer wakeup */
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                            } 
                            else if (msg.param3 == 1)
                            {
                                /* State changed - full processing */
                                LOGI("NS RTC wakeup[reason:%d][Count:%d][Current:%d]\r\n",bk_pm_sleep_wakeup_reason_get(),msg.param4,s_pm_thread_rtc_sleep_count);
                                if(s_pm_thread_rtc_sleep_count == msg.param4)
                                {
                                    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                                    s_pm_thread_rtc_sleep_count = 0;
                                }
                                else
                                {
                                    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x1, 0x0);
                                }
                            }
                            else
                            {
                                /* Periodic callback - fast path */
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x1, 0x0);
                            }
                        }
                    }
                    /* GPIO wakeup - less frequent, can use full processing */
                    else if (msg.param2 == PM_WAKEUP_SOURCE_INT_GPIO)
                    {
                        bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                        if (msg.param1 == PM_MODE_LOW_VOLTAGE) {
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG, 0x0, 0x0);
                            LOGI("LV GPIO wakeup[reason:%d][id:%d]\r\n", bk_pm_sleep_wakeup_reason_get(), msg.param3);

                            alarm_info_t rxd_wk_alarm = {
                                                            "rxd_wk",
                                                            PM_UART_RXD_WAKEUP_TIME_MS*AON_RTC_MS_TICK_CNT,
                                                            1,
                                                            pm_lv_gpio_wakeup_oneshot_timer_handler,
                                                            NULL
                                                        };

                            //force unregister previous if doesn't finish.
                            bk_alarm_unregister(AON_RTC_ID_1, rxd_wk_alarm.name);
                            ret = bk_alarm_register(AON_RTC_ID_1, &rxd_wk_alarm);
                            if(ret != BK_OK)
                            {
                                LOGE("register rxd_wk_alarm failed\r\n");
                                return BK_FAIL;
                            }
                        } else if (msg.param1 == PM_MODE_NORMAL_SLEEP) {
                            LOGI("NS GPIO wakeup[reason:%d][id:%d]\r\n", bk_pm_sleep_wakeup_reason_get(), msg.param3);
                            //rtos_delay_milliseconds(2000);
                            //bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG, 0x1, 0x0);
                        }

                    }
                    else
                    {
                        /* Other wakeup sources */
                        bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0x0, 0x0);
                    }
                }
                break;

                default:
                    break;
            }
        }
    }

	return  ret;
}

bk_err_t pm_thread_main(void)
{
    bk_err_t ret = BK_OK;
	ret = rtos_init_queue(&s_queue,
                          "pm_queue",
                          sizeof(pm_ap_core_msg_t),
                          PM_QUEUE_NUMBER_OF_MESSAGE);

    if (ret != BK_OK)
    {
        LOGE("create pm que fail\n");
    }

	/* Optimization: Use higher priority (lower number = higher priority)
	 * to ensure 10ms messages are processed promptly.
	 * Changed from BEKEN_DEFAULT_WORKER_PRIORITY to 3 for time-critical processing.
	 */
	ret = rtos_create_thread(&s_thd,
                             PM_THREAD_PRIORITY, /* Higher priority for 10ms interrupt handling */
                             "pm_thd",
                             (beken_thread_function_t)pm_message_handle,
                             PM_THREAD_STACK_SIZE,
                             NULL);
    if (ret != BK_OK)
    {
        LOGE("create pm thrd fail\n");
    }
	return 0;
}