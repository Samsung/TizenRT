//#include "rtl8195a.h"
#include <cmsis.h>
#include "build_info.h"
#ifdef PLATFORM_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#endif
#include "osdep_service.h"
#include "lwip_netconf.h"
#include "ethernet_api.h"
#include "wifi_intf_drv_to_lwip.h"
#include "ethernet_mii.h"
#include "platform_opts.h"
#include "ethernet_ex_api.h"
#include "rtw_ethernet.h"
#include "usbh_cdc_ecm_hal.h"

/**
  \brief  Defines Ethernet interrupt event.
*/
enum  eth_int_event_e {
	EthTxDone       = 0,
	EthRxDone       = 1,
	EthLinkUp       = 2,
	EthLinkDown     = 3,
	EthRx_RDU       = 4
};

#if ((defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))||(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))||(defined(CONFIG_AMEBAD2) && (CONFIG_AMEBAD2 == 1)))
#define ETH_TXDONE              EthTxDone
#define ETH_RXDONE              EthRxDone
#define ETH_LINKUP              EthLinkUp
#define ETH_LINKDOWN            EthLinkDown

#define MII_TX_DESC_NO					8
#define MII_RX_DESC_NO					4
#define MII_BUF_SIZE					1600
#endif  // end of "#if defined(CONFIG_PLATFORM_8195BHP)"

static _sema mii_rx_sema;
static _mutex mii_tx_mutex;
#if (defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1))
extern volatile u32 ethernet_unplug;
#elif ((defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))||(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))||(defined(CONFIG_AMEBAD2) && (CONFIG_AMEBAD2 == 1)))
volatile u32 ethernet_unplug = 0;
#endif

extern void *pvPortMallocExt(size_t xWantedSize, int idx);
extern int ethernet_init(void);
extern int ethernet_receive(void);
extern int ethernet_read(char *data, int size);
extern void ethernetif_mii_recv(struct netif *netif, int total_len);
extern void ethernet_address(char *mac);
extern u16 usbh_cdc_ecm_get_receive_mps(void);
extern int ethernet_write(const char *data, int size);
extern int ethernet_send(void);
extern u8 usbh_cdc_ecm_senddata(u8 *buf, u32 len);
extern u8 usbh_cdc_ecm_get_sendflag(void);
extern u8 usbh_cdc_ecm_do_init(usb_report_usbdata cb_handle);
extern struct netif  xnetif[NET_IF_NUM];

static u8 TX_BUFFER[1536];
static u8 RX_BUFFER[1536];

static u8 *pTmpTxDesc = NULL;
static u8 *pTmpRxDesc = NULL;
static u8 *pTmpTxPktBuf = NULL;
static u8 *pTmpRxPktBuf = NULL;

int ethernet_init_done = 0;
int dhcp_ethernet_mii = 1;
int ethernet_if_default = 0;
int link_is_up = 0;


link_up_down_callback p_link_change_callback = 0;

extern int lwip_init_done;

static _sema mii_linkup_sema;

#include "device_lock.h"
void hal_eth_otp_lock(void)
{
	device_mutex_lock(RT_DEV_LOCK_EFUSE);
	rtw_enter_critical(NULL, NULL);
}
void hal_eth_otp_unlock(void)
{
	rtw_exit_critical(NULL, NULL);
	device_mutex_unlock(RT_DEV_LOCK_EFUSE);
}

void mii_rx_thread(void *param)
{
	UNUSED(param);
	u32 len = 0;
	u8 *pbuf = RX_BUFFER;
	while (1) {
		if (rtw_down_sema(&mii_rx_sema) == _FAIL) {
			printf("%s, Take Semaphore Fail\n", __FUNCTION__);
			goto exit;
		}
		// continues read the rx ring until its empty
		while (1) {
			len = ethernet_receive();
			if (len) {
				//dbg_printf ("mii_recv len = %d\n\r", len);
				ethernet_read((char *)pbuf, len);
// calculate the time duration
				ethernetif_mii_recv(&xnetif[ETHERNET_IDX], len);
				//__rtl_memDump_v1_00(pbuf, len, "ethernet_receive Data:");
				//rtw_memset(pbuf, 0, len);
			} else if (len == 0) {
				break;
			}
		}
	}
exit:
	rtw_free_sema(&mii_rx_sema);
	vTaskDelete(NULL);
}

