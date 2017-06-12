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
import org.iotivity.base.OcRepresentation;

/**
 *
 * This class provides implementation of Notification Message object.
 *
 */
public class Message {

    private static final String LOG_TAG = "NotificationService_Message";

    /**
     * This enum is used to represent different types of notification messages
     */
    public enum MessageType {
        ALERT(1),
        NOTICE(2),
        EVENT(3),
        INFO(4),
        WARNING(5);

        private int type;

        private MessageType(int type) {
            this.type = type;
        }

        public int getMessageType() {
            return this.type;
        }
    };

    private long             mMessageId     = 0;
    private String           mProviderId    = null;

    private String           mSourceName    = null;
    private MessageType      mType          = MessageType.ALERT;
    private String           mTime          = null;
    private long             mTTL           = 0;
    private String           mTitle         = null;
    private String           mContentText   = null;
    private MediaContents    mMediaContents = null;
    private String           mTopic         = null;
    private OcRepresentation mExtraInfo     = null;

    public Message(String title, String contentText, String sourceName) {
        Log.i(LOG_TAG, "Message()");

        mTitle = title;
        mContentText = contentText;
        mSourceName = sourceName;
    }

    public long getMessageId() {
        return mMessageId;
    }

    public String getProviderId() {
        return mProviderId;
    }

    public String getSourceName() {
        return mSourceName;
    }

    public MessageType getType() {
        return mType;
    }

    public String getTime() {
        return mTime;
    }

    public long getTTL() {
        return mTTL;
    }

    public String getTitle() {
        return mTitle;
    }

    public String getContentText() {
        return mContentText;
    }

    public MediaContents getMediaContents() {
        return mMediaContents;
    }

    public String getTopic() {
        return mTopic;
    }

    public OcRepresentation getExtraInfo() {
        return mExtraInfo;
    }

    public void setSourceName(String sourceName) {
        mSourceName = sourceName;
    }

    public void setType(MessageType type) {
        mType = type;
    }

    public void setTime(String time) {
        mTime = time;
    }

    public void setTTL(long ttl) {
        mTTL = ttl;
    }

    public void setTitle(String title) {
        mTitle = title;
    }

    public void setContentText(String contextText) {
        mContentText = contextText;
    }

    public void setMediaContents(MediaContents mediaContents) {
        mMediaContents = mediaContents;
    }

    public void setTopic(String topic) {
        mTopic = topic;
    }

    public void setExtraInfo(OcRepresentation extraInfo) {
        mExtraInfo = extraInfo;
    }
}
