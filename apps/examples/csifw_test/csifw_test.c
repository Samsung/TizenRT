#include "csifw_test.h"
// #include "csimanager/config.h"
#include "csifw/csifw_api.h"
// #include <system/system_log.h>

#include <stdio.h>


/****************************************************************************
 * csifw_test
 ****************************************************************************/

#include <fcntl.h>
static int gFd; /* File descriptor*/

#define PRNT_LN1 printf("______________________________________________________________________________________________________\n")
#define PRNT_LN2 printf("______________________________________________________________________________________________________\n\n")
#define PRNT_LN3 printf("\n\n______________________________________________________________________________________________________\n")


static unsigned int gCounter;
static unsigned char* g_raw_buffer;
static float* g_parsed_buffer;

static int print_log(const char *format, ...)
{
	
	va_list args;
	va_start(args, format);

	vprintf(format, args);
	va_end(args);
	return 1;
}

static void print_buf(const unsigned char* buf, int len)
{
	print_log("\n RAW DATA %d\n\n", len);
	unsigned long long *buff_tmp = (u64 *)buf;
	int buff_len = (len/8) + 1;
	for (int i = 0; i < buff_len; i++) 
		print_log("[%02d]0x%016llx\n", i, buff_tmp[i]);
}



static void demo_get_empty_data_buffer(BUF_TYPE buftype, void **buffptr, int data_size)
{
	if (buftype == BTYPE_RAW) {
		printf("[APP] Providing RAW Buffer \n");
		if(!g_raw_buffer)
			g_raw_buffer = malloc(data_size);
		*buffptr = g_raw_buffer;
	} else if(buftype == BTYPE_PARSED) {
		printf("[APP] Providing PARSE Buffer \n");
		if(!g_parsed_buffer)
			g_parsed_buffer = (float *)malloc(sizeof(float)*data_size);
		*buffptr = g_parsed_buffer;
	}
}

static void demo_upd_raw_data_listener(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void* ptr)
{
	if(res == CSIFW_OK) {
		gCounter++;
		printf("\n%d. Raw Data received in APP [%d]\n", gCounter, csi_buff_len);
		printf("\n[APP]LOGGING DATA\n");
		print_buf(csi_raw_buff, csi_buff_len);
		return;
	}
	if(res == CSIFW_ERROR_WIFI_DIS_CONNECTED) {
		printf("<URGENT CALLBACK N/W CHANGE>[APP] wifi disconnected\n");
		printf("<URGENT CALLBACK N/W CHANGE>[APP] so service will PAUSE\n");
		return;
	}
	if (res == CSIFW_ERROR) {
		printf("[APP] Some error\n");
		return;
	}

}

#define LOG_PARSED_DATA 1
static void demo_upd_parsed_data_listener(CSIFW_RES res, int csi_data_len, float *csi_parsed_buff, void* ptr)
{
	// print parsed data buffer
	printf("\n%d. Parsed Data received in APP [%d]\n", gCounter, csi_data_len);
	
	#if LOG_PARSED_DATA
	printf("[APP] Displaying parsed data, csi_data_len: %d\n\n", csi_data_len);
	for(int i = 0; i < csi_data_len; i += 2) {
		printf("sub_carrier:%d[%f,%f], ", i/2, csi_parsed_buff[i], csi_parsed_buff[i+1]);
		}
		printf("\n");
	#endif
	printf("\n\n");
}

