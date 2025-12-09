/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <net/if.h>
#include <debug.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/kthread.h>
#include <tinyara/net/if/ble.h>
#include "vble_handler.h"

#define VBLE_MSG_QUEUE_NAME "/dev/vble"

/****************************************************************************
 * Private Types
 ****************************************************************************/
/*** Common ***/
static trble_result_e vdev_init(struct bledev *dev, trble_client_init_config *client, trble_server_init_config *server);
static trble_result_e vdev_deinit(struct bledev *dev);
static trble_result_e vdev_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN]);
static trble_result_e vdev_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count);
static trble_result_e vdev_delete_bond(struct bledev *dev, trble_addr *addr);
static trble_result_e vdev_delete_bond_all(struct bledev *dev);
static trble_result_e vdev_conn_is_active(struct bledev *dev, trble_conn_handle con_handle, bool *is_active);
static trble_result_e vdev_conn_is_any_active(struct bledev *dev, bool *is_active);
static trble_result_e vdev_ioctl(struct bledev *dev, trble_msg_s *msg);

/*** Scanner(Observer) ***/
static trble_result_e vdev_start_scan(struct bledev *dev, trble_scan_filter *filter);
static trble_result_e vdev_stop_scan(struct bledev *dev);
static trble_result_e vdev_scan_whitelist_add(struct bledev *dev, trble_addr *addr);
static trble_result_e vdev_scan_whitelist_delete(struct bledev *dev, trble_addr *addr);
static trble_result_e vdev_scan_whitelist_clear_all(struct bledev *dev);

/*** Central(Client) ***/
static trble_result_e vdev_client_connect(struct bledev *dev, trble_conn_info *conn_info);
static trble_result_e vdev_client_disconnect(struct bledev *dev, trble_conn_handle con_handle);
static trble_result_e vdev_client_disconnect_all(struct bledev *dev);
static trble_result_e vdev_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list);
static trble_result_e vdev_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device);
static trble_result_e vdev_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle);
static trble_result_e vdev_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data);
static trble_result_e vdev_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);
static trble_result_e vdev_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data);

/*** Peripheral(Server) ***/
static trble_result_e vdev_get_profile_count(struct bledev *dev, uint16_t *count);
static trble_result_e vdev_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data);
static trble_result_e vdev_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
static trble_result_e vdev_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data);
static trble_result_e vdev_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode);
static trble_result_e vdev_server_disconnect(struct bledev *dev, trble_conn_handle con_handle);
static trble_result_e vdev_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN]);
static trble_result_e vdev_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle);

/*** Advertiser(Broadcaster) ***/
static trble_result_e vdev_set_adv_data(struct bledev *dev, trble_data *data);
static trble_result_e vdev_set_adv_resp(struct bledev *dev, trble_data *data);
static trble_result_e vdev_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr);
static trble_result_e vdev_set_adv_interval(struct bledev *dev, uint16_t interval);
static trble_result_e vdev_start_adv(struct bledev *dev);
static trble_result_e vdev_stop_adv(struct bledev *dev);


__attribute__((weak)) unsigned char rltk_wlan_running(unsigned char idx)
{
    return 1;
}


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
static struct trble_ops g_trble_drv_ops = {
    // Common
    vdev_init,
    vdev_deinit,
    vdev_get_mac_addr,
    vdev_get_bonded_device,
    vdev_delete_bond,
    vdev_delete_bond_all,
    vdev_conn_is_active,
    vdev_conn_is_any_active,
    vdev_ioctl,

    // Observer
    vdev_start_scan,
    vdev_stop_scan,
    vdev_scan_whitelist_add,
    vdev_scan_whitelist_delete,
    vdev_scan_whitelist_clear_all,

    // Client
    vdev_client_connect,
    vdev_client_disconnect,
    vdev_client_disconnect_all,
    vdev_connected_device_list,
    vdev_connected_info,
    vdev_operation_enable_notification,
    vdev_operation_read,
    vdev_operation_write,
    vdev_operation_write_no_response,

    // Server
    vdev_get_profile_count,
    vdev_charact_notify,
    vdev_attr_set_data,
    vdev_attr_get_data,
    vdev_attr_reject,
    vdev_server_disconnect,
    vdev_get_mac_addr_by_conn_handle,
    vdev_get_conn_handle_by_addr,

    // Broadcaster
    vdev_set_adv_data,
    vdev_set_adv_resp,
    vdev_set_adv_type,
    vdev_set_adv_interval,
    vdev_start_adv,
    vdev_stop_adv,
};