void mii_intr_thread(void *param)
{
	UNUSED(param);
	u32 dhcp_status = 0;

	while (1) {
		if (rtw_down_sema(&mii_linkup_sema) == _FAIL) {
			printf("%s, Take Semaphore Fail\n", __FUNCTION__);
			break;
		}

		//Used to process there is no cable plugged in when power-on
//		if(1 == ethernet_unplug){
//			if(p_link_change_callback)
//				p_link_change_callback(link_is_up);
//		}

		if (1 == ethernet_init_done) {
			if (link_is_up) {
				printf("...Link up\n");
				if (dhcp_ethernet_mii == 1) {
					dhcp_status = LwIP_DHCP(ETHERNET_IDX, DHCP_START);
				}
				if (DHCP_ADDRESS_ASSIGNED == dhcp_status) {
					if (1 == ethernet_if_default) {
						netif_set_default(&xnetif[ETHERNET_IDX]);    //Set default gw to ether netif
					} else {
						netif_set_default(&xnetif[WLAN_IDX]);
					}
				}
			} else {
				printf("...Link down\n");
				netif_set_default(&xnetif[WLAN_IDX]);	//Set default gw to wlan netif
#if CONFIG_LWIP_LAYER
				LwIP_ReleaseIP(ETHERNET_IDX);
#endif
			}

			if (p_link_change_callback) {
				p_link_change_callback(link_is_up);
			}
		}
	}

	rtw_free_sema(&mii_linkup_sema);
	vTaskDelete(NULL);
}


void mii_intr_handler(u32 Event, u32 Data)
{
	UNUSED(Data);
	switch (Event) {
	case ETH_TXDONE:
		//printf ("TX Data = %d\n", Data);
		break;
	case ETH_RXDONE:
		//printf ("\r\nRX Data = %d\n", Data);
		// wake up rx thread to receive data
		rtw_up_sema_from_isr(&mii_rx_sema);
		break;
	case ETH_LINKUP:
		printf("Link Up\n");
		link_is_up = 1;
		rtw_up_sema_from_isr(&mii_linkup_sema);
		break;
	case ETH_LINKDOWN:
		printf("Link Down\n");
		link_is_up = 0;
		rtw_up_sema_from_isr(&mii_linkup_sema);
		break;
	default:
		printf("Unknown event !!\n");
		break;
	}
}

