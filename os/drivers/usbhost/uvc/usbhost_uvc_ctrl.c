/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include "usbhost_uvc.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct uvc_menu_info ctrl_exposure_auto[] = {
	{2, "Auto Mode"},
	{1, "Manual Mode"},
	{4, "Shutter Priority Mode"},
	{8, "Aperture Priority Mode"},
};

static const struct uvc_control_info uvc_ctrls[] = {
	/* UVC supported processing unit controls */
	{UVC_GUID_UVC_PROCESSING, 0, UVC_PU_BRIGHTNESS_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 1, UVC_PU_CONTRAST_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 2, UVC_PU_HUE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_PROCESSING, 3, UVC_PU_SATURATION_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 4, UVC_PU_SHARPNESS_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 5, UVC_PU_GAMMA_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 6, UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_PROCESSING, 7, UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL, 4, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_PROCESSING, 8, UVC_PU_BACKLIGHT_COMPENSATION_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 9, UVC_PU_GAIN_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 10, UVC_PU_POWER_LINE_FREQUENCY_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 11, UVC_PU_HUE_AUTO_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 12, UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 13, UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 14, UVC_PU_DIGITAL_MULTIPLIER_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 15, UVC_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE,},
	{UVC_GUID_UVC_PROCESSING, 16, UVC_PU_ANALOG_VIDEO_STANDARD_CONTROL, 1, F_GET_CUR,},
	{UVC_GUID_UVC_PROCESSING, 17, UVC_PU_ANALOG_LOCK_STATUS_CONTROL, 1, F_GET_CUR,},
	{UVC_GUID_UVC_PROCESSING, 18, UVC_PU_CONTRAST_AUTO_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF,},
	/* UVC supported Camera unit controls */
	{UVC_GUID_UVC_CAMERA, 0, UVC_CT_SCANNING_MODE_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_RESTORE,},
	{UVC_GUID_UVC_CAMERA, 1, UVC_CT_AE_MODE_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_GET_RES | F_RESTORE,},
	{UVC_GUID_UVC_CAMERA, 2, UVC_CT_AE_PRIORITY_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_RESTORE,},
	{UVC_GUID_UVC_CAMERA, 3, UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL, 4, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 4, UVC_CT_EXPOSURE_TIME_RELATIVE_CONTROL, 1, F_SET_CUR | F_RESTORE,},
	{UVC_GUID_UVC_CAMERA, 5, UVC_CT_FOCUS_ABSOLUTE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 6, UVC_CT_FOCUS_RELATIVE_CONTROL, 2, F_SET_CUR | F_GET_MIN | F_GET_MAX | F_GET_RES | F_GET_DEF | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 7, UVC_CT_IRIS_ABSOLUTE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 8, UVC_CT_IRIS_RELATIVE_CONTROL, 1, F_SET_CUR | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 9, UVC_CT_ZOOM_ABSOLUTE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 10, UVC_CT_ZOOM_RELATIVE_CONTROL, 3, F_SET_CUR | F_GET_MIN | F_GET_MAX | F_GET_RES | F_GET_DEF | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 11, UVC_CT_PANTILT_ABSOLUTE_CONTROL, 8, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 12, UVC_CT_PANTILT_RELATIVE_CONTROL, 4, F_SET_CUR | F_GET_RANGE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 13, UVC_CT_ROLL_ABSOLUTE_CONTROL, 2, F_SET_CUR | F_GET_RANGE | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 14, UVC_CT_ROLL_RELATIVE_CONTROL, 2, F_SET_CUR | F_GET_MIN | F_GET_MAX | F_GET_RES | F_GET_DEF | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 17, UVC_CT_FOCUS_AUTO_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF | F_RESTORE,},
	{UVC_GUID_UVC_CAMERA, 18, UVC_CT_PRIVACY_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_RESTORE | F_AUTO,},
	{UVC_GUID_UVC_CAMERA, 19, UVC_CT_FOCUS_SIMPLE_CONTROL, 1, F_SET_CUR | F_GET_CUR | F_GET_DEF,},
	{UVC_GUID_UVC_CAMERA, 20, UVC_CT_WINDOW_CONTROL, 2, F_SET_CUR | F_GET_CUR | F_GET_MIN | F_GET_MAX | F_GET_DEF,},
	{UVC_GUID_UVC_CAMERA, 21, UVC_CT_REGION_OF_INTEREST_CONTROL, 2, F_SET_CUR | F_GET_CUR | F_GET_MIN | F_GET_MAX | F_GET_DEF,},
};