int csifw_test_main(void)
{
	printf("\n[APP]: CSI Manager DEFAULT FLOW \n");
	g_parsed_buffer = NULL;
	g_raw_buffer = NULL;
	PRNT_LN1;
	PRNT_LN2;

	PRNT_LN3;
	printf("[APP]: csi_manager_init\n");
	csi_manager_init();
	PRNT_LN3;

	printf("[APP]: csi_service_init\n");
	PRNT_LN2;
	CSIFW_RES res = csi_service_init(demo_get_empty_data_buffer, demo_upd_raw_data_listener, demo_upd_parsed_data_listener, 0, NULL); 
	if (res != CSIFW_OK) {
		PRNT_LN2;
		if (res == CSIFW_ERROR_WIFI_NOT_CONNECTED) {
			printf("\n[APP]: CSIFW-> {WIFI NOT CONNECTED}\n");
		} else {
			printf("\n[APP]: CSI Manager INIT FAIL\n");
			return 0;
		}
	}
	PRNT_LN2;
	printf("\n[APP]: CSI Manager INIT SUCCESS\n");
	printf("[APP]: SLEEP--> 5 seconds\n");
	PRNT_LN2;
	sleep(5);

	PRNT_LN3;
	rtw_csi_action_parm_t config = {0};
	config.group_num = CSI_GROUP_NUM_1;
	config.mode = CSI_MODE_RX_RESP;
	config.accuracy = CSI_ACCU_1BYTE;
	config.ch_opt = CSI_CH_LEGACY;
	config.trig_period = 200;
	config.data_rate = 0xC; //Kernel/os/board/rtl8720e/src/component/wifi/driver/include/rtw_wifi_constants.h L465  MGN_6M

	printf("[APP]: csi_service_set_config\n");
	PRNT_LN2;
	if (csi_service_set_config(config) == CSIFW_ERROR) {
		PRNT_LN2;
		printf("[APP]: CSI Manager CONFIG SET FAIL\n");
		return 0;
	}
	printf("[APP]: CSI Manager CONFIG SET SUCCESS\n");
	PRNT_LN2;

	PRNT_LN3;
	printf("[APP]: csi_service_start\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		PRNT_LN2;
		printf("[APP]: CSI Manager START FAIL\n");
		return 0;
	}
	printf("[APP]: CSI Manager START SUCCESS\n");
	printf("[APP]: SLEEP--> 15 seconds\n");
	PRNT_LN2;
	sleep(15);
	
	PRNT_LN3;
	printf("[APP]: csi_service_stop\n");
	PRNT_LN2;
	sleep(2);
	CSIFW_REASON reason = CSIFW_NORMAL;
	
	if (csi_service_stop(reason)== CSIFW_ERROR) {
		printf("[APP]: CSI Manager STOP FAIL\n");
		PRNT_LN2;
		free(g_parsed_buffer);
		free(g_raw_buffer);
		printf("[APP]: Buffers Freed\n");
		return 0;
		}
	printf("[APP]: CSI Manager STOP SUCCESS\n");
	printf("[APP]: SLEEP--> 10 seconds\n");
	PRNT_LN2;
	sleep(10);

	PRNT_LN3;
	printf("[APP]: csi_service_start\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager START FAIL\n");
		PRNT_LN2;
		free(g_parsed_buffer);
		free(g_raw_buffer);
		printf("[APP]: Buffers Freed\n");
		return 0;
	}
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	sleep(20);

	PRNT_LN3;
	printf("[APP]: csi_service_stop\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_stop(reason) == CSIFW_ERROR) {
		printf("[APP]: CSI Manager STOP FAIL\n");
		PRNT_LN2;
		free(g_parsed_buffer);
		free(g_raw_buffer);
		printf("[APP]: Buffers Freed\n");
		return 0;
	}
	printf("[APP]: SLEEP--> 10 seconds\n");
	PRNT_LN2;
	sleep(10);

	PRNT_LN3;
	printf("[APP]: csi_service_deinit\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_deinit() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager DEINIT FAIL\n");
		PRNT_LN2;
		free(g_parsed_buffer);
		free(g_raw_buffer);
		printf("[APP]: Buffers Freed\n");
		return 0;
	}
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	sleep(20);
	PRNT_LN2;
	free(g_parsed_buffer);
	free(g_raw_buffer);
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
	
	return 0;
}

void csi_manager_csifw_test_init(void)
{
	// SYSTEM_LOG_INFO(SYSTEM_TAG_API, "apps 1");
	// SYSTEM_LOG_INFO(SYSTEM_TAG_API, "apps 2");
	// SYSTEM_LOG_INFO(SYSTEM_TAG_API, "apps 3");
	// SYSTEM_LOG_INFO(SYSTEM_TAG_API, "apps 4");
	sleep(1);
	csifw_test_main(0);
}
