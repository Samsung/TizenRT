#ifndef _WIFI_UTILS_EVENT_H__
#define _WIFI_UTILS_EVENT_H__

void lwnl_start_monitor(void);
void lwnl_stop_monitor(void);
int lwnl_join_monitor(wifi_utils_cb_s *cb);
int lwnl_leave_monitor(wifi_utils_cb_s *cb);

#endif