static const struct uvc_control_mapping uvc_ctrl_mappings[] = {
	/* V4L2 User class UVC mappings */
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BRIGHTNESS, "Brightness", UVC_GUID_UVC_PROCESSING, UVC_PU_BRIGHTNESS_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_CONTRAST, "Contrast", UVC_GUID_UVC_PROCESSING, UVC_PU_CONTRAST_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HUE, "Hue", UVC_GUID_UVC_PROCESSING, UVC_PU_HUE_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SATURATION, "Saturation", UVC_GUID_UVC_PROCESSING, UVC_PU_SATURATION_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SHARPNESS, "Sharpness", UVC_GUID_UVC_PROCESSING, UVC_PU_SHARPNESS_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_GAMMA, "Gamma", UVC_GUID_UVC_PROCESSING, UVC_PU_GAMMA_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_AUTO_WHITE_BALANCE, "White Balance Component, Auto", UVC_GUID_UVC_PROCESSING, UVC_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL, 1, 0, V4L2_CTRL_TYPE_BOOLEAN, UVC_CTRL_DATA_TYPE_BOOLEAN,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BLUE_BALANCE, "WB Blue Component", UVC_GUID_UVC_PROCESSING, UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_RED_BALANCE, "WB Red Component", UVC_GUID_UVC_PROCESSING, UVC_PU_WHITE_BALANCE_COMPONENT_CONTROL, 16, 16, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	/* V4L2 Camera class UVC mappings */
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_ABSOLUTE, "Focus (absolute)", UVC_GUID_UVC_CAMERA, UVC_CT_FOCUS_ABSOLUTE_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_AUTO, "Focus, Auto", UVC_GUID_UVC_CAMERA, UVC_CT_FOCUS_AUTO_CONTROL, 1, 0, V4L2_CTRL_TYPE_BOOLEAN, UVC_CTRL_DATA_TYPE_BOOLEAN,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_ABSOLUTE, "Iris, Absolute", UVC_GUID_UVC_CAMERA, UVC_CT_IRIS_ABSOLUTE_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_RELATIVE, "Iris, Relative", UVC_GUID_UVC_CAMERA, UVC_CT_IRIS_RELATIVE_CONTROL, 8, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_ABSOLUTE, "Zoom, Absolute", UVC_GUID_UVC_CAMERA, UVC_CT_ZOOM_ABSOLUTE_CONTROL, 16, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_CONTINUOUS, "Zoom, Continuous", UVC_GUID_UVC_CAMERA, UVC_CT_ZOOM_RELATIVE_CONTROL, 0, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_SIGNED,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_AUTO, "Exposure, Auto", UVC_GUID_UVC_CAMERA, UVC_CT_AE_MODE_CONTROL, 4, 0, V4L2_CTRL_TYPE_MENU, UVC_CTRL_DATA_TYPE_BITMASK,},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_ABSOLUTE, "Exposure (Absolute)", UVC_GUID_UVC_CAMERA, UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL, 32, 0, V4L2_CTRL_TYPE_INTEGER, UVC_CTRL_DATA_TYPE_UNSIGNED,},
};

static const uint8_t pu_guid[16] = UVC_GUID_UVC_PROCESSING;
static const uint8_t camera_guid[16] = UVC_GUID_UVC_CAMERA;
static const uint8_t media_transport_input_guid[16] = UVC_GUID_UVC_MEDIA_TRANSPORT_INPUT;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: uvc_init_mapping
 *
 * Description:
 *   This function is use to initialize the mapping table for supported controls
 *   Mostly used to initialize those controls which does have additional items
 *   where in there is no direct mapping from V4L2 to UVC (master/slave id),
 *   menu items etc.
 *
 * Input Parameters:
 *   map  - pointer to mapping object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void uvc_init_mapping(struct uvc_control_mapping *map)
{
	/* Initialiation for V4L2 Camera class controls */
	if (map->class == V4L2_CTRL_CLASS_CAMERA) {
		switch (map->id) {
		case V4L2_CID_FOCUS_AUTO:
			map->slave_ids[0] = V4L2_CID_FOCUS_ABSOLUTE;
			break;
		case V4L2_CID_FOCUS_ABSOLUTE:
			map->master_id = V4L2_CID_FOCUS_AUTO;
			map->master_class = V4L2_CTRL_CLASS_CAMERA;
			map->master_manual = 0;
			break;
		case V4L2_CID_EXPOSURE_ABSOLUTE:
			map->master_id = V4L2_CID_EXPOSURE_AUTO;
			map->master_class = V4L2_CTRL_CLASS_CAMERA;
			map->master_manual = V4L2_EXPOSURE_MANUAL;
			break;
		case V4L2_CID_EXPOSURE_AUTO:
			map->menu_info = ctrl_exposure_auto;
			map->menu_count = ARRAY_SIZE(ctrl_exposure_auto);
			map->slave_ids[0] = V4L2_CID_EXPOSURE_ABSOLUTE;
			break;
		default:
			break;
		}
		/* Initialiation for V4L2 User class controls */
	} else {
		switch (map->id) {
		case V4L2_CID_RED_BALANCE:
			map->master_id = V4L2_CID_AUTO_WHITE_BALANCE;
			map->master_class = V4L2_CTRL_CLASS_USER;
			map->master_manual = 0;
			break;
		case V4L2_CID_BLUE_BALANCE:
			map->master_id = V4L2_CID_AUTO_WHITE_BALANCE;
			map->master_class = V4L2_CTRL_CLASS_USER;
			map->master_manual = 0;
			break;
		case V4L2_CID_AUTO_WHITE_BALANCE:
			map->slave_ids[0] = V4L2_CID_BLUE_BALANCE;
			map->slave_ids[1] = V4L2_CID_RED_BALANCE;
			break;
		/* V4L2_CID_HUE_AUTO: supported be added in future */
		default:
			break;
		}
	}
}

/****************************************************************************
 * Name: uvc_test_bit
 *
 * Description:
 *   This function is used to check particular bit is set or not, the control
 *   bits in UVC PU, EU, Camera units to identifty the supported controls
 *   controls
 *
 * Input Parameters:
 *   data  - pointer to data object.
 *   bit   - index to the bit in data.
 *
 * Returned Values:
 *   0 if not set, 1 if the bit is set.
 *
 ****************************************************************************/
static inline int uvc_test_bit(const uint8_t *data, int bit)
{
	return (data[bit >> 3] >> (bit & 7)) & 1;
}

/****************************************************************************
 * Name: uvc_get_value
 *
 * Description:
 *   This function is used to retrive the control information from the data based
 *   on its V4L2 id/class query(CUR, DEF, STEP, MIN, MAX etc)and data type of the
 *   control.
 *
 * Input Parameters:
 *   mapping - pointer to mapping object
 *   query - type of query(CUR, DEF, STEP, MIN, MAX etc.)
 *   data  - pointer to data object.
 *
 * Returned Values:
 *   Return appropriate value based on V4L2 control id/class query and type.
 *
 ****************************************************************************/