void ethernet_demo(void *param)
{
	UNUSED(param);
	u8 mac[6];
	/* Initilaize the LwIP stack */
	// can not init twice
	if (!lwip_init_done) {
		LwIP_Init();
	}
	printf("LWIP Init done\n");

	ethernet_irq_hook(mii_intr_handler);

	if (pTmpTxDesc) {
		free(pTmpTxDesc);
		pTmpTxDesc = NULL;
	}
	if (pTmpRxDesc) {
		free(pTmpRxDesc);
		pTmpRxDesc = NULL;
	}
	if (pTmpTxPktBuf) {
		free(pTmpTxPktBuf);
		pTmpTxPktBuf = NULL;
	}
	if (pTmpRxPktBuf) {
		free(pTmpRxPktBuf);
		pTmpRxPktBuf = NULL;
	}

#if ((defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))||(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)))
	//amebapro ethernet Tx/Rx buffer must be placed in internal ram, not placed in external LPDDR
	pTmpTxDesc = (u8 *)pvPortMallocExt(/*MII_TX_DESC_CNT*/MII_TX_DESC_NO * ETH_TX_DESC_SIZE, 1);
	pTmpRxDesc = (u8 *)pvPortMallocExt(/*MII_RX_DESC_CNT*/MII_RX_DESC_NO * ETH_RX_DESC_SIZE, 1);
	pTmpTxPktBuf = (u8 *)pvPortMallocExt(/*MII_TX_DESC_CNT*/MII_TX_DESC_NO * ETH_PKT_BUF_SIZE, 1);
	pTmpRxPktBuf = (u8 *)pvPortMallocExt(/*MII_RX_DESC_CNT*/MII_RX_DESC_NO * ETH_PKT_BUF_SIZE, 1);

#else
	pTmpTxDesc = (u8 *)malloc(/*MII_TX_DESC_CNT*/MII_TX_DESC_NO * ETH_TX_DESC_SIZE);
	pTmpRxDesc = (u8 *)malloc(/*MII_RX_DESC_CNT*/MII_RX_DESC_NO * ETH_RX_DESC_SIZE);
	pTmpTxPktBuf = (u8 *)malloc(/*MII_TX_DESC_CNT*/MII_TX_DESC_NO * ETH_PKT_BUF_SIZE);
	pTmpRxPktBuf = (u8 *)malloc(/*MII_RX_DESC_CNT*/MII_RX_DESC_NO * ETH_PKT_BUF_SIZE);

#endif  // end of "#if defined(CONFIG_PLATFORM_8195BHP)"    

	if (pTmpTxDesc == NULL || pTmpRxDesc == NULL || pTmpTxPktBuf == NULL || pTmpRxPktBuf == NULL) {
		printf("TX/RX descriptor malloc fail\n");
		return;
	}
	memset(pTmpTxDesc, 0, MII_TX_DESC_NO * ETH_TX_DESC_SIZE);
	memset(pTmpRxDesc, 0, MII_RX_DESC_NO * ETH_RX_DESC_SIZE);
	memset(pTmpTxPktBuf, 0, MII_TX_DESC_NO * ETH_PKT_BUF_SIZE);
	memset(pTmpRxPktBuf, 0, MII_RX_DESC_NO * ETH_PKT_BUF_SIZE);
	//size 160        128     12288   12288

	ethernet_set_descnum(MII_TX_DESC_NO, MII_RX_DESC_NO);
	printf("TRX descriptor number setting done\n");
	ethernet_trx_pre_setting(pTmpTxDesc, pTmpRxDesc, pTmpTxPktBuf, pTmpRxPktBuf);
	printf("TRX pre setting done\n");

#if ((defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1))||(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)))
	u8 id[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
	ethernet_set_address((char *)id);
#endif  // end of "#if defined(CONFIG_PLATFORM_8195BHP)"

	ethernet_init();

#if ((defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1))||(defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1)))
	DBG_INFO_MSG_OFF(_DBG_MII_);
	DBG_WARN_MSG_OFF(_DBG_MII_);
	DBG_ERR_MSG_ON(_DBG_MII_);
#endif

#if defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1)
	DBG_INFO_MSG_OFF(_DBG_ETH_);
	DBG_WARN_MSG_OFF(_DBG_ETH_);
	DBG_ERR_MSG_ON(_DBG_ETH_);
#endif  // end of "#if defined(CONFIG_PLATFORM_8195BHP)"

	/*get mac*/
	ethernet_address((char *)mac);
	memcpy((void *)xnetif[ETHERNET_IDX].hwaddr, (void *)mac, 6);

	rtw_init_sema(&mii_rx_sema, 0);
	rtw_mutex_init(&mii_tx_mutex);

	netif_set_up(&xnetif[ETHERNET_IDX]);
	netif_set_link_up(&xnetif[ETHERNET_IDX]);

	if (xTaskCreate(mii_rx_thread, ((const char *)"mii_rx_thread"), 1024, NULL, tskIDLE_PRIORITY + 5, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(mii_rx_thread) failed", __FUNCTION__);
	}

	printf("\nEthernet_mii Init done, interface %d", ETHERNET_IDX);

	if (dhcp_ethernet_mii == 1) {
		LwIP_DHCP(ETHERNET_IDX, DHCP_START);
		netif_set_default(&xnetif[ETHERNET_IDX]);  //Set default gw to ether netif
	}
	ethernet_init_done = 1;

	vTaskDelete(NULL);
}

int ethernet_is_linked(void)
{
	if ((link_is_up == 1) && (1 == ethernet_init_done)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int ethernet_is_unplug(void)
{
	if (ethernet_unplug == 1) {
		return TRUE;
	} else {
		return FALSE;
	}
}


void ethernet_mii_init(void)
{
	printf("\ninitializing Ethernet_mii......\n");

	// set the ethernet interface as default
	ethernet_if_default = 1;
	vSemaphoreCreateBinary(mii_linkup_sema);
	if (xTaskCreate((TaskFunction_t)mii_intr_thread, "DHCP_START_MII", 1024, NULL, 3, NULL) != pdPASS) {
		printf("Cannot create demo task\n\r");
	}

	if (xTaskCreate((TaskFunction_t)ethernet_demo, "ETHERNET DEMO", 1024, NULL, 2, NULL) != pdPASS) {
		printf("Cannot create demo task\n\r");
	}
#if 0
	extern void ethernet_wlan_iperf_test_task(void *param);
	if (xTaskCreate((TaskFunction_t)ethernet_wlan_iperf_test_task, "wifi_entry_task", 1024, NULL, 2, NULL) != pdPASS) {
		dbg_printf("\n\r%s xTaskCreate(wifi_entry_task) failed", __FUNCTION__);
	}
#endif

}

#define DST_MAC_LEN		6
#define SRC_MAC_LEN		6
#define PROTO_TYPE_LEN	2	//	protocol type 
#define IP_LEN_OFFSET	2	//	offset of total length field in IP packet 


static u32 rx_buffer_saved_data = 0;
static u32 pkt_total_len = 0;
static u16 eth_type;	//protocol type

void rltk_mii_init(void)
{
	rtw_mutex_init(&mii_tx_mutex);
}

u8 rltk_mii_recv_data(u8 *buf, u32 total_len, u32 *frame_length)
{
	u8 *pbuf;
	u32 pkt_len_index = DST_MAC_LEN + SRC_MAC_LEN + PROTO_TYPE_LEN;
	u16 usb_receive_mps = usbh_cdc_ecm_get_receive_mps();	//only 512 bytes is supported now.

	if (0 == pkt_total_len) { //first packet
		pbuf = RX_BUFFER;
		rtw_memcpy((void *)pbuf, buf, total_len);
		if (total_len != usb_receive_mps) { //should finish
			*frame_length = total_len;
			return 1;
		} else { //get the total length
			rx_buffer_saved_data = total_len;
			//should check the vlan header
			eth_type = buf[DST_MAC_LEN + SRC_MAC_LEN] * 256 + buf[DST_MAC_LEN + SRC_MAC_LEN + 1];

			if (eth_type == ETH_P_IP) {
				pkt_total_len =  buf[pkt_len_index + IP_LEN_OFFSET] * 256 + buf[pkt_len_index + IP_LEN_OFFSET + 1];
			}
		}
	} else {
		pbuf = RX_BUFFER + rx_buffer_saved_data;
		if (total_len > 0) {
			rtw_memcpy((void *)pbuf, buf, total_len);
		}
		rx_buffer_saved_data += total_len;
		if (total_len != usb_receive_mps) {
			//should finish
			*frame_length = rx_buffer_saved_data;
			pkt_total_len = 0;
			return 1;
		}
	}

	return 0;
}
u8 rltk_mii_recv_data_check(u8 *mac, u32 frame_length)
{
	UNUSED(frame_length);
	u8 *pbuf = RX_BUFFER;
	u8 checklen = 0;
	u8 multi[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	if (memcmp(mac, pbuf, 6) == 0 || memcmp(multi, pbuf, 6) == 0) {
		checklen = 7 ;
	} else {
		checklen = 6;
	}

	return (checklen == 6) ? (0) : (1);
}


void rltk_mii_recv(struct eth_drv_sg *sg_list, int sg_len)
{
	struct eth_drv_sg *last_sg;
	u8 *pbuf = RX_BUFFER;

	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		if (sg_list->buf != 0) {
			rtw_memcpy((void *)(sg_list->buf), pbuf, sg_list->len);
			pbuf += sg_list->len;
		}
	}
}


s8 rltk_mii_send(struct eth_drv_sg *sg_list, int sg_len, int total_len)
{
	UNUSED(total_len);
	int ret = 0;
	struct eth_drv_sg *last_sg;
	u8 *pdata = TX_BUFFER;
	u8	retry_cnt = 0;
	u32 size = 0;
	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		rtw_memcpy(pdata, (void *)(sg_list->buf), sg_list->len);
		pdata += sg_list->len;
		size += sg_list->len;
	}
	pdata = TX_BUFFER;
	//DBG_8195A("mii_send len= %d\n\r", size);
	rtw_mutex_get(&mii_tx_mutex);
	while (1) {
		ret = ethernet_write((char *)pdata, size);
		if (ret > 0) {
			ethernet_send();
			ret = 0;
			break;
		}
		if (++retry_cnt > 3) {
			printf("TX drop\n\r");
			ret = -1;
		} else {
			rtw_udelay_os(1);
		}
	}
	rtw_mutex_put(&mii_tx_mutex);

	return ret;
}

s8 rltk_mii_send_usb_ecm(struct eth_drv_sg *sg_list, int sg_len, int total_len)
{
	UNUSED(total_len);
	int ret = 0;
	struct eth_drv_sg *last_sg;
	u8 *pdata = TX_BUFFER;
	u8	retry_cnt = 0;
	u32 size = 0;
	for (last_sg = &sg_list[sg_len]; sg_list < last_sg; ++sg_list) {
		rtw_memcpy(pdata, (void *)(sg_list->buf), sg_list->len);
		pdata += sg_list->len;
		size += sg_list->len;
	}
	pdata = TX_BUFFER;
	rtw_mutex_get(&mii_tx_mutex);
	while (1) {
		ret = usbh_cdc_ecm_senddata(pdata, size);
		if (ret == 0) {
			break;
		}
		if (++retry_cnt > 3) {
			printf("TX drop\n\r");
			ret = -1;
			break;
		} else {
			rtw_udelay_os(1);
		}
	}
	rtw_mutex_put(&mii_tx_mutex);

	//wait reply success,wait signal
	while (!usbh_cdc_ecm_get_sendflag()) {
		rtw_msleep_os(1);
	}
	return ret;
}

extern void ethernetif_mii_recv_usbdata(u8 *buf, u32 total_len);
u8 cdc_ecm_do_init(void)
{
	return usbh_cdc_ecm_do_init(ethernetif_mii_recv_usbdata);
}

#define ____TEST____
/* used for test */
#if 0
#include "wifi_constants.h"
struct iperf_data_t {
	uint64_t total_size;
	uint64_t bandwidth;
	int  server_fd;
	int  client_fd;
	uint32_t buf_size;
	uint32_t time;
	uint32_t report_interval;
	uint16_t port;
	uint8_t  server_ip[16];
	uint8_t  start;
	uint8_t  tos_value;
};

/*ATWT=-c,192.168.31.111,-t,10*/
int ethernet_wlan_iperf_test()
{
	char server_ip[] = "192.168.31.111";
	u16  time = 10;
	struct iperf_data_t tcp_client_data;

	dbg_printf("\n###################ethernet wlan iperf test !!!...\n");
	memset(&tcp_client_data, 0, sizeof(tcp_client_data));

	strncpy(tcp_client_data.server_ip, server_ip, (strlen(server_ip) > 16) ? 16 : strlen(server_ip));
	tcp_client_data.time = time;
	tcp_client_data.bandwidth = 268459220;
	tcp_client_data.buf_size = 1460;
	tcp_client_data.client_fd = 0;
	tcp_client_data.port = 5001;
	tcp_client_data.report_interval = -1;
	tcp_client_data.server_fd = 0;
	tcp_client_data.start = 1;
	tcp_client_data.tos_value = 0;
	tcp_client_data.total_size = 0;

	tcp_client_func(tcp_client_data);

	return 0;
}

void ethernet_wlan_iperf_test_task(void *param)
{
	dbg_printf("\n#########################ethernet wlan iperf test task...\n");

	/* used for test */
	p_link_change_callback = (link_up_down_callback)ethernet_wlan_iperf_test;

	/*sys_reset is not necessary, upper layer can define p_link_change_callback on demand*/
	//p_link_change_callback = (link_up_down_callback)sys_reset();
	while (1) {
		if (((wifi_is_ready_to_transceive(RTW_STA_INTERFACE) == RTW_SUCCESS) && ethernet_is_unplug()) || (ethernet_is_linked())) {
			break;
		} else {
			vTaskDelay(500);
		}
	}
	ethernet_wlan_iperf_test();

	dbg_printf("\r\n[%s] task del", __func__);
	vTaskDelete(NULL);
}

#endif

