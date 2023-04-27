//----------------------------------------------------------------------------//
#include "wifi_ind.h"
#include "wifi_conf.h"
#include "platform_stdlib.h"
#if defined(CONFIG_AS_INIC_NP)
#include "inic_ipc_api.h"
#endif

/******************************************************
 *                    Globals
 ******************************************************/
extern rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback;

static event_list_elem_t     event_callback_list[WIFI_EVENT_MAX][WIFI_EVENT_MAX_ROW];

//----------------------------------------------------------------------------//
rtw_result_t rtw_indicate_event_handle(int event_cmd, char *buf, int buf_len, int flags)
{
	rtw_event_handler_t handle = NULL;
	int i;

	if (event_cmd >= WIFI_EVENT_MAX) {
		return (rtw_result_t)RTW_BADARG;
	}

	for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
		handle = event_callback_list[event_cmd][i].handler;
		if (handle == NULL) {
			continue;
		}
		handle(buf, buf_len, flags, event_callback_list[event_cmd][i].handler_user_data);
	}

	return RTW_SUCCESS;
}

void wifi_indication(rtw_event_indicate_t event, char *buf, int buf_len, int flags)
{
	//
	// If upper layer application triggers additional operations on wireless event,
	// 		please strictly check current stack size usage (by using uxTaskGetStackHighWaterMark() )
	//		, and tries not to share the same stack with wlan driver if remaining stack space is
	//		not available for the following operations.
	//		ex: using semaphore to notice another thread.

#if defined(CONFIG_AS_INIC_NP)
	inic_ipc_wifi_event_indicate(event, buf, buf_len, flags);
#endif

	if (event == WIFI_EVENT_JOIN_STATUS) {
		if (p_wifi_joinstatus_internal_callback) {
			p_wifi_joinstatus_internal_callback((rtw_join_status_t)flags);
		}
	} else {
		rtw_indicate_event_handle(event, buf, buf_len, flags);
	}
}

void wifi_reg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func, void *handler_user_data)
{
	int i = 0, j = 0;
	if (event_cmds < WIFI_EVENT_MAX) {
		for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
			if (event_callback_list[event_cmds][i].handler == NULL) {
				for (j = 0; j < WIFI_EVENT_MAX_ROW; j++) {
					if (event_callback_list[event_cmds][j].handler == handler_func) {
						return;
					}
				}
				event_callback_list[event_cmds][i].handler = handler_func;
				event_callback_list[event_cmds][i].handler_user_data = handler_user_data;
				return;
			}
		}
	}
}

void wifi_unreg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func)
{
	int i;
	if (event_cmds < WIFI_EVENT_MAX) {
		for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
			if (event_callback_list[event_cmds][i].handler == handler_func) {
				event_callback_list[event_cmds][i].handler = NULL;
				event_callback_list[event_cmds][i].handler_user_data = NULL;
				return;
			}
		}
	}
}

void init_event_callback_list(void)
{
	memset(event_callback_list, 0, sizeof(event_callback_list));
}