int32_t uvc_get_value(struct uvc_control_mapping *mapping, uint8_t query, const uint8_t *data)
{
	if ((mapping->id == V4L2_CID_ZOOM_CONTINUOUS) && (mapping->class == V4L2_CTRL_CLASS_CAMERA)) {
		int8_t zoom = (int8_t) data[0];
		if (query == UVC_GET_CUR) {
			return (zoom == 0) ? 0 : (zoom > 0 ? data[2] : -data[2]);
		}

		return data[2];
	} else {
		int bits = mapping->size;
		int offset = mapping->offset;
		int32_t value = 0;
		uint8_t mask;

		data += offset / 8;
		offset &= 7;
		mask = ((1LL << bits) - 1) << offset;

		for (; bits > 0; data++) {
			uint8_t byte = *data & mask;
			value |= offset > 0 ? (byte >> offset) : (byte << (-offset));
			bits -= 8 - (offset > 0 ? offset : 0);
			offset -= 8;
			mask = (1 << bits) - 1;
		}

		/* Sign-extend the value if needed. */
		if (mapping->data_type == UVC_CTRL_DATA_TYPE_SIGNED) {
			value |= -(value & (1 << (mapping->size - 1)));
		}

		return value;
	}
}

/****************************************************************************
 * Name: uvc_set_value
 *
 * Description:
 *   This function is used to set the control information to data based on its
 *   V4L2 id/class query(CUR, DEF, STEP, MIN, MAX etc)and data type of the
 *   control.
 *
 * Input Parameters:
 *   mapping - pointer to mapping object
 *   value - value to be set.
 *   data  - pointer to data object.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void uvc_set_value(struct uvc_control_mapping *mapping, int32_t value, uint8_t *data)
{
	if ((mapping->id == V4L2_CID_ZOOM_CONTINUOUS) && (mapping->class == V4L2_CTRL_CLASS_CAMERA)) {
		data[0] = value == 0 ? 0 : (value > 0) ? 1 : 0xff;
		data[2] = min_t((int)abs(value), 0xff);
	} else {
		int bits = mapping->size;
		int offset = mapping->offset;
		uint8_t mask;

		if (mapping->v4l2_type == V4L2_CTRL_TYPE_BUTTON) {
			value = -1;
		}

		data += offset / 8;
		offset &= 7;

		for (; bits > 0; data++) {
			mask = ((1LL << bits) - 1) << offset;
			*data = (*data & ~mask) | ((value << offset) & mask);
			value >>= offset ? offset : 8;
			bits -= 8 - offset;
			offset = 0;
		}
	}
}

/****************************************************************************
 * Name: uvc_read_control
 *
 * Description:
 *   This function is used to read the control data (MIN, MAX, STEP, DEF)
 *   based it support, not all these (MIN, MAX, STEP, DEF) is supported by
 *   UVC controls, based on the flags only this function returns the values.
 *
 * Input Parameters:
 *   ctrl - pointer to control object.
 *   mapping - pointer to mapping object
 *   val - pointer to uvc_ctrl_values.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void uvc_read_control(struct uvc_control *ctrl, struct uvc_control_mapping *mapping, struct uvc_ctrl_value *val)
{
	if (ctrl->info.flags & F_GET_DEF) {
		val->def = uvc_get_value(mapping, UVC_GET_DEF, ctrl->def);
	}

	if (ctrl->info.flags & F_GET_MIN) {
		val->min = uvc_get_value(mapping, UVC_GET_MIN, ctrl->min);
	}

	if (ctrl->info.flags & F_GET_MAX) {
		val->max = uvc_get_value(mapping, UVC_GET_MAX, ctrl->max);
	}

	if (ctrl->info.flags & F_GET_RES) {
		val->step = uvc_get_value(mapping, UVC_GET_RES, ctrl->step);
	}
}

/****************************************************************************
 * Name: uvc_entity_match_guid
 *
 * Description:
 *   This function compare the GUID of the entity and pass GUID, and return the
 *   status.
 *
 * Input Parameters:
 *   entity - pointer to entity object.
 *   guid - guid array.
 *
 * Returned Values:
 *   1 if matches, 0 if GUID is different
 *
 ****************************************************************************/
static bool uvc_entity_match_guid(const struct uvc_entity *entity, const uint8_t guid[16])
{
	switch (UVC_ENTITY_TYPE(entity)) {
	case UVC_ITT_CAMERA:
		return memcmp(camera_guid, guid, 16) == 0;

	case UVC_ITT_MEDIA_TRANSPORT_INPUT:
		return memcmp(media_transport_input_guid, guid, 16) == 0;

	case UVC_VC_PROCESSING_UNIT:
		return memcmp(pu_guid, guid, 16) == 0;

	case UVC_VC_EXTENSION_UNIT:
		return memcmp(entity->extension.guidExtensionCode, guid, 16) == 0;

	default:
		return 0;
	}
}

/****************************************************************************
 * Name: uvc_find_control
 *
 * Description:
 *   This function is used to get the individual controls of a unit
 *
 *   This function is called from the multiple methods to find appropriate
 *   controls for a particular unit.
 *
 * Input Parameters:
 *   entity - reference to unit
 *   v4l2_id - v4l2 control id
 *   v4l2_class - v4l2 class id
 *   mapping  - pointer to hold mapping object.
 *   control  - pointer to hold the control object
 *   next - flag to check the next control in unit.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static void uvc_find_control(struct uvc_entity *entity, uint32_t v4l2_id, uint32_t v4l2_class, struct uvc_control_mapping **mapping, struct uvc_control **control, int next)
{
	struct uvc_control *ctrl = NULL;
	struct uvc_control_mapping *map = NULL;
	unsigned int i;

	if (entity == NULL) {
		udbg("Entity cannot be NULL\n");
		return;
	}

	/* Each entity would holds multiple controls, retrieve each of them and
	 * check if it matches with the parameters passed
	 */
	for (i = 0; i < entity->ncontrols; ++i) {
		ctrl = &entity->controls[i];
		if (!CTRL_IS_SUPPORTED(ctrl->status)) {
			continue;
		}
		/* Parse the mapping list for the control */
		map = queue_entry(ctrl->info.mappings.head, struct uvc_control_mapping, list);
		for (; &map->list != (dq_entry_t *)&ctrl->info.mappings; map = queue_entry(map->list.flink, struct uvc_control_mapping, list)) {
			if ((map->id == v4l2_id) && (map->class == v4l2_class) && !next) {
				*control = ctrl;
				*mapping = map;
				return;
			}

			if ((*mapping == NULL || (*mapping)->id > map->id) && (map->id > v4l2_id) && next) {
				*control = ctrl;
				*mapping = map;
			}
		}
	}
}

