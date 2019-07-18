/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tinyara/config.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/bluetooth_internal.h>
#include <bluetooth/bluetooth_extension.h>
#include "bluetooth_private.h"

int bt_audio_initialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_audio_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_audio_connect(const char *remote_address, bt_audio_profile_type_e type)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_audio_disconnect(const char *remote_address, bt_audio_profile_type_e type)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_audio_select_role(bt_audio_role_e role)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_audio_set_connection_state_changed_cb(bt_audio_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
int bt_audio_unset_connection_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_ag_notify_speaker_gain(int gain)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_get_speaker_gain(int *gain)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_is_nrec_enabled(bool *enabled)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_is_wbs_mode(bool *wbs_mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_microphone_gain_changed_cb(bt_ag_microphone_gain_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_microphone_gain_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_speaker_gain_changed_cb(bt_ag_speaker_gain_changed_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_speaker_gain_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_open_sco(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_close_sco(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_is_sco_opened(bool *opened)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_sco_state_changed_cb(bt_ag_sco_state_changed_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_sco_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_notify_call_event(bt_ag_call_event_e event, unsigned int call_id, const char *phone_number)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_notify_call_list(bt_call_list_h list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_notify_vendor_cmd(const char *command)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_notify_voice_recognition_state(bool state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_call_handling_event_cb(bt_ag_call_handling_event_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_call_handling_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_multi_call_handling_event_cb(
					bt_ag_multi_call_handling_event_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_multi_call_handling_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_vendor_cmd_cb(bt_ag_vendor_cmd_cb callback,
						void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_vendor_cmd_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_set_dtmf_transmitted_cb(bt_ag_dtmf_transmitted_cb callback,
						void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_unset_dtmf_transmitted_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_is_connected(bool *connected)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_ag_switch_headset(const char *remote_addr)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_call_list_create(bt_call_list_h *list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_call_list_destroy(bt_call_list_h list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_call_list_reset(bt_call_list_h list)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_call_list_add(bt_call_list_h list, unsigned int call_id, bt_ag_call_state_e state, const char *phone_number)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_a2dp_set_content_protection(bool status)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_initialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_notify_call_event(bt_hf_call_event_e event, char *phone_number)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_notify_speaker_gain(int gain)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_set_speaker_gain_changed_cb(bt_hf_speaker_gain_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_unset_speaker_gain_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_notify_voice_recognition_state(bool state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_notify_vendor_cmd(int appid, char *msg)
{
	return BT_ERROR_NOT_SUPPORTED;
}

#ifdef GLIB_SUPPORTED
int bt_hf_set_call_status_updated_event_cb(bt_hf_call_status_updated_event_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}
#endif

int bt_hf_unset_call_status_updated_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_close_sco(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_send_dtmf(char *dtmf)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_is_connected(bool *connected)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_is_sco_opened(bool *opened)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_get_codec_id(unsigned int *codec_id)
{
	return BT_ERROR_NOT_SUPPORTED;
}

#ifdef GLIB_SUPPORTED
int bt_hf_get_call_status_info_list(GSList **call_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}
#endif

int bt_hf_request_call_status_info_list(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_is_inband_ringtone_supported(bool *supported)
{
	return BT_ERROR_NOT_SUPPORTED;
}

#ifdef GLIB_SUPPORTED
int bt_hf_free_call_status_info_list(GSList *call_list)
{
	return BT_ERROR_NOT_SUPPORTED;
}
#endif

int bt_hf_set_sco_state_changed_cb(bt_hf_sco_state_changed_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_unset_sco_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_set_call_handling_event_cb(bt_hf_call_handling_event_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_unset_call_handling_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_set_multi_call_handling_event_cb(
					bt_hf_multi_call_handling_event_cb callback,
					void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_unset_multi_call_handling_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_set_remote_call_event_cb(
				bt_hf_remote_call_event_cb callback,
				void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_hf_unset_remote_call_event_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_STOP */
