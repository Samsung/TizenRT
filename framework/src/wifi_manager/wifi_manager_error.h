#ifndef _WIFI_MANAGER_ERROR_H__
#define _WIFI_MANAGER_ERROR_H__

#ifdef CONFIG_WIFIMGR_ERROR_REPORT
#include <error_report/error_report.h>
#define WIFIADD_ERR_RECORD(reason_code)	ERR_DATA_CREATE(ERRMOD_WIFI_MANAGER, reason_code)
#else
#define WIFIADD_ERR_RECORD(reason_code)
#endif

#ifdef CONFIG_WIFIMGR_ERROR_REPORT
static inline _set_error_code(wifi_manager_result_e res)
{
	if (res == WIFI_MANAGER_SUCCESS) {
		return;
	}

	WM_LOG_ERROR("[WM] Wi-Fi manager fail: state error\n");
	error_code_wifi_manager_t wifi_err_code;
	switch (msg->event) {
	case EVT_INIT_CMD:
		wifi_err_code = ERR_WIFIMGR_INIT_FAIL;
		break;
	case EVT_DEINIT_CMD:
		wifi_err_code = ERR_WIFIMGR_DEINIT_FAIL;
		break;
	case EVT_SET_SOFTAP_CMD:
		wifi_err_code = ERR_WIFIMGR_SOFTAP_FAIL;
		break;
	case EVT_SET_STA_CMD:
		wifi_err_code = ERR_WIFIMGR_STA_FAIL;
		break;
	case EVT_CONNECT_CMD:
		wifi_err_code = ERR_WIFIMGR_CONNECT_FAIL;
		break;
	case EVT_DISCONNECT_CMD:
		wifi_err_code = ERR_WIFIMGR_DISCONNECT_FAIL;
		break;
	case EVT_SCAN_CMD:
		wifi_err_code = ERR_WIFIMGR_SCAN_FAIL;
		break;
	default:
		wifi_err_code = ERR_WIFIMGR_UTILS_FAIL;
		break;
	}
	WIFIADD_ERR_RECORD(wifi_err_code);
}
#endif

#endif // _WIFI_MANAGER_ERROR_H__
