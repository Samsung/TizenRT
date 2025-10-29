#include "bk_private/bk_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include <driver/mailbox.h>
#include "cli.h"
#include <driver/pwr_clk.h>
#include <modules/pm.h>
#include "bk_pm_demo.h"
//#include "driver/pm_ap_core.h"

/*=====================DEFINE  SECTION  START=====================*/
#define TAG "pm_demo"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define PM_DEMO_STACK_SIZE              (1024)
#define PM_DEMO_QUEUE_NUMBER_OF_MESSAGE (10)

/*=====================DEFINE  SECTION  END=====================*/
/*=====================STRUCT AND ENUM  SECTION  START==========*/

/*=====================STRUCT AND ENUM  SECTION  END=============*/
/*=====================VARIABLE  SECTION  START==================*/
static  beken_thread_t s_thd;
static  beken_queue_t  s_queue;
/*=====================VARIABLE  SECTION  END==================*/

/*================FUNCTION DECLARATION  SECTION  START==========*/
bk_err_t bk_pm_demo_send_msg(pm_ap_core_msg_t *msg);
/*================FUNCTION DECLARATION  SECTION  END===========*/
static bk_err_t pm_demo_rtc_sleep_wakeup_callback(pm_sleep_mode_e sleep_mode,pm_wakeup_source_e wake_source,void* param_p)
{
    pm_ap_core_msg_t msg = {0};
    msg.event= PM_DEMO_CALLBACK_MSG_HANDLE;
    msg.param1 = PM_MODE_LOW_VOLTAGE;
    msg.param2 = PM_WAKEUP_SOURCE_INT_RTC;
    msg.param3 = 0;
    bk_pm_demo_send_msg(&msg);
    return BK_OK;
}
void pm_demo_gpio_callback(gpio_id_t gpio_id)
{
    pm_ap_core_msg_t msg = {0};
    msg.event= PM_DEMO_CALLBACK_MSG_HANDLE;
    msg.param1 = PM_MODE_LOW_VOLTAGE;
    msg.param2 = PM_WAKEUP_SOURCE_INT_GPIO;
    msg.param3 = gpio_id;
    bk_pm_demo_send_msg(&msg);
}
static bk_err_t pm_demo_sleep_wakeup_callback(void* param1,uint32_t param2)
{
    LOGD("%s\r\n",__func__);
    return BK_OK;
}

static bk_err_t pm_demo_init(void)
{
    return BK_OK;
}
bk_err_t bk_pm_demo_send_msg(pm_ap_core_msg_t *msg)
{
    bk_err_t ret = BK_OK;
	if(msg == NULL)
	{
	    LOGE("Pm core send msg error\r\n");
		return BK_FAIL;
	}

    if (s_queue)
    {
        ret = rtos_push_to_queue(&s_queue, msg, BEKEN_NO_WAIT);

        if (BK_OK != ret)
        {
            LOGE("%s failed\n", __func__);
            return BK_FAIL;
        }
        return ret;
    }

    return ret;
}

static bk_err_t pm_demo_message_handle(void)
{
    bk_err_t ret = BK_OK;
    pm_ap_core_msg_t msg;

	pm_demo_init();

    while (1)
    {
        ret = rtos_pop_from_queue(&s_queue, &msg, BEKEN_WAIT_FOREVER);
        LOGD("%s event:%d\n", __func__,msg.event);
        if (kNoErr == ret)
        {
            switch (msg.event)
            {
                case PM_DEMO_ENTER_LOW_VOLTAGE:
                {

                }
                break;
                case PM_DEMO_ENTER_DEEP_SLEEP:
                {

                }
                break;
                case PM_DEMO_CALLBACK_MSG_HANDLE:
                {
                    if(msg.param1 == PM_MODE_LOW_VOLTAGE)
                    {
                        if((msg.param3 == BK_PM_WAKEUP_UART_CONSOLE)&&(msg.param2 == PM_WAKEUP_SOURCE_INT_GPIO))//uart0 rxd wakeup
                        {
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG,0x0,0x0);
                            LOGI("lv wakeup[%d]\r\n",bk_pm_sleep_wakeup_reason_get());
                            rtos_delay_milliseconds(2000);//wakeup 2s
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG,0x1,0x0);
                        }
                        else
                        {
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
                            LOGI("Lv sleep[src:%d][%d][%d]\r\n",msg.param2,bk_pm_exit_low_vol_wakeup_source_get(),bk_pm_sleep_wakeup_reason_get());
                            if(msg.param2 == PM_WAKEUP_SOURCE_INT_RTC)
                            {
                                rtos_delay_milliseconds(10);
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,0x0);
                            }
                            else if(msg.param2 == PM_WAKEUP_SOURCE_INT_GPIO)
                            {

                            }
                        }
                    }
                    else if(msg.param1 == PM_MODE_NORMAL_SLEEP)
                    {
                        if(msg.param2 == PM_WAKEUP_SOURCE_INT_RTC)
                        {
                            LOGI("sleep rtc wakeup:%d,type:%d\r\n",bk_pm_sleep_wakeup_reason_get(),msg.param3);
                            if(msg.param3 == BK_PM_WAKEUP_HW_TIMER)
                            {
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
                                //one count wakeup from timer
                            }
                            else
                            {
                                /*Repeat test for sleep enter*/
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
                                rtos_delay_milliseconds(10);
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,0x0);
                            }
                        }
                        else if(msg.param2 == PM_WAKEUP_SOURCE_INT_GPIO)
                        {
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
                            if((msg.param3 == BK_PM_WAKEUP_UART_CONSOLE)&&(msg.param2 == PM_WAKEUP_SOURCE_INT_GPIO))
                            {
                                LOGI("slep wakeup[%d]\r\n",bk_pm_sleep_wakeup_reason_get());
                                rtos_delay_milliseconds(2000);//wakeup 2s
                                bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_LOG,0x1,0x0);
                            }
                        }
                        else
                        {
                            bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
                        }
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

bk_err_t pm_demo_thread_main(void)
{
    bk_err_t ret = BK_OK;
	ret = rtos_init_queue(&s_queue,
                          "demo_queue",
                          sizeof(pm_ap_core_msg_t),
                          PM_DEMO_QUEUE_NUMBER_OF_MESSAGE);

    if (ret != BK_OK)
    {
        LOGE("create pm demo que fail\n");
    }
	ret = rtos_create_thread(&s_thd,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "pm_demo_thd",
                             (beken_thread_function_t)pm_demo_message_handle,
                             PM_DEMO_STACK_SIZE,
                             NULL);
    if (ret != BK_OK)
    {
        LOGE("create pm demo thrd fail\n");
    }
	return 0;
}