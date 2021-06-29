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

/* state, handler, str*/
WIFIMGR_STATE_TABLE(WIFIMGR_UNINITIALIZED, _handler_on_uninitialized_state, "UNINITIALIZED")
WIFIMGR_STATE_TABLE(WIFIMGR_STA_DISCONNECTED, _handler_on_disconnected_state, "STA_DISCONNECTED")
WIFIMGR_STATE_TABLE(WIFIMGR_STA_DISCONNECTING, _handler_on_disconnecting_state, "STA_DISCONNECTING")
WIFIMGR_STATE_TABLE(WIFIMGR_STA_CONNECTING, _handler_on_connecting_state, "STA_CONNECTING")
WIFIMGR_STATE_TABLE(WIFIMGR_STA_CONNECTED, _handler_on_connected_state, "STA_CONNECTED")
WIFIMGR_STATE_TABLE(WIFIMGR_STA_RECONNECT, _handler_on_reconnect_state, "STA_RECONNECT")
WIFIMGR_STATE_TABLE(WIFIMGR_SOFTAP, _handler_on_softap_state, "SOFTAP")
WIFIMGR_STATE_TABLE(WIFIMGR_SCANNING, _handler_on_scanning_state, "SCANNING")