/****************************************************************************
 * Name: uvc_search_control
 *
 * Description:
 *   This function is used to search a control item in chain of terminal objects
 *
 *   This function is called from the multiple methods to find appropriate
 *   control object.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   v4l2_id - v4l2 control id
 *   v4l2_class - v4l2 class id
 *   mapping  - pointer to hold mapping object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
struct uvc_control *uvc_search_control(struct uvc_video_chain *chain, uint32_t v4l2_id, uint32_t v4l2_class, struct uvc_control_mapping **mapping)
{
	struct uvc_control *ctrl = NULL;
	struct uvc_entity *entity = queue_entry(chain->entities.head, struct uvc_entity, chain);
	int next = v4l2_id & V4L2_CTRL_FLAG_NEXT_CTRL;

	*mapping = NULL;

	/* Find the control. */
	for (; &entity->chain != &chain->entities; entity = queue_entry(entity->chain.head, struct uvc_entity, chain)) {
		uvc_find_control(entity, v4l2_id, v4l2_class, mapping, &ctrl, next);
		if (ctrl && !next) {
			return ctrl;
		}
	}

	if (ctrl == NULL && !next) {
		uvdbg("Control 0x%08x not found.\n", v4l2_id);
	}

	return ctrl;
}

/****************************************************************************
 * Name: uvc_ctrl_populate_values
 *
 * Description:
 *   This function is used to populate the values read from the device to
 *   control, this avoid further read to device everytime.
 *
 *   This function is called from the query/set/get/menu method of V4L2.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   ctrl  - reference to the control object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_populate_values(struct uvc_video_chain *chain, struct uvc_control *ctrl)
{
	int ret = OK;

	/* If already cached, return immediately */
	if (CTRL_IS_CACHED(ctrl->status)) {
		return ret;
	}

	/* Read the control values from the device base on the flags, not all controls support
	 * MIN, MAX, STEP, DEF. if we send request without checking flags would result in ep0 to
	 * stall.
	 */
	if (ctrl->info.flags & F_GET_DEF) {
		ret = uvc_query_ctrl(chain->dev, UVC_GET_DEF, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->def, ctrl->info.size);
		if (ret < 0) {
			return ret;
		}
	}

	if (ctrl->info.flags & F_GET_MIN) {
		ret = uvc_query_ctrl(chain->dev, UVC_GET_MIN, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->min, ctrl->info.size);
		if (ret < 0) {
			return ret;
		}
	}

	if (ctrl->info.flags & F_GET_MAX) {
		ret = uvc_query_ctrl(chain->dev, UVC_GET_MAX, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->max, ctrl->info.size);
		if (ret < 0) {
			return ret;
		}
	}

	if (ctrl->info.flags & F_GET_RES) {
		ret = uvc_query_ctrl(chain->dev, UVC_GET_RES, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->step, ctrl->info.size);
		if (ret < 0) {
			if (UVC_ENTITY_TYPE(ctrl->entity) != UVC_VC_EXTENSION_UNIT) {
				return ret;
			}
			memset(ctrl->step, 0, ctrl->info.size);
		}
	}
	/* This is requried as uvc_query_ctrl will return >=0 in case of success,
	 * but caller expect OK as success
	 */
	ret = OK;
	/* Once we read it make caching flag set, so that no more reads from the device */
	CTRL_SET_CACHED(ctrl->status);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: uvc_query_v4l2_ctrl
 *
 * Description:
 *   This function is used to get the control value from one of the unit.
 *
 *   This function is called from the uvc_get_range_of_ctrlvalue method of V4L2.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   v4l2_ctrl  - reference to the v4l2 query.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_query_v4l2_ctrl(struct uvc_video_chain *chain, struct v4l2_queryctrl *v4l2_ctrl)
{
	int ret = OK;
	unsigned int i;
	struct uvc_control *ctrl;
	struct uvc_ctrl_value val = { 0, };
	struct uvc_control_mapping *master_map = NULL;
	struct uvc_control *master_ctrl = NULL;
	const struct uvc_menu_info *menu;
	struct uvc_control_mapping *mapping;

	usbhost_uvc_takesem(&chain->ctrl_mutex);

	/* Search for control item with V4L2 id and class */
	ctrl = uvc_search_control(chain, v4l2_ctrl->id, v4l2_ctrl->ctrl_class, &mapping);
	if (ctrl == NULL) {
		uvdbg("Couldn't find any valid control associated with V4L2 id/class!!\n");
		ret = -EINVAL;
		goto done;
	}

	/* reset the values */
	memset(v4l2_ctrl, 0, sizeof(*v4l2_ctrl));
	v4l2_ctrl->ctrl_class = mapping->class;
	v4l2_ctrl->id = mapping->id;
	v4l2_ctrl->type = mapping->v4l2_type;
	strncpy(v4l2_ctrl->name, mapping->name, (sizeof(v4l2_ctrl->name) - 1));
	v4l2_ctrl->name[sizeof(v4l2_ctrl->name) - 1] = 0;
	v4l2_ctrl->flags = 0;

	/* Not control are directly mapped between V4L2 and UVC, if the current mapping has a
	 * master mapping, the query to device should be with master mapping instead
	 */
	if (mapping->master_id && mapping->master_class) {
		uvc_find_control(ctrl->entity, mapping->master_id, mapping->master_class, &master_map, &master_ctrl, 0);
	}

	if (master_ctrl && (master_ctrl->info.flags & F_GET_CUR)) {
		if ((master_ctrl->info.flags & F_GET_CUR) == 0) {
			uvdbg("Invalid access to control!!\n");
			ret = -EACCES;
			goto done;
		}

		if (!CTRL_IS_LOADED(master_ctrl->status)) {
			ret = uvc_query_ctrl(chain->dev, UVC_GET_CUR, master_ctrl->entity->id, chain->dev->intfnum, master_ctrl->info.selector, master_ctrl->cur, master_ctrl->info.size);
			if (ret < 0) {
				udbg("Failed to query the control(%d) to device!!\n", master_ctrl->info.selector);
				goto done;
			}
			ret = OK;
			CTRL_SET_LOADED(master_ctrl->status);
		}
		uvc_get_value(master_map, UVC_GET_CUR, master_ctrl->cur);
	}

	/* Populate the values if not read already */
	ret = uvc_ctrl_populate_values(chain, ctrl);
	if (ret < 0) {
		udbg("Failed to populate control values!!\n");
		goto done;
	}

	uvc_read_control(ctrl, mapping, &val);
	v4l2_ctrl->default_value = val.def;

	switch (mapping->v4l2_type) {
	case V4L2_CTRL_TYPE_MENU:
		v4l2_ctrl->minimum = 0;
		v4l2_ctrl->maximum = mapping->menu_count - 1;
		v4l2_ctrl->step = 1;

		menu = mapping->menu_info;
		for (i = 0; i < mapping->menu_count; ++i, ++menu) {
			if (menu->value == v4l2_ctrl->default_value) {
				v4l2_ctrl->default_value = i;
				break;
			}
		}

		goto done;

	case V4L2_CTRL_TYPE_BOOLEAN:
		v4l2_ctrl->minimum = 0;
		v4l2_ctrl->maximum = 1;
		v4l2_ctrl->step = 1;
		goto done;

	case V4L2_CTRL_TYPE_BUTTON:
		v4l2_ctrl->minimum = 0;
		v4l2_ctrl->maximum = 0;
		v4l2_ctrl->step = 0;
		goto done;

	default:
		break;
	}

	v4l2_ctrl->minimum = val.min;
	v4l2_ctrl->maximum = val.max;
	v4l2_ctrl->step = val.step;

done:
	usbhost_uvc_givesem(&chain->ctrl_mutex);
	return ret;
}

