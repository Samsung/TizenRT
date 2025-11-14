#ifndef _BK_COEX_EXT_H_
#define _BK_COEX_EXT_H_
#include <common/bk_include.h>
// Wi-Fi Status ID defines
#define COEX_WIFI_STAT_ID_SCANNING          0
#define COEX_WIFI_STAT_ID_CONNECTING        1
typedef struct {
    int version;
    void (*inform_wifi_status)(uint8_t status_id, uint8_t status_info);
} coex_to_bt_func_p_t;
/*
 ****************************************************************************************
 * @brief Bluetoot call this funciont to initialize the array of function point
 *
 * @param[in] function point
 *
 * @return void
 ****************************************************************************************
*/
void coex_bt_if_init(coex_to_bt_func_p_t *pf_ctb);
#endif
