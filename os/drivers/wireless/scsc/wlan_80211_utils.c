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
#include <sys/types.h>
#include <stddef.h>
#include "wlan_80211_utils.h"

const u8 *slsi_80211_find_ie(u8 eid, const u8 *ies, int len)
{
	if (ies == NULL) {
		return NULL;
	}

	while (len > 2 && ies[0] != eid) {
		len -= ies[1] + 2;
		ies += ies[1] + 2;
	}
	if (len < 2) {
		return NULL;
	}
	if (len < 2 + ies[1]) {
		return NULL;
	}
	return ies;
}

u8 *slsi_80211_find_ie_mod(u8 eid, u8 *ies, int len)
{
	if (ies == NULL) {
		return NULL;
	}

	while (len > 2 && ies[0] != eid) {
		len -= ies[1] + 2;
		ies += ies[1] + 2;
	}
	if (len < 2) {
		return NULL;
	}
	if (len < 2 + ies[1]) {
		return NULL;
	}
	return ies;
}

const u8 *slsi_80211_find_vendor_ie(unsigned int oui, u8 oui_type, const u8 *ies, int len)
{
	struct slsi_80211_vendor_ie *ie;
	const u8 *pos = ies, *end = ies + len;
	int ie_oui;

#define WLAN_EID_VENDOR_SPECIFIC 221

	while (pos < end) {
		pos = slsi_80211_find_ie(WLAN_EID_VENDOR_SPECIFIC, pos, end - pos);
		if (!pos) {
			return NULL;
		}

		ie = (struct slsi_80211_vendor_ie *)pos;

		/* make sure we can access ie->len */
		if (offsetof(struct slsi_80211_vendor_ie, len) != 1) {
			SLSI_NET_WARN(NULL, "Unable to access ie len \n");
		}

		if (ie->len < sizeof(*ie)) {
			goto cont;
		}

		ie_oui = ie->oui[0] << 16 | ie->oui[1] << 8 | ie->oui[2];
		if (ie_oui == oui && ie->oui_type == oui_type) {
			return pos;
		}
cont:
		pos += 2 + ie->len;
	}
	return NULL;
}

int slsi_80211_channel_to_frequency(int chan, enum slsi_80211_band band)
{
	/* see 802.11 17.3.8.3.2 and Annex J
	 * there are overlapping channel numbers in 5GHz and 2GHz bands */
	if (chan <= 0) {
		return 0;    /* not supported */
	}
	switch (band) {
	case SLSI_80211_BAND_2GHZ:
		if (chan == 14) {
			return 2484;
		} else if (chan < 14) {
			return 2407 + chan * 5;
		}
		break;
	case SLSI_80211_BAND_5GHZ:
		if (chan >= 182 && chan <= 196) {
			return 4000 + chan * 5;
		} else {
			return 5000 + chan * 5;
		}
		break;
	default:
		;
	}
	return 0;					/* not supported */
}