struct bledev *g_vble_dev = NULL;
/*
 * Callback
 */
static inline int _create_message(struct vble_msg **vmsg, struct vble_req *req)
{
    struct vble_msg *tmsg = (struct vble_msg *)kmm_malloc(sizeof(struct vble_msg));
    if (!tmsg) {
        VBLE_ERROR(0);
        return -1;
    }
    sem_t *signal = (sem_t *)kmm_malloc(sizeof(sem_t));
    if (!signal) {
        kmm_free(tmsg);
        VBLE_ERROR(0);
        return -1;
    }
    sem_init(signal, 0, 0);

    tmsg->req = req;
    tmsg->signal = signal;

    *vmsg = tmsg;

    return 0;
}

static inline int _destroy_message(struct vble_msg *msg)
{
    if (msg) {
        if (msg->signal) {
            sem_destroy(msg->signal);
            kmm_free(msg->signal);
        }
        kmm_free(msg);
    }

    return 0;
}

static inline void _wait_message(struct vble_msg *msg)
{
    int res = sem_wait(msg->signal);
    if (res < 0) {
        VBLE_ERROR(res);
    }
}

static inline void _send_signal(struct vble_msg *msg)
{
    int res = sem_post(msg->signal);
    if (res < 0) {
        VBLE_ERROR(res);
    }
}

static inline int _recv_message(int fd, char *buf, int buflen)
{
    int received = 0;
    while (1) {
        int res = read(fd, (void *)(buf + received), buflen - received);
        if (res < 0) {
            VBLE_ERROR(res);
            return -1;
        }
        received += res;
        if (received == buflen) {
            break;
        }
    }
    return 0;
}

static inline int _send_message(int fd, char *buf, int buflen)
{
    int sent = 0;
    while (1) {
        int res = write(fd, (void *)(buf + sent), buflen - sent);
        if (res < 0) {
            VBLE_ERROR(res);
            return -1;
        }
        sent += res;
        if (sent == buflen) {
            break;
        }
    }
    return 0;
}

static inline int _progress_message(struct vble_req *req)
{
    int fd = open(VBLE_MSG_QUEUE_NAME, O_WRONLY);
    if (fd < 0) {
        VBLE_ERROR(0);
        return -1;
    }

    struct vble_msg *msg = NULL;
    int res = _create_message(&msg, req);
    if (res < 0) {
        VBLE_ERROR(res);
        close(fd);
        return -1;
    }

    res = _send_message(fd, (char *)msg, sizeof(struct vble_msg));
    close(fd);
    if (res < 0) {
        VBLE_ERROR(res);
        _destroy_message(msg);
        return -1;
    }

    _wait_message(msg);
    _destroy_message(msg);

    return 0;
}

int _vble_create_msgqueue(int *fd)
{
    int res = mkfifo(VBLE_MSG_QUEUE_NAME, 0666);
    if (res < 0 && res != -EEXIST) {
        VBLE_ERROR(0);
        return -1;
    }

    *fd = open(VBLE_MSG_QUEUE_NAME, O_RDWR);
    if (*fd < 0) {
        VBLE_ERROR(0);
        unlink(VBLE_MSG_QUEUE_NAME);
        return -1;
    }

    return 0;
}


/*
 * Handler
 */
static void vdev_run(int argc, char *argv[])
{
    int fd;
    int res = _vble_create_msgqueue(&fd);
    if (res < 0) {
        VBLE_ERROR(res);
        return;
    }

    fd_set rfds, tfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    while (1) {
        tfds = rfds;
        res = select(fd + 1, &tfds, NULL, NULL, NULL);
        if (res <= 0) {
            if (errno == EINTR) {
                continue;
            }
            VBLE_ERROR(res);
            break;
        }
        struct vble_msg msg;
        res = _recv_message(fd, (char *)&msg, sizeof(struct vble_msg));
        if (res < 0) {
            VBLE_ERROR(res);
            break;
        }

        res = vble_handle_message(msg.req);
        if (res < 0) {
            VBLE_ERROR(res);
            break;
        }
        _send_signal(&msg);
    }
    return;
}

void vble_register(void){
    g_vble_dev = bledev_register(&g_trble_drv_ops);
    if (!g_vble_dev) {
        VBLE_ERROR(0);
    }
    return;
}