/****************************************************************************
 * Name: uvc_ctrl_commit
 *
 * Description:
 *   This function is used to write back the values which are update from user
 *
 *   This function is called from the uvc_get_ctrlvalue method of V4L2.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   rollback - flag to determine rollback required or not
 *   xctrl - control information to be set on device.
 *   xctrls_count - count of controls to be updated
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_commit(struct uvc_video_chain *chain, int rollback, const struct v4l2_ext_control *xctrls, unsigned int xctrls_count)
{
	unsigned int i;
	struct uvc_control *ctrl;
	struct uvc_entity *entity = queue_entry(chain->entities.head, struct uvc_entity, chain);
	int ret = OK;

	/* Find the control. */
	for (; &entity->chain != &chain->entities; entity = queue_entry(entity->chain.head, struct uvc_entity, chain)) {
		if (entity == NULL) {
			continue;
		}

		for (i = 0; i < entity->ncontrols; ++i) {
			ctrl = &entity->controls[i];
			if (!CTRL_IS_SUPPORTED(ctrl->status)) {
				continue;
			}

			if (ctrl->info.flags & F_AUTO || !(ctrl->info.flags & F_GET_CUR)) {
				CTRL_CLEAR_LOADED(ctrl->status);
			}

			if (!CTRL_IS_DIRTY(ctrl->status)) {
				continue;
			}

			if (!rollback) {
				ret = uvc_query_ctrl(chain->dev, UVC_SET_CUR, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->cur, ctrl->info.size);
			} else {
				ret = OK;
			}

			if (rollback || ret < 0) {
				memcpy(ctrl->cur, ctrl->bak, ctrl->info.size);
			}

			CTRL_CLEAR_DIRTY(ctrl->status);
		}
	}

	usbhost_uvc_givesem(&chain->ctrl_mutex);
	return ret;
}

