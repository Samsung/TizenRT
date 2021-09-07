/*
 * test_type, test_handler, test_parser, test_command
 */
WT_MEMBER_POOL(WT_TYPE_START, _wt_start, _wt_parse_none, "start")
WT_MEMBER_POOL(WT_TYPE_STOP, _wt_stop, _wt_parse_none, "stop")
WT_MEMBER_POOL(WT_TYPE_SOFTAP, _wt_softap_start, _wt_parse_softap, "softap")
WT_MEMBER_POOL(WT_TYPE_STA, _wt_sta_start, _wt_parse_none, "sta")
WT_MEMBER_POOL(WT_TYPE_JOIN, _wt_connect, _wt_parse_join, "join")
WT_MEMBER_POOL(WT_TYPE_SJOIN, _wt_scan_connect, _wt_parse_join, "sjoin")
WT_MEMBER_POOL(WT_TYPE_LEAVE, _wt_disconnect, _wt_parse_none, "leave")
// stop reconnecting to wifi AP. it doesn't expect to receive a signal because AP is already disconnected */
WT_MEMBER_POOL(WT_TYPE_CANCEL, _wt_cancel, _wt_parse_none, "cancel")
WT_MEMBER_POOL(WT_TYPE_SET, _wt_set_info, _wt_parse_set, "set")
WT_MEMBER_POOL(WT_TYPE_GET, _wt_get_info, _wt_parse_none, "get")
WT_MEMBER_POOL(WT_TYPE_RESET, _wt_reset_info, _wt_parse_none, "reset")
WT_MEMBER_POOL(WT_TYPE_SCAN, _wt_scan, _wt_parse_scan, "scan")
WT_MEMBER_POOL(WT_TYPE_STATS, _wt_get_stats, _wt_parse_none, "stats")
WT_MEMBER_POOL(WT_TYPE_INFO, _wt_get_conn_info, _wt_parse_none, "info")
WT_MEMBER_POOL(WT_TYPE_SET_POWER, _wt_set_power, _wt_parse_power, "power")
WT_MEMBER_POOL(WT_TYPE_ONOFF, _wt_onoff_test, _wt_parse_join, "on_off")
WT_MEMBER_POOL(WT_TYPE_STRESS, _wt_stress_test, _wt_parse_stress, "stress")
