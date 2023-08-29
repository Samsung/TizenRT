#ifndef WIFI_CONF_CAST_H
#define WIFI_CONF_CAST_H

#include "rtw_wifi_constants.h"
#include "wifi_ind.h"
#include "osdep_service.h"
#include <rtw_timer.h>
#include "err.h"
#include "ethernetif.h"


#if defined(CONFIG_AS_INIC_AP)
#include "inic_ipc_host_trx.h"
#endif


#ifndef ETH_LEN
#define ETH_LEN		6
#endif

#define WIFI_CAST_MAX_PASSWORD_LEN		64
#define WIFI_CAST_MAX_SSID_LEN			32
#define WIFI_CAST_MAX_NODE_NUM			8

/* Definitions for error constants. */
#define WIFI_CAST_OK					0
#define WIFI_CAST_ERR					-1

#define WIFI_CAST_ERR_STARTAP_FAIL		0x101
#define WIFI_CAST_ERR_CONNECTAP_FAIL	0x102
#define WIFI_CAST_ERR_INVALID_SSID		0x103
#define WIFI_CAST_ERR_INVALID_PASSWIFI_CAST	0x104
#define WIFI_CAST_ERR_NO_MEMORY			0x105
#define WIFI_CAST_ERR_INVALID_DATA		0x106
#define WIFI_CAST_ERR_NODE_EXIST		0x105



typedef int wcast_err_t;

/**
 * @brief Wifi cast node type.
 */
typedef enum {
	WIFI_CAST_SLAVE_MODE = 0,
	WIFI_CAST_MASTER_MODE = 1,
} wifi_cast_node_type_t;

/**
 * @brief Wifi cast node trx mode.
 */
typedef enum {
	WIFI_CAST_TRX_UNICAST	= 0,
	WIFI_CAST_TRX_BRODCAST,
	WIFI_CAST_TRX_MONITOR,
} wifi_cast_trx_mode_t;

/**
 * @brief Wifi cast node status.
 */
typedef enum {
	WIFI_CAST_NODE_OFFLINE = 0,
	WIFI_CAST_NODE_ONLINE,
} wifi_cast_node_status_t;

/**
 * @brief Wifi cast node parameters.
 */
typedef struct wifi_cast_node {
	unsigned char 	mac[ETH_ALEN]; 	   /* node mac address */
	void 			*priv;				/* user define */
} wifi_cast_node_t;

/**
 * @brief Wifi cast node information parameters.
 */
typedef struct wifi_cast_node_info {
	signed char 	rssi;
	unsigned char 	status;		/* node status, offline/online */
	unsigned int 	tx_count;	/* tx packet count */
	unsigned int 	rx_count;	/* rx packet count */
	unsigned int 	ctrl_seq;	/* ctrl packet sequence */
	unsigned int 	data_seq;	/* data packet sequence */
	unsigned int 	timeout;
	unsigned int 	pkt_loss_rate;
} wifi_cast_node_info_t;


/**
  * @brief     Wifi Delivery data receive callback function
  * @param     pnode node parameters
  * @param     data receive data
  * @param     data_len length of receive data
  * @param	   userdata user defined data
  */
typedef void (*wifi_cast_recv_cb_t)(wifi_cast_node_t *pnode, unsigned char *buf, unsigned int len);

/**
  * @brief     Wifi Delivery data send callback function
  * @param     pnode node parameters
  * @param     data send data
  * @param     data_len length of send data
  * @param	   userdata user defined data
  */
typedef void (*wifi_cast_send_cb_t)(wifi_cast_node_t *pnode, unsigned char *buf, unsigned int len);

/**
  * @brief     Wifi Delivery node status check callback function
  * @param     pnode node parameters
  * @param     id
  *					- for master, node status
  *					- for slave, dissconnect reason code
  */
typedef void (*wifi_cast_node_status_cb_t)(wifi_cast_node_t *pnode, unsigned short id);


/**
 * @brief Wifi Delivery adapter.
 */
