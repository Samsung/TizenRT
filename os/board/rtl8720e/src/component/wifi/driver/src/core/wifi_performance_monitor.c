#include "wifi_performance_monitor.h"

#ifdef WIFI_PERFORMANCE_MONITOR
struct WIFI_TIME wifi_time_test;
struct WIFI_TIME wifi_time_result;

/**
  * @brief  Get wifi performance monitor timer tick.
  * @param  none
  */
u32 WIFI_PMTimer_GetCount(void)
{
	return DTimestamp_Get();  //debug timer
}

/**
  * @brief  Get wifi performance monitor timer pass time, unit is us.
  * @param  start: start time stamp
  * @note: deubg timer -> default 1MHz
  */
u32 WIFI_PMTimer_GetPassTime(u32 start)
{
	u32 current = 0;
	u32 passed = 0;

	current = DTimestamp_Get();

	if (current >= start) {
		passed = current - start;
	} else {
		passed = 0xFFFFFFFF - (start - current);
	}

	/* ms */
	//passed = (passed * 31) / 1000; //31us one tick in TIM0

	return passed;
}


void wifi_performance_print()
{
//print wifi_time_test
#if defined(CONFIG_AS_INIC_NP) ||defined(CONFIG_SINGLE_CORE_WIFI)
	printf("--------WIFI_TIME_TEST---------  \n\r");
	printf("-------------RX --------------- \n\r");
	printf("rx_mpdu: %d us, memcpy: %d us, phy_status: %d us \n\r", wifi_time_result.rx_mpdu_time, wifi_time_result.rx_mpdu_time1, wifi_time_result.rx_mpdu_time2);
	printf("recv_entry: %d us \n\r", wifi_time_result.recv_entry_time);
	printf("recv_func_time: %d us \n\r", wifi_time_result.recv_func_time);
	printf("recv_func_prehandle: %d us \n\r", wifi_time_result.recv_func_prehandle_time);
	printf("validate_recv_frame: %d, validate_recv_data_frame: %d us \n\r", wifi_time_result.validate_recv_frame_time, wifi_time_result.validate_recv_frame_time1);
	printf("recv_func_posthandle: %d,  decryptor: %d, portctrl: %d, count_rx_ststs: %d us \n\r", wifi_time_result.recv_func_posthandle_time,
		   wifi_time_result.recv_func_posthandle_time1, wifi_time_result.recv_func_posthandle_time2, wifi_time_result.recv_func_posthandle_time3);
	printf("Process_recv_indicatepkts: %d us \n\r", wifi_time_result.process_recv_indicatepkts_time);
	printf("rtw_recv_indicatpkt: %d us \n\r", wifi_time_result.rtw_recv_indicatept_time);
	printf("rltk_netif_rx: %d us \n\r", wifi_time_result.rltk_netif_rx_time);
	printf("netif_rx: %d us \n\r", wifi_time_result.netif_rx_time);
	printf("etherbetif_recv: %d, rltk_wlan_recv: %d, netif->input: %d us \n\r", wifi_time_result.ethernetif_recv_time, wifi_time_result.rltk_wlan_recv_time,
		   wifi_time_result.netif_input_time);
#endif
	printf("-------------TX--------------- \n\r");

#if defined(CONFIG_AS_INIC_AP)
	printf("inic_ipc_host_send: %d, inic_ipc_host_alloc_skb: %d, memcpy: %d us,inic_ipc_host_send_skb: %d us\n\r",   wifi_time_result.wlan_send_time,
		   wifi_time_result.wlan_send_time1,
		   wifi_time_result.wlan_send_time2, wifi_time_result.wlan_send_time3);
#endif

#if defined(CONFIG_AS_INIC_NP)
	printf("inic_xmit_tasklet: %d, inic_dequeue_xmitbuf: %d, inic_xmit_tasklet_handler: %d us \n\r",   wifi_time_result.xmit_task_time,
		   wifi_time_result.dequeue_xmitbuf_time,
		   wifi_time_result.xmit_handler_time);
#endif

#if defined(CONFIG_AS_INIC_NP) ||defined(CONFIG_SINGLE_CORE_WIFI)
	printf("rltk_wlan_send_skb: %d us \n\r",   wifi_time_result.wlan_send_skb_time);
	printf("rtw_xmit_entry: %d us \n\r",   wifi_time_result.xmit_entry_time);
	printf("rtw_xmit: %d, update_attrib: %d us \n\r",   wifi_time_result.xmit_time,   wifi_time_result.xmit_time1);
	printf("rtw_xmit_data: %d us \n\r",   wifi_time_result.xmit_data_time);
	printf("pre_xmitframe: %d, check_nic: %d, alloc_xmitbuf: %d us \n\r",   wifi_time_result.pre_xmitframe_time,   wifi_time_result.pre_xmitframe_time1,
		   wifi_time_result.pre_xmitframe_time2);
	printf("xmitframe_direct: %d us \n\r",   wifi_time_result.xmitframe_direct_time);
	printf("xmitframe_coalesce: %d us \n\r",   wifi_time_result.xmitframe_coalesce_time);
	printf("dump_xframe: %d us \n\r",   wifi_time_result.dump_xframe_time);
#endif
}
#endif //WIFI_PERFORMANCE_MONITOR