/****************************************************************************
 * Name: uvc_ctrl_get
 *
 * Description:
 *   This function is used to get the control values from the camera device. If
 *   already read from the device, this function will return the value from
 *   control object instead to reading it again from the device.
 *
 *   This function is called from the uvc_get_ctrlvalue method of V4L2.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   ctrl  - reference to the control object from which data has to be read.
 *   ctrl_class - class the control command to be set.
 *   xctrl - control information to be set on device.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_get(struct uvc_video_chain *chain, struct uvc_control *ctrl, uint16_t ctrl_class, struct v4l2_ext_control *xctrl)
{
	int ret = OK;
	int32_t val = 0;
	unsigned int i;
	struct uvc_control *control;
	struct uvc_control_mapping *mapping;
	const struct uvc_menu_info *menu = NULL;

	if (ctrl == NULL) {
		control = uvc_search_control(chain, xctrl->id, ctrl_class, &mapping);
		if (control == NULL) {
			uvdbg("Failed to find a valid control!!\n");
			return -EINVAL;
		}
	} else {
		control = ctrl;
	}

	if ((control->info.flags & F_GET_CUR) == 0) {
		uvdbg("Invalid access to control, not supported!!\n");
		return -EACCES;
	}

	if (!CTRL_IS_LOADED(control->status)) {
		ret = uvc_query_ctrl(chain->dev, UVC_GET_CUR, control->entity->id, chain->dev->intfnum, control->info.selector, control->cur, control->info.size);
		if (ret < 0) {
			udbg("Failed to query the control(%d) to device!!\n", control->info.selector);
			return ret;
		}
		ret = OK;
		CTRL_SET_LOADED(control->status);
	}

	val = uvc_get_value(mapping, UVC_GET_CUR, control->cur);

	if (mapping->v4l2_type == V4L2_CTRL_TYPE_MENU) {
		menu = mapping->menu_info;

		for (i = 0; i < mapping->menu_count; ++i, ++menu) {
			if (menu->value == val) {
				val = i;
				break;
			}
		}
	}

	xctrl->value = val;

	return OK;
}

/****************************************************************************
 * Name: uvc_ctrl_set
 *
 * Description:
 *   This function is used to set the control value to the camera device.
 *   Will verify the range to which the value can be set on device, also backup
 *   the previous values to restore when required.
 *
 *   This function is called from the uvc_set_ctrlvalue method of V4L2.
 *
 * Input Parameters:
 *   chain - reference to the video chain object.
 *   ctrl_class - class the control command to be set.
 *   xctrl - control information to be set on device.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_set(struct uvc_video_chain *chain, uint16_t ctrl_class, struct v4l2_ext_control *xctrl)
{
	struct uvc_control *ctrl;
	struct uvc_ctrl_value val;
	struct uvc_control_mapping *mapping;
	int32_t value;
	uint32_t step;
	int32_t min;
	int32_t max;
	int ret;

	ctrl = uvc_search_control(chain, xctrl->id, ctrl_class, &mapping);
	if (ctrl == NULL) {
		udbg("Failed to find a valid control!!\n");
		return -EINVAL;
	}
	if (!(ctrl->info.flags & F_SET_CUR)) {
		uvdbg("Invalid access to control, not supported!!\n");
		return -EACCES;
	}

	/* range check, value should be in the range of min and max resoluton of steps */
	switch (mapping->v4l2_type) {
	case V4L2_CTRL_TYPE_INTEGER:
		ret = uvc_ctrl_populate_values(chain, ctrl);
		if (ret < 0) {
			uvdbg("Failed to populate control values\n");
			return ret;
		}

		uvc_read_control(ctrl, mapping, &val);
		min = val.min;
		max = val.max;
		step = val.step;
		if (step == 0) {
			step = 1;
		}

		CHECK_RANGE(xctrl->value, min, max, step);

		xctrl->value = min + (((uint32_t)(xctrl->value - min) + step / 2) / (step * step));
		value = xctrl->value;
		break;

	case V4L2_CTRL_TYPE_BOOLEAN:
		CHECK_RANGE(xctrl->value, 0, 1, 1);
		value = xctrl->value;
		break;

	case V4L2_CTRL_TYPE_MENU:
		if (xctrl->value < 0 || xctrl->value >= mapping->menu_count) {
			return -ERANGE;
		}
		value = mapping->menu_info[xctrl->value].value;

		/* Valid menu indices are reported by the GET_RES request for
		 * UVC controls that support it.
		 */
		if (mapping->data_type == UVC_CTRL_DATA_TYPE_BITMASK && (ctrl->info.flags & F_GET_RES)) {
			ret = uvc_ctrl_populate_values(chain, ctrl);
			if (ret < 0) {
				udbg("Failed to populate control values!!\n");
				return ret;
			}

			step = uvc_get_value(mapping, UVC_GET_RES, ctrl->step);
			if (!(step & value)) {
				uvdbg("Failed to get steps from control\n");
				return -EPERM;
			}
		}
		break;

	default:
		value = xctrl->value;
		break;
	}

	if (!CTRL_IS_LOADED(ctrl->status) && (ctrl->info.size * 8) != mapping->size) {
		if ((ctrl->info.flags & F_GET_CUR) == 0) {
			memset(ctrl->cur, 0, ctrl->info.size);
		} else {
			ret = uvc_query_ctrl(chain->dev, UVC_GET_CUR, ctrl->entity->id, chain->dev->intfnum, ctrl->info.selector, ctrl->cur, ctrl->info.size);
			if (ret < 0) {
				udbg("Failed to query the control(%d) to device!!\n", ctrl->info.selector);
				return ret;
			}
			ret = OK;
		}

		CTRL_SET_LOADED(ctrl->status);
	}

	/* Backup the current value in case we need to rollback later. */
	if (!CTRL_IS_DIRTY(ctrl->status)) {
		memcpy(ctrl->bak, ctrl->cur, ctrl->info.size);
	}

	uvc_set_value(mapping, value, ctrl->cur);

	CTRL_SET_DIRTY(ctrl->status);
	CTRL_SET_MODIFIED(ctrl->status);
	return 0;
}

/****************************************************************************
 * Name: uvc_ctrl_restore_values
 *
 * Description:
 *   This function restore the value on resume from a low power cycle.
 *
 *   This function is called from the resume method of driver.
 *
 * Input Parameters:
 *   dev - reference to the driver object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_restore_values(uvc_state_t *dev)
{
	struct uvc_control *ctrl;
	struct uvc_entity *entity = queue_entry(dev->entities.head, struct uvc_entity, list);
	unsigned int i;
	int ret = OK;

	/* Walk the entities list and restore controls when possible. */
	for (; &entity->list != (dq_entry_t *)&dev->entities; entity = queue_entry(entity->list.flink, struct uvc_entity, list)) {

		for (i = 0; i < entity->ncontrols; ++i) {
			ctrl = &entity->controls[i];

			if (!CTRL_IS_SUPPORTED(ctrl->status) || !CTRL_IS_MODIFIED(ctrl->status) || (ctrl->info.flags & F_RESTORE) == 0) {
				continue;
			}

			uvdbg("restoring control %pUl/%u/%u\n", ctrl->info.entity, ctrl->info.index, ctrl->info.selector);
			CTRL_SET_DIRTY(ctrl->status);

			if (ctrl->info.flags & F_AUTO || !(ctrl->info.flags & F_GET_CUR)) {
				CTRL_CLEAR_LOADED(ctrl->status);
			}

			ret = uvc_query_ctrl(dev, UVC_SET_CUR, ctrl->entity->id, dev->intfnum, ctrl->info.selector, ctrl->cur, ctrl->info.size);

			if (ret < 0) {
				memcpy(ctrl->cur, ctrl->bak, ctrl->info.size);
			} else {
				ret = OK;
			}

			CTRL_CLEAR_DIRTY(ctrl->status);
		}
	}

	return ret;
}

