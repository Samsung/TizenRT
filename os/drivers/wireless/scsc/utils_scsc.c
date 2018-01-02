/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include "utils_scsc.h"

bool slsi_is_broadcast_addr(const u8 *a)
{
	return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

bool slsi_is_mgmt_frame(le16 frame_ctrl)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_WLAN_FRM_CTRL_TYPE)) == cpu_to_le16(SLSI_WLAN_FRM_CTRL_TYPE_MGMT));
}

u8 *slsi_wlan_get_dest_addr(struct slsi_wlan_frame_header *header)
{
	bool val = ((header->frame_ctrl & cpu_to_le16(SLSI_WLAN_FRM_CTRL_TO_DS)) == SLSI_WLAN_FRM_CTRL_TO_DS);

	return val ? header->addr_3 : header->addr_1;
}

u16 slsi_wlan_channel_to_freq(int chan)
{
	if (chan <= 0) {
		return 0;		/* Not supported */
	} else if (chan < 14) {		/* 2.4 GHz channels */
		return 2407 + chan * 5;
	} else if (chan == 14) {
		return 2484;
	} else if (chan >= 36) {	/* 5 GHz channels */
		return 5000 + chan * 5;
	}

	return 0;	/* Not supported */
}

u32 slsi_get_center_freq1(struct slsi_dev *sdev, u16 chann_info, u16 center_freq)
{
	u32 center_freq1 = 0x0000;

	SLSI_UNUSED_PARAMETER(sdev);

	switch (chann_info & 0xFF) {
	case 40:
		center_freq1 = center_freq - 20 * ((chann_info & 0xFF00) >> 8) + 10;
		break;

	case 80:
		center_freq1 = center_freq - 20 * ((chann_info & 0xFF00) >> 8) + 30;
		break;
	default:
		break;
	}
	return center_freq1;
}

u8 *slsi_wlan_find_ie_mod(u8 eid, u8 *ies, int len)
{
	u8 offset;

	if (ies == NULL) {
		return NULL;
	}

	while (len > SLSI_WLAN_MIN_IE_LENGTH) {
		offset = SLSI_WLAN_MIN_IE_LENGTH + ies[1];

		if (len < offset) {
			break;
		}

		if (ies[0] == eid) {
			return ies;
		}

		len -= offset;
		ies += offset;
	}

	return NULL;
}

const u8 *slsi_wlan_find_ie(u8 eid, const u8 *ies, int len)
{
	const u8 *ie = (const u8 *)slsi_wlan_find_ie_mod(eid, (u8 *)ies, len);
	return ie;
}

const u8 *slsi_wlan_find_vendor_spec_ie(u32 oui, u8 oui_type, const u8 *ies, int len)
{
	const u8 *vs_ie = ies, *end = ies + len;
	int rem_len = len;
	u32 vs_ie_oui;
	u8 vs_ie_oui_type;

	while (rem_len > SLSI_WLAN_MIN_IE_LENGTH) {
		vs_ie = slsi_wlan_find_ie(SLSI_WLAN_EID_VENDOR_SPEC, vs_ie, rem_len);
		if (vs_ie) {
			/* Vendor Spec IE format - EID + LEN + OUI[0] + OUI[1] + OUI[2] + OUI_TYPE + ... */
			if (vs_ie[1] >= 6) {
				vs_ie_oui = (vs_ie[2] << 16) | (vs_ie[3] << 8) | (vs_ie[4]);
				vs_ie_oui_type = vs_ie[5];

				if (vs_ie_oui == oui && vs_ie_oui_type == oui_type) {
					return vs_ie;
				}

			}

			vs_ie += SLSI_WLAN_MIN_IE_LENGTH + vs_ie[1];
			rem_len = end - vs_ie;
		} else {
			break;	/* Vendor Specific IE not found */
		}
	}

	return NULL;
}

