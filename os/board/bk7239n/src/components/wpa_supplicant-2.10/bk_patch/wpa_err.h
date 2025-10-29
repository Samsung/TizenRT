#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define WPA_OK                          BK_OK
#define WPA_FAIL                        BK_FAIL

#define WPA_ERR_NO_MEM                  BK_ERR_NO_MEM
#define WPA_ERR_STATE                   BK_ERR_STATE
#define WPA_ERR_TRY_AGAIN               BK_ERR_TRY_AGAIN
#define WPA_ERR_PARAM                   BK_ERR_PARAM

#define WPA_ERR_SCAN_PENDING            (BK_ERR_WPA_BASE)
#define WPA_ERR_SCAN_SELECT_IN_PROGRESS (BK_ERR_WPA_BASE - 1)
#define WPA_ERR_SCAN_SCHED_IN_PROGRESS  (BK_ERR_WPA_BASE - 2)
#define WPA_ERR_SCAN_IN_PROGRESS        (BK_ERR_WPA_BASE - 3)
#define WPA_ERR_SCAN_FREQ               (BK_ERR_WPA_BASE - 4)
#define WPA_ERR_SCAN_ID                 (BK_ERR_WPA_BASE - 5)
#define WPA_ERR_SCAN_BSSID              (BK_ERR_WPA_BASE - 6)
#define WPA_ERR_SCAN_SSID               (BK_ERR_WPA_BASE - 7)
#define WPA_ERR_SCAN_NORMAL_DISCONNECT  (BK_ERR_WPA_BASE - 8)
#define WPA_ERR_SCAN_NORMAL_NO_NETWORK  (BK_ERR_WPA_BASE - 9)
#define WPA_ERR_SCAN_P2P_MGMT           (BK_ERR_WPA_BASE - 10)
#define WPA_ERR_SCAN_CLONE_PARAMS       (BK_ERR_WPA_BASE - 11)
#define WPA_ERR_SCAN_REG_CALLBACK       (BK_ERR_WPA_BASE - 12)
#define WPA_ERR_SCAN_REQ                (BK_ERR_WPA_BASE - 13)

#define WPA_ERR_WPAH_QUEUE_INIT         (BK_ERR_WPA_BASE - 30)
#define WPA_ERR_WPAS_INIT               (BK_ERR_WPA_BASE - 31)
#define WPA_ERR_CTRL_SEM_INIT           (BK_ERR_WPA_BASE - 32)
#define WPA_ERR_HAPD_INIT               (BK_ERR_WPA_BASE - 33)
#ifdef __cplusplus
}
#endif