/****************************************************************************
 * Name: allocate_ctrl_vals
 *
 * Description:
 *   This function allocate memory for min/max/default/current/backup.
 *
 *   This function is called from the uvc_ctrl_add_info method.
 *
 * Input Parameters:
 *   ctrl - reference to the control object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int allocate_ctrl_vals(struct uvc_control *ctrl)
{
	int ret = OK;
	/* Allocate memory for each of the controls */
	ctrl->min = kmm_zalloc(ctrl->info.size);
	if (ctrl->min == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	ctrl->max = kmm_zalloc(ctrl->info.size);
	if (ctrl->max == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	ctrl->cur = kmm_zalloc(ctrl->info.size);
	if (ctrl->cur == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	ctrl->step = kmm_zalloc(ctrl->info.size);
	if (ctrl->step == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	ctrl->def = kmm_zalloc(ctrl->info.size);
	if (ctrl->def == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	ctrl->bak = kmm_zalloc(ctrl->info.size);
	if (ctrl->bak == NULL) {
		ret = -ENOMEM;
		goto done;
	}
done:
	if (ret < 0) {
		if (ctrl->min) {
			kmm_free(ctrl->min);
		}
		if (ctrl->max) {
			kmm_free(ctrl->max);
		}
		if (ctrl->cur) {
			kmm_free(ctrl->cur);
		}
		if (ctrl->bak) {
			kmm_free(ctrl->bak);
		}
		if (ctrl->step) {
			kmm_free(ctrl->step);
		}
		if (ctrl->def) {
			kmm_free(ctrl->def);
		}
	}
	return ret;
}

/****************************************************************************
 * Name: uvc_ctrl_add_info
 *
 * Description:
 *   This function allocate memory for min/max/default/current/backup, retrives
 *   the capabilities supported by the device.
 *
 *   This function is called from the uvc_init_device_ctrl method.
 *
 * Input Parameters:
 *   dev - reference to driver object.
 *   ctrl - reference to the conrol to be initialized.
 *   info - reference to the control info.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_add_info(uvc_state_t *dev, struct uvc_control *ctrl, const struct uvc_control_info *info)
{
	uint8_t *data;
	int ret = 0;

	ctrl->info = *info;
	INIT_QUEUE_HEAD(&ctrl->info.mappings);

	/* Allocate an array to save control values (cur, def, max, etc.) */
	ret = allocate_ctrl_vals(ctrl);
	if (ret < 0) {
		udbg("Failed to allocate memory!!\n");
		return ret;
	}

	/* Ignore the error and work with default values */
	data = kmm_zalloc(1);
	if (data == NULL) {
		udbg("Failed to allocate memory!!\n");
		ret = -ENOMEM;
		return ret;
	}

	ret = uvc_query_ctrl(dev, UVC_GET_INF, ctrl->entity->id, dev->intfnum, ctrl->info.selector, data, 1);
	if (ret >= 0) {
		ctrl->info.flags |= (data[0] & UVC_CONTROL_CAP_GET ? F_GET_CUR : 0)
							| (data[0] & UVC_CONTROL_CAP_SET ? F_SET_CUR : 0)
							| (data[0] & UVC_CONTROL_CAP_AUTOUPDATE ? F_AUTO : 0)
							| (data[0] & UVC_CONTROL_CAP_ASYNCHRONOUS ? F_ASYNCH : 0);
		ret = OK;
	}

	kmm_free(data);

	CTRL_SET_SUPPORTED(ctrl->status);

	return ret;
}

/****************************************************************************
 * Name: uvc_init_device_ctrl
 *
 * Description:
 *   This initialize the mapping information for UVC controls,
 *
 *   This function is called from the uvc_ctrl_init_device method.
 *
 * Input Parameters:
 *   dev - A reference to driver object.
 *   ctrl - A reference to the conrol to be initialized.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_init_device_ctrl(uvc_state_t *dev, struct uvc_control *ctrl)
{
	unsigned int size;
	struct uvc_control_mapping *map;
	const struct uvc_control_info *info = uvc_ctrls;
	const struct uvc_control_info *iend = info + ARRAY_SIZE(uvc_ctrls);
	const struct uvc_control_mapping *mapping = uvc_ctrl_mappings;
	const struct uvc_control_mapping *mend = mapping + ARRAY_SIZE(uvc_ctrl_mappings);

	/* Only the processing unit, input unit controls will be taken care here */
	if (UVC_ENTITY_TYPE(ctrl->entity) == UVC_VC_EXTENSION_UNIT) {
		udbg("Invalid entity!!\n");
		return OK;
	}

	for (; info < iend; ++info) {
		if (uvc_entity_match_guid(ctrl->entity, info->entity) && ctrl->index == info->index) {
			uvc_ctrl_add_info(dev, ctrl, info);
			break;
		}
	}

	if (!CTRL_IS_SUPPORTED(ctrl->status)) {
		return OK;
	}

	for (; mapping < mend; ++mapping) {
		if (uvc_entity_match_guid(ctrl->entity, mapping->entity) && ctrl->info.selector == mapping->selector) {

			map = kmm_zalloc(sizeof(*mapping));
			if (map == NULL) {
				return -ENOMEM;
			}

			uvc_init_mapping(mapping);

			memcpy(map, mapping, sizeof(*mapping));

			size = sizeof(*mapping->menu_info) * mapping->menu_count;
			if (size) {
				map->menu_info = kmm_zalloc(size);
				if (map->menu_info == NULL) {
					kmm_free(map);
					return -ENOMEM;
				}

				memcpy(map->menu_info, mapping->menu_info, size);
			}

			dq_addfirst(&map->list, &ctrl->info.mappings);
		}
	}

	return OK;
}

/****************************************************************************
 * Name: uvc_create_controls
 *
 * Description:
 *   This function walk through each of the controls supported by each unit,
 *   allocate resource and initialize the mapping information for UVC controls,
 *
 *   This function is called from the uvc_ctrl_init_device method.
 *
 * Input Parameters:
 *   entity - Unit to be parsed for control.
 *   bmContols - Object to controls.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int uvc_create_controls(struct uvc_entity *entity, uint8_t **bmControls)
{
	unsigned int i;
	unsigned int bControlSize = 0, ncontrols = 0;

	if (UVC_ENTITY_TYPE(entity) == UVC_VC_EXTENSION_UNIT) {
		*bmControls = entity->extension.bmControls;
		bControlSize = entity->extension.bControlSize;
	} else if (UVC_ENTITY_TYPE(entity) == UVC_VC_PROCESSING_UNIT) {
		*bmControls = entity->processing.bmControls;
		bControlSize = entity->processing.bControlSize;
	} else if (UVC_ENTITY_TYPE(entity) == UVC_ITT_CAMERA) {
		*bmControls = entity->camera.bmControls;
		bControlSize = entity->camera.bControlSize;
	}

	/* Count supported controls and allocate the controls array */
	for (i = 0; i < bControlSize * 8; ++i) {
		if (uvc_test_bit(*bmControls, i) == 0) {
			continue;
		}
		ncontrols++;
	}
	if (ncontrols == 0) {
		return 0;
	}

	entity->controls = (struct uvc_control *)kmm_zalloc(ncontrols * sizeof(struct uvc_control));
	if (entity->controls == NULL) {
		udbg("Failed to allocate memory for control object!!\n");
		return -ENOMEM;
	}
	entity->ncontrols = ncontrols;

	return bControlSize;
}

/****************************************************************************
 * Name: uvc_ctrl_init_device
 *
 * Description:
 *   This function walk through each of the terminal entities, allocate
 *   resource and initialize the mapping information for UVC controls,
 *
 *   This function is called from the usbhost_uvc_devinit method.
 *
 * Input Parameters:
 *   dev - A reference to the driver instance.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
int uvc_ctrl_init_device(FAR uvc_state_t *dev)
{
	struct uvc_entity *entity = queue_entry(dev->entities.head, struct uvc_entity, list);
	unsigned int i;
	int ret = OK;

	/* Walk the entities list and instantiate controls */
	for (; &entity->list != (dq_entry_t *)&dev->entities; entity = queue_entry(entity->list.flink, struct uvc_entity, list)) {
		struct uvc_control *ctrl;
		unsigned int bControlSize = 0;
		uint8_t *bmControls = NULL;

		ret = uvc_create_controls(entity, &bmControls);
		if (ret < 0) {
			udbg("Failed to create control object!!\n");
			return ret;
		}

		bControlSize = ret;

		/* Initialize all supported controls */
		ctrl = entity->controls;
		for (i = 0; i < bControlSize * 8; ++i) {
			if (uvc_test_bit(bmControls, i) == 0) {
				continue;
			}

			ctrl->entity = entity;
			ctrl->index = i;

			ret = uvc_init_device_ctrl(dev, ctrl);
			if (ret < 0) {
				udbg("Failed to initialize device control!!\n");
				return ret;
			}
			ctrl++;
		}
	}

	return ret;
}

