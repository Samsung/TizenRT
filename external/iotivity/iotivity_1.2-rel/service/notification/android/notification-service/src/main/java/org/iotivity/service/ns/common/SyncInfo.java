//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

package org.iotivity.service.ns.common;

import android.util.Log;

/**
 *
 * This class provides implementation of Notification SyncInfo object.
 *
 */
public class SyncInfo {

    private static final String LOG_TAG = "NotificationService_SyncInfo";

    /**
     * This enum is used to inform about read status of notification message
     */
    public enum SyncType {
        UNREAD(0),
        READ(1),
        DELETED(2);

        private int type;

        private SyncType(int type) {
            this.type = type;
        }

        public int getSyncType() {
            return this.type;
        }
    };

    private long     mMessageId  = 0;
    private String   mProviderId = null;
    private SyncType mState      = SyncType.UNREAD;

    public SyncInfo(long messageId, String providerId, SyncType state) {
        Log.i(LOG_TAG, "SyncInfo()");

        mMessageId = messageId;
        mProviderId = providerId;
        mState = state;
    }

    public long getMessageId() {
        return mMessageId;
    }

    public String getProviderId() {
        return mProviderId;
    }

    public SyncType getState() {
        return mState;
    }
}
