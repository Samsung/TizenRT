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

#include "scsc_logring.h"
#include "scsc_mx_impl.h"
#include "mxmgmt_transport.h"
#include "mxlog_transport.h"
#include "mxlog.h"

/** Receive handler for messages from the FW along the maxwell management transport */
/* mxlog phase 3 */
static void mxlog_message_handler(const void *message, void *data)
{
	struct mxlog *mxlog = (struct mxlog *)data;
	unsigned char *payload = (unsigned char *)message;
	int m, i;

	/*
	 * Encoding of payload is:
	 * | len | b1 | b2 | b3 | b4 | b5 | b6 |
	 * e.g.
	 * | 6 | 'h' | 'e' | 'l' | 'l' | 'o' | '\0' |
	 * or
	 * | 4 | 'b' | 'y' | 'e' | '\0' | ignore | ignore |
	 */
	m = payload[0] + 1;
	for (i = 1; i < m; i++) {
		mxlog->buffer[mxlog->index++] = payload[i];
		if (payload[i] == '\0') {
			/* Catch a log message without carriage return. */
			if ((mxlog->index > 2) && (mxlog->buffer[mxlog->index - 2] != '\n') && (mxlog->index < MXLOG_BUFFER_SIZE - 1)) {
				mxlog->buffer[mxlog->index - 1] = '\n';
				mxlog->buffer[mxlog->index] = '\0';
			}
			SCSC_PRINTK_TAG(MX_FW, "%s\n", mxlog->buffer);
			mxlog->index = 0;
		}
		if (mxlog->index >= MXLOG_BUFFER_SIZE - 1) {
			mxlog->index = 0;
		}
	}
}

#ifdef MX_LOG_PHASE_4
/* mxlog phase 4 */
static void mxlog_phase4_message_handler(const void *message, size_t length, u32 level, void *data)
{
	/*struct mxlog  *mxlog = (struct mxlog *)data; */
	unsigned char *buf = (unsigned char *)message;

	SCSC_PRINTK_TAG(MX_FW, "%d: %s\n", (int)length, buf);
}
#endif

void mxlog_init(struct mxlog *mxlog, struct scsc_mx *mx)
{
	mxlog->mx = mx;

	/* register channel handler */
	/* mxlog phase 3 */
	mxlog->index = 0;
	mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mx), MMTRANS_CHAN_ID_MAXWELL_LOGGING, &mxlog_message_handler, mxlog);
#ifdef MX_LOG_PHASE_4
	/* mxlog phase 4 */
	mxlog_transport_register_channel_handler(scsc_mx_get_mxlog_transport(mx), &mxlog_phase4_message_handler, mxlog);
#endif
}

void mxlog_release(struct mxlog *mxlog)
{
	/* unregister channel handler */
	/* mxlog phase 3 */
	mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mxlog->mx), MMTRANS_CHAN_ID_MAXWELL_LOGGING, NULL, NULL);
#ifdef MX_LOG_PHASE_4
	/* mxlog phase 4 */
	mxlog_transport_register_channel_handler(scsc_mx_get_mxlog_transport(mxlog->mx), NULL, NULL);
#endif
}
