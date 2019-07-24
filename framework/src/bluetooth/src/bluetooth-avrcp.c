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
#include "bluetooth_private.h"

int bt_avrcp_target_initialize(bt_avrcp_target_connection_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_avrcp_target_connect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_avrcp_target_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_equalizer_state(bt_avrcp_equalizer_state_e state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_repeat_mode(bt_avrcp_repeat_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_shuffle_mode(bt_avrcp_shuffle_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_scan_mode(bt_avrcp_scan_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_player_state(bt_avrcp_player_state_e state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_position(unsigned int position)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_target_notify_track(const char *title, const char *artist, const char *album,
		const char *genre, unsigned int track_num, unsigned int total_tracks, unsigned int duration)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_avrcp_control_initialize(bt_avrcp_control_connection_state_changed_cb callback,
								void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_deinitialize(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_connect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_disconnect(const char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_send_player_command(bt_avrcp_player_command_e cmd)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_send_player_command_to(bt_avrcp_player_command_e cmd, char *remote_address)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_set_absolute_volume(unsigned int value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_increase_volume(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_decrease_volume(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_send_delay_report(unsigned int value)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_set_equalizer_state(bt_avrcp_equalizer_state_e state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_equalizer_state(bt_avrcp_equalizer_state_e *state)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_set_repeat_mode(bt_avrcp_repeat_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_repeat_mode(bt_avrcp_repeat_mode_e *mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_set_shuffle_mode(bt_avrcp_shuffle_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_shuffle_mode(bt_avrcp_shuffle_mode_e *mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}
int bt_avrcp_control_set_scan_mode(bt_avrcp_scan_mode_e mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_scan_mode(bt_avrcp_scan_mode_e *mode)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_position(unsigned int *position)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_play_status(bt_avrcp_player_state_e *status)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_get_track_info(bt_avrcp_metadata_attributes_info_s **track)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_control_free_track_info(bt_avrcp_metadata_attributes_info_s *track)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */

int bt_avrcp_set_equalizer_state_changed_cb(bt_avrcp_equalizer_state_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_equalizer_state_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_repeat_mode_changed_cb(bt_avrcp_repeat_mode_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_repeat_mode_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_shuffle_mode_changed_cb(bt_avrcp_shuffle_mode_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_shuffle_mode_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_scan_mode_changed_cb(bt_avrcp_scan_mode_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_scan_mode_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

/* LCOV_EXCL_START */
int bt_avrcp_set_position_changed_cb(bt_avrcp_position_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_position_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_delay_changed_cb(bt_avrcp_delay_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_delay_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_play_status_changed_cb(bt_avrcp_play_status_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_play_status_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_set_track_info_changed_cb(bt_avrcp_track_info_changed_cb callback, void *user_data)
{
	return BT_ERROR_NOT_SUPPORTED;
}

int bt_avrcp_unset_track_info_changed_cb(void)
{
	return BT_ERROR_NOT_SUPPORTED;
}
/* LCOV_EXCL_STOP */