void vble_start(void){
    int new_thread = kernel_thread("ble mocking driver", 100, 2048,
                                   (main_t)vdev_run, (char *const *)NULL);
    if (new_thread < 0) {
        VBLE_ERROR(new_thread);
    }
    return;
}

/*
 * Interface API
 */

trble_result_e vdev_init(struct bledev *dev, trble_client_init_config *init_client, trble_server_init_config *init_server)
{
    if (!rltk_wlan_running(0)) {
        printf("[VBLE] WiFi is off. Please turn on WiFi first.\n");
        return TRBLE_FAIL;
    }

    if (init_client == NULL || init_server == NULL || init_server->profile == NULL \
                            || init_server->profile_count == 0)
    {
        return TRBLE_FAIL;
    } else if (is_server_init != false) {
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vconfigs config = {init_client, init_server};
    struct vble_req req = {VBLE_MSG_INIT, (void *)&config, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_deinit(struct bledev *dev)
{
    if (is_server_init == false) {
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_DEINIT, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_get_mac_addr(struct bledev *dev, uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{

    uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
    memcpy(mac, dummy, TRBLE_BD_ADDR_MAX_LEN);
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_GET_MAC_ADDR, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_get_bonded_device(struct bledev *dev, trble_bonded_device_list_s *device_list, uint16_t *device_count)
{
    if ( device_count == NULL || device_list == NULL)
    {
        return TRBLE_INVALID_ARGS;
    }
    int count = 0;
    for (int i = 0; i < TRBLE_MAX_CONNECTION_COUNT; i++)
    {
        if (connected_device[i].is_bonded) {
            device_list[*device_count].bd_addr = connected_device[i].conn_info.addr;
            count++;
        }
        *device_count = count;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_GET_BONDED_DEVICE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_delete_bond(struct bledev *dev, trble_addr *addr)
{
    if (addr == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_DELETE_BONDED, (void *)addr, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_delete_bond_all(struct bledev *dev)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_DELETE_BONDED_ALL, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_conn_is_active(struct bledev *dev, trble_conn_handle conn_handle, bool *is_active)
{
    if (is_active == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    if (vble_connected_list.conn_handle[conn_handle] == VBLE_NO_CONNECTION) {
        *is_active = false;
    } else {
        *is_active = true;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CONN_IS_ACTIVE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_conn_is_any_active(struct bledev *dev, bool *is_active)
{
    if (is_active == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CONN_IS_ANY_ACTIVE, (void *)is_active, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_ioctl(struct bledev *dev, trble_msg_s *msg)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_IOCTL, (void *)msg, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

/*** Scanner(Observer) ***/
trble_result_e vdev_start_scan(struct bledev *dev, trble_scan_filter *filter)
{
    if (is_client_init != true) {
        return TRBLE_FAIL;
    }
    if (is_scan_started != false) {
        VBLE_LOG("Scan already running\n");
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_START_SCAN, (void *)filter, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_stop_scan(struct bledev *dev)
{
    if (is_scan_started == false) {
        VBLE_LOG("Scan not running");
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_STOP_SCAN, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_scan_whitelist_add(struct bledev *dev, trble_addr *addr)
{
    if (addr == NULL) {
        return TRBLE_INVALID_ARGS;
    } else if (is_scan_started != false) {
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SCAN_WHITELIST_ADD, (void *)addr, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_scan_whitelist_delete(struct bledev *dev, trble_addr *addr)
{
    if (addr == NULL) {
        return TRBLE_INVALID_ARGS;
    } else if (is_scan_started != false) {
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SCAN_WHITELIST_DELETE, (void *)addr, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_scan_whitelist_clear_all(struct bledev *dev)
{
    if (is_scan_started != false) {
        return TRBLE_INVALID_STATE;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SCAN_WHITELIST_CLEAR_ALL, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

/*** Central(Client) ***/
trble_result_e vdev_client_connect(struct bledev *dev, trble_conn_info *conn_info)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CLIENT_CONNECT, (void *)conn_info, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_client_disconnect(struct bledev *dev, trble_conn_handle conn_handle)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CLIENT_DISCONNECT, (void *)&conn_handle, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_client_disconnect_all(struct bledev *dev)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CLIENT_DISCONNECT_ALL, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_connected_device_list(struct bledev *dev, trble_connected_list *out_connected_list)
{
    if (out_connected_list == NULL) {
        return BLE_MANAGER_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CONNECTED_DEVICE_LIST, (void *)out_connected_list, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_connected_info(struct bledev *dev, trble_conn_handle conn_handle, trble_device_connected *out_connected_device)
{
    if (out_connected_device == NULL) {
        return BLE_MANAGER_INVALID_ARGS;
    }
    if (conn_handle >= vble_connected_list.connected_count) {
        return TRBLE_FAIL;
    }
    memset(out_connected_device, 0, sizeof(out_connected_device));
    out_connected_device->conn_info = connected_device[conn_handle].conn_info;
    out_connected_device->is_bonded = connected_device[conn_handle].is_bonded;
    out_connected_device->conn_handle = connected_device[conn_handle].conn_handle;
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CONNECTED_INFO, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_operation_enable_notification(struct bledev *dev, trble_operation_handle *handle)
{
    if (handle == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_OPERATION_ENABLE_NOTIFICATION, (void *)handle, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_operation_read(struct bledev *dev, trble_operation_handle *handle, trble_data *out_data)
{
    if (handle == NULL || out_data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_OPERATION_READ, (void *)out_data, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_operation_write(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
    if (handle == NULL || in_data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_OPERATION_WRITE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_operation_write_no_response(struct bledev *dev, trble_operation_handle *handle, trble_data *in_data)
{
    if (handle == NULL || in_data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_OPERATION_WRITE_NO_RESPONSE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}


/*** Peripheral(Server) ***/
trble_result_e vdev_get_profile_count(struct bledev *dev, uint16_t *count)
{
    if (count == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_GET_PROFILE_COUNT, (void *)count, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_charact_notify(struct bledev *dev, trble_attr_handle attr_handle, trble_conn_handle con_handle, trble_data *data)
{
    if (data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_CHARACTER_NOTIFY, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_attr_set_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
    if (data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    vattr attr_data;
    attr_data.attr_handle = attr_handle;
    attr_data.data = data;
    struct vble_req req = {VBLE_MSG_ATTR_SET_DATA, (void *)&attr_data, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_attr_get_data(struct bledev *dev, trble_attr_handle attr_handle, trble_data *data)
{
    if (data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    
    trble_result_e tres = TRBLE_SUCCESS;
    vattr attr_data;
    attr_data.attr_handle = attr_handle;
    attr_data.data = data;
    struct vble_req req = {VBLE_MSG_ATTR_GET_DATA, (void *)&attr_data, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_attr_reject(struct bledev *dev, trble_attr_handle attr_handle, uint8_t app_errorcode)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_ATTR_REJECT, (void *)&attr_handle, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_server_disconnect(struct bledev *dev, trble_conn_handle con_handle)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SERVER_DISCONNECT, (void *)&con_handle, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_get_mac_addr_by_conn_handle(struct bledev *dev, trble_conn_handle con_handle, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN])
{
    uint8_t dummy[] = {0x00, 0x1E, 0xC0, 0x04, 0x9F, 0xF3};
    memcpy(bd_addr, dummy, TRBLE_BD_ADDR_MAX_LEN);
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_GET_MAC_ADDR_BY_CONN_HANDLE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_get_conn_handle_by_addr(struct bledev *dev, uint8_t bd_addr[TRBLE_BD_ADDR_MAX_LEN], trble_conn_handle *con_handle)
{
    if (con_handle == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    *con_handle = 0;
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_GET_CONN_HANDLE_BY_ADDR, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

/*** Advertiser(Broadcaster) ***/
trble_result_e vdev_set_adv_data(struct bledev *dev, trble_data *data)
{
    if (data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SET_ADV_DATA, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_set_adv_resp(struct bledev *dev, trble_data *data)
{
    if (data == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SET_ADV_RESP, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_set_adv_type(struct bledev *dev, trble_adv_type_e adv_type, trble_addr *addr)
{
    if (addr == NULL) {
        return TRBLE_INVALID_ARGS;
    }
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SET_ADV_TYPE, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_set_adv_interval(struct bledev *dev, uint16_t interval)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_SET_ADV_INTERVAL, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_start_adv(struct bledev *dev)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_START_ADV, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}

trble_result_e vdev_stop_adv(struct bledev *dev)
{
    trble_result_e tres = TRBLE_SUCCESS;
    struct vble_req req = {VBLE_MSG_STOP_ADV, NULL, &tres};
    int res = _progress_message(&req);
    if (res < 0) {
        return TRBLE_FAIL;
    }
    return tres;
}