/****************************************************************************
 * Name: uvc_ctrl_deinit_device
 *
 * Description:
 *   This function deintializes the allocated resource for UVC controls
 *
 *   This function is called from the usbhost_uvc_destroy method.
 *
 * Input Parameters:
 *   dev - A reference to the driver instance.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
void uvc_ctrl_deinit_device(uvc_state_t *dev)
{
	unsigned int i;
	struct uvc_control_mapping *mapping, *map;
	struct uvc_entity *entity = queue_entry(dev->entities.head, struct uvc_entity, list);

	/* Free controls and control mappings for all entities. */
	for (; &entity->list != (dq_entry_t *)&dev->entities; entity = queue_entry(entity->list.flink, struct uvc_entity, list)) {
		for (i = 0; i < entity->ncontrols; ++i) {
			struct uvc_control *ctrl = &entity->controls[i];

			if (!CTRL_IS_SUPPORTED(ctrl->status)) {
				continue;
			}
			mapping = queue_entry(ctrl->info.mappings.head, struct uvc_control_mapping, list);
			map = queue_entry(mapping->list.flink, struct uvc_control_mapping, list);

			for (; &mapping->list != (dq_entry_t *)&ctrl->info.mappings; mapping = map, map = queue_entry(map->list.flink, struct uvc_control_mapping, list)) {
				if (mapping->menu_info) {
					kmm_free(mapping->menu_info);
				}
				kmm_free(mapping);
			}
			if (ctrl->min) {
				kmm_free(ctrl->min);
			}
			if (ctrl->max) {
				kmm_free(ctrl->max);
			}
			if (ctrl->step) {
				kmm_free(ctrl->step);
			}
			if (ctrl->def) {
				kmm_free(ctrl->def);
			}
			if (ctrl->cur) {
				kmm_free(ctrl->cur);
			}
			if (ctrl->bak) {
				kmm_free(ctrl->bak);
			}
		}

		kmm_free(entity->controls);
	}
}
