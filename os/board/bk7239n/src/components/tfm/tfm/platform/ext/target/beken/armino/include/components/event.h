// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BK EVENT API
 * @addtogroup bk_api_event BK EVENT API
 * @{
 */

/**
 * @brief BK EVENT typedefs
 * @defgroup bk_api_event_typedef
 * @ingroup bk_api_event
 * @{
 */

typedef enum {
	EVENT_MOD_WIFI_INTERNAL,   /**< WiFi internal event */
	EVENT_MOD_WIFI,            /**< WiFi public event */
	EVENT_MOD_NETIF,           /**< Netif event */
	EVENT_MOD_COUNT,           /**< Event module count */
} event_module_t;

#define BK_ERR_EVENT_MOD            (BK_ERR_EVENT_BASE - 1) /**< Event module is not supported */
#define BK_ERR_EVENT_ID             (BK_ERR_EVENT_BASE - 2) /**< Event ID is not supported */
#define BK_ERR_EVENT_MOD_OR_ID      (BK_ERR_EVENT_BASE - 3) /**< Event ID or Module is invalid */
#define BK_ERR_EVENT_CB_EXIST       (BK_ERR_EVENT_BASE - 4) /**< Event cb already registered */
#define BK_ERR_EVENT_NO_CB          (BK_ERR_EVENT_BASE - 5) /**< No event cb is registered */
#define BK_ERR_EVENT_CREATE_QUEUE   (BK_ERR_EVENT_BASE - 6) /**< Failed to create event message queue */
#define BK_ERR_EVENT_CREATE_MUTEX   (BK_ERR_EVENT_BASE - 7) /**< Failed to create event mutex */
#define BK_ERR_EVENT_CREATE_TASK    (BK_ERR_EVENT_BASE - 8) /**< Failed to create event task */
#define BK_ERR_EVENT_NULL_MSG       (BK_ERR_EVENT_BASE - 9) /**< Event message is NULL */
#define BK_ERR_EVENT_INIT_SEM       (BK_ERR_EVENT_BASE - 10) /**< Failed to init RTOS semaphore */
#define BK_ERR_EVENT_NOT_INIT       (BK_ERR_EVENT_BASE - 11) /**< Event module not init */
#define BK_ERR_EVENT_POST_QUEUE     (BK_ERR_EVENT_BASE - 12) /**< Failed to post event request to event queue */
#define BK_ERR_EVENT_UNKNOWN_MSG    (BK_ERR_EVENT_BASE - 13) /**< Not supported event message */

#define EVENT_ID_ALL            (-1) /**< All events in a event module */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @brief BK EVENT API
 * @addtogroup bk_api_event BK EVENT API
 * @{
 */

/**
 * @brief Event callback
 *
 * The event callback get called when related event dispatched.
 */
typedef bk_err_t (*event_cb_t)(void *arg, event_module_t event_module,
		int event_id, void *event_data);

/**
 * @brief Register event callback
 *
 * @param event_module_id the module ID of the event to register the callback for
 * @param event_id the id of the event to register the callback for
 * @param event_cb the callback which gets called when the event is dispatched
 * @param event_cb_arg data, aside from event data, that is passed to the handler when it is called
 *
 * @return
 *  - BK_OK: succeed
 *  - BK_ERR_EVENT_NOT_INIT: the event module is not initialized
 *  - BK_ERR_EVENT_MOD_OR_ID: the event module ID or event ID is invalid
 *  - BK_ERR_EVENT_NO_MEM: out of memory
 *  - BK_ERR_EVENT_INIT_SEM: failed to init the internal event semaphore
 *  - BK_ERR_EVENT_POST_QUEUE: failed to post the event to event queue
 *  - BK_ERR_EVENT_CB_EXIST: the event callback already unregistered
 *  - Others: Fail
 */
bk_err_t bk_event_register_cb(event_module_t event_module_id, int event_id,
		event_cb_t event_cb, void *event_cb_arg);

/**
 * @brief Unregister a callback
 *
 * This function can be used to unregister a callback so that it no longer gets called
 * during dispatch.
 *
 * @param event_module_id the ID of the module with which to unregister the callback
 * @param event_id the ID of the event with which to unregister the callback
 * @param event_cb the callback to be unregistered
 *
 * @return
 *  - BK_OK: Success
 *  - BK_ERR_EVENT_NOT_INIT: the event module is not initialized
 *  - BK_ERR_EVENT_MOD_OR_ID: the event module ID or event ID is invalid
 *  - BK_ERR_EVENT_NO_MEM: out of memory
 *  - BK_ERR_EVENT_INIT_SEM: failed to init the internal event semaphore
 *  - BK_ERR_EVENT_POST_QUEUE: failed to post the event to event queue
 *  - BK_ERR_EVENT_NO_CB: the event callback already unregistered
 *  - Others: Fail
 */
bk_err_t bk_event_unregister_cb(event_module_t event_module_id, int event_id,
		event_cb_t event_cb);

/**
 * @brief Posts an event to the event task.
 *
 * @param event_module_id The ID of the module that generates the event
 * @param event_id the event id that identifies the event
 * @param event_data the data, specific to the event occurence, that gets passed to the callback
 * @param event_data_size the size of the event data
 * @param timeout number of milliseconds to block on a full event queue
 *
 * @return
 *  - BK_OK: Success
 *  - BK_ERR_EVENT_NOT_INIT: the event module is not initialized
 *  - BK_ERR_EVENT_MOD_OR_ID: the event module ID or event ID is not invalid
 *  - BK_ERR_EVENT_NO_MEM: out of memory
 *  - BK_ERR_EVENT_INIT_SEM: failed to init the internal event semaphore
 *  - BK_ERR_EVENT_POST_QUEUE: failed to post the event to event queue
 *  - Others: Fail
 */
bk_err_t bk_event_post(event_module_t event_module_id, int event_id,
		void* event_data, size_t event_data_size, uint32_t timeout);

/**
 * @brief Init the event module
 *
 * This API initializes the event queue and create the event task.
 *
 * This API should be called before any event API is called, generally it should
 * be called before WiFi/BLE is initialized.
 *
 * @return
 *  - BK_OK: succeed
 *  - BK_ERR_EVENT_CREATE_QUEUE: failed to create event queue
 *  - BK_ERR_EVENT_CREATE_TASK: failed to create event task
 *  - Others: Fail
 */
bk_err_t bk_event_init(void);

/**
 * @brief Deinit the event module
 *
 * This API destroy the event queue and delete the event task.
 *
 * @return
 *  - BK_OK: succeed
 *  - BK_ERR_NO_MEM: out of memory
 *  - Others: Fail
 */
bk_err_t bk_event_deinit(void);

/**
 * @brief Dump all registered event callback
 *
 * @attention This API is for debug only
 *
 * @return
 *  - BK_OK: succeed
 *  - BK_ERR_NO_MEM: out of memory
 *  - Others: Fail
 */
bk_err_t bk_event_dump(void);

/**
 * @}
 */

typedef bk_err_t (*event_callback_t)(void *arg, event_module_t event_module,
		int event_id, void *event_data);

#ifdef __cplusplus
}
#endif
