/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __INCLUDE_TINYARA_LWNL_H__
#define __INCLUDE_TINYARA_LWNL_H__

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LWNL_PATH "/dev/lwnl"

#define LWNL_TAG "[LWNL]"

#define LWNL_ERR										\
	do {												\
		nldbg(LWNL_TAG"[ERR] %s: %d line err(%s)\n",	\
			  __FILE__, __LINE__, strerror(errno));		\
	} while (0)

#define LWNL_ENTER											\
	do {													\
		nldbg(LWNL_TAG"--->%s:%d\n", __FILE__, __LINE__);	\
	} while (0)

#define LWNL_LEAVE											\
	do {													\
		nldbg(LWNL_TAG"<---%s:%d\n", __FILE__, __LINE__);	\
	} while (0)




/* Light-weight netlink domain definition */
#define AF_LWNL 3

/*	Event type */
#define LWNL_ROUTE 1
#define LWNL_NI_NAME_SIZE 7

/* IOCTL commands ***********************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef enum {
	LWNL_FAIL = -1,
	LWNL_SUCCESS,
	LWNL_INVALID_ARGS,
	LWNL_TIMEOUT,
	LWNL_BUSY,
	LWNL_FILE_ERROR,
	LWNL_ALREADY_CONNECTED,
} lwnl_result_e;

typedef struct {
	void *data;
	uint32_t data_len;
	lwnl_result_e res;
	uint8_t name[LWNL_NI_NAME_SIZE];
} lwnl_data;

typedef enum {
	// common
	LWNL_GET_ADDR_INFO,
	// wifi
	LWNL_INIT,
	LWNL_DEINIT,
	LWNL_SCAN_AP,
	LWNL_GET_INFO,
	LWNL_REGISTER_CALLBACK,
	LWNL_SET_AUTOCONNECT,
	LWNL_START_STA,
	LWNL_CONNECT_AP,
	LWNL_DISCONNECT_AP,
	LWNL_START_SOFTAP,
	LWNL_STOP_SOFTAP,
	LWNL_REQ_UNKNOWN,
} lwnl_req;

typedef struct {
	uint8_t name[LWNL_NI_NAME_SIZE];
	lwnl_req req_type;
	uint32_t data_len;
	void *data;
	lwnl_result_e res;
} lwnl_msg;

// Todo lwnl return status need to be specified by interface type.
typedef enum {
	LWNL_STA_CONNECTED,
	LWNL_STA_CONNECT_FAILED,
	LWNL_STA_DISCONNECTED,
	LWNL_SOFTAP_STA_JOINED,
	LWNL_SOFTAP_STA_LEFT,
	LWNL_SCAN_DONE,
	LWNL_SCAN_FAILED,
	LWNL_EXIT,
	LWNL_UNKNOWN,
} lwnl_cb_status;

typedef struct {
	lwnl_cb_status status;
	void *data;
	uint32_t data_len;
	bool md;
} lwnl_cb_data;

struct lwnl_lowerhalf_s;
struct lwnl_upperhalf_s;

/* Callback */
typedef CODE void (*lwnl_callback_t)(struct lwnl_lowerhalf_s *dev, lwnl_cb_status status, void *buffer);

typedef struct lwnl_lowerhalf_s {
	struct lwnl_ops_s *ops;
	struct lwnl_upperhalf_s *parent;
	lwnl_callback_t cbk;
	void *priv;
};

#ifndef CONFIG_NET_NETMGR
typedef enum {
	NM_LOOPBACK,
	NM_WIFI,
	NM_ETHERNET,
	NM_UNKNOWN,
} netdev_type;

typedef enum {
	/** Delete a filter entry */
	NM_DEL_MAC_FILTER = 0,
	/** Add a filter entry */
	NM_ADD_MAC_FILTER = 1
} netdev_mac_filter_action;

struct netdev {
	char ifname[IFNAMSIZ];
	netdev_type type;
	void *ops;
};
#endif //CONFIG_NET_NETMGR
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Registrations */
int lwnl_register(struct lwnl_lowerhalf_s *ldev);
int lwnl_unregister(struct lwnl_lowerhalf_s *ldev);
int lwnl_register_dev(struct netdev *dev);
int lwnl_postmsg(lwnl_cb_status status, void *buffer);

#ifndef CONFIG_NET_NETMGR
int lwnl_register_dev(struct netdev *dev);
#endif // CONFIG_NET_NETMGR

#endif /* __INCLUDE_TINYARA_LWNL_H__ */