typedef struct wifi_cast_adapter {
	wifi_cast_node_type_t 	node_type;   		/* device type, ap/sta */
	wifi_cast_trx_mode_t 	trx_mode;			/* unicast, broadcast, monitor */
	unsigned char 	powersave_en;		/* device enable powersave or not */
	unsigned char 	encrypt;			/* data send/receive is encrypted or not */
	unsigned char 	ssid[WIFI_CAST_MAX_SSID_LEN + 1];	/**< softap ssid */
	unsigned char 	password[WIFI_CAST_MAX_PASSWORD_LEN + 1]; /**< softap password */
	unsigned char 	channel;			/**< softap channel */
	unsigned char 	retrylimit;			/**< hardware retry limit times */
	unsigned int 	hb_interval; 		/* node heartbeat interval */
	unsigned int 	hb_timeout; 		/* node heartbeat timeout */
	wifi_cast_recv_cb_t recv_cb; 			/* receive callback */
	wifi_cast_send_cb_t send_cb; 				/* send callback */
	wifi_cast_node_status_cb_t node_status_cb;	/* node status callback */
} wifi_cast_adapter_t;


#define WIFI_CAST_ERROR_CHECK(x) \
	do {                                      \
        wcast_err_t err_rc_ = (x);                                        \
        if (err_rc_ != WIFI_CAST_OK) {                              \
            printf("%s, filename: %d, line: %d, err: %d\n", __FUNCTION__, __FILE__, __LINE__,        \
                                    err_rc_);                 \
        }                                                               \
    } while(0)

enum {
	WIFI_CAST_ERROR = 0,
	WIFI_CAST_WARN,
	WIFI_CAST_INFO,
	WIFI_CAST_DEBUG,
	WIFI_CAST_NONE,
};

#define WIFI_CAST_DEBUG_ON			1
#define WIFI_CAST_DEBUG_LEVEL		WIFI_CAST_INFO


#define WIFI_CAST_PRINT_DATA(level, data, len) \
do{\
	if (WIFI_CAST_DEBUG_LEVEL >= level) {\
		for (int i = 0; i < len; i++) {\
			printf("%02X ", *(data + i));\
		}\
		printf("\n");\
	}\
}while(0)


/**
  * @brief     Initialize wifi cast function
  * @param     padapter wifi cast adapter, should be set by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_init(wifi_cast_adapter_t *padapter);

/**
  * @brief     De-Initialize wifi cast function
  * @param     padapter wifi cast adapter, should be set by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_deinit(wifi_cast_adapter_t *padapter);

/**
  * @brief     Add node to node list
  * @param     pnode node need to be added, should be set by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR_NO_MEMORY: out of memory or out of maximum node number
  */
wcast_err_t wifi_cast_add_node(wifi_cast_node_t *pnode);

/**
  * @brief     Delete node from node list
  * @param     pnode node need to be deleted, should be set by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_del_node(wifi_cast_node_t *pnode);

/**
  * @brief     Get node information from node list
  * @param     pnode node need to be checked, mac address should be set by user
  * @return
  * 		   - If exist, return target node information else return NULL
  */
wifi_cast_node_t *wifi_cast_get_node_info(wifi_cast_node_t *pnode);

/**
  * @brief     Send wifi cast data
  * @param     pnode node need to be deleted, should be set by user
  * @param	   data data to send
  * @param     data_len length of data
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_send(wifi_cast_node_t *pnode, unsigned char *data, int data_len);

/**
  * @brief     Get wifi cast timestamp
  * @param	   port_id 1 for master node, 0 for slave node
  * @return
  * 		   - timestamp, unit: us
  */
unsigned int wifi_cast_get_timestamp(unsigned char port_id);

/**
  * @brief     Set wifi cast softap channel
  * @param     channel softap channel, 2.4G and 5G channels are supported
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_set_channel(unsigned char channel);

/**
  * @brief     Set wifi cast data receive callback function
  * @param     recv_cb receive callback function, defined by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_register_recv_cb(wifi_cast_recv_cb_t recv_cb);

/**
  * @brief     Set wifi cast data send callback function
  * @param     send_cb send callback function, defined by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_register_send_cb(wifi_cast_send_cb_t send_cb);

/**
  * @brief     Set wifi cast node status check callback function
  * @param     node_status_cb node status check callback function, defined by user
  * @return
  * 		   - WIFI_CAST_OK : succeed
  * 		   - WIFI_CAST_ERR: failed
  */
wcast_err_t wifi_cast_register_node_status_cb(wifi_cast_node_status_cb_t node_status_cb);
#endif
