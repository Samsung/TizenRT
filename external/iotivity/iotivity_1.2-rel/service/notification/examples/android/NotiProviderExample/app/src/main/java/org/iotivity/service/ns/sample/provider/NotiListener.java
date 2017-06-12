/*
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
 */

package org.iotivity.service.ns.sample.provider;

import android.app.Notification;
import android.os.Bundle;
import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import android.util.Log;
import org.iotivity.service.ns.common.MediaContents;
import org.iotivity.service.ns.common.Message;
import java.util.ArrayList;

public class NotiListener extends NotificationListenerService {

    private final String          TAG              = "NS_JNI_NOTI_LISTENER";
    private static ProviderSample mProviderSample  = null;
    private MainActivity          mActivity        = null;
    ArrayList                     mBlackSourceList = new ArrayList<String>();

    public NotiListener() {

        Log.i(TAG, "Create NotiListener");
    }

    public NotiListener(MainActivity activity) {

        Log.i(TAG, "Create NotiListener with MainActivity");

        this.mActivity = activity;
        this.mProviderSample = mActivity.getProviderSample();

        setBlackSourceList();

        if (mProviderSample == null) {
            Log.i(TAG, "Fail to get providerProxy instance");
        }
    }

    public void setBlackSourceList() {

        // set blacklist of app package name not to receive notification
        mBlackSourceList.add("android");
        mBlackSourceList.add("com.android.systemui");
    }

    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {
        super.onNotificationPosted(sbn);
        Log.i(TAG, "Notification posted By package" + sbn.getPackageName());

        Bundle bundle = sbn.getNotification().extras;
        String source = null;

        // prevent not to send notification
        for (int i = 0; i < mBlackSourceList.size(); ++i) {
            if (sbn.getPackageName().equals(mBlackSourceList.get(i))) {
                return;
            }
        }

        // filter exception case : Some notification are generated twice
        if (sbn.getId() > 10000 || sbn.getId() < 0) {
            return;
        }

        // Temporary protocol code to display ICON on consumer app.
        // For example, consumer app shows KAKAOTALK Icon when receiving
        // Notification with SOURCE
        // that is set to KAKAO, otherwise it displays OCF Icon on current
        // sample app.
        if (sbn.getPackageName().equals("com.kakao.talk")) {
            source = "KAKAO";
        }
        else {
            source = "OCF";
        }

        Log.i(TAG, "Noti. Package Name : " + sbn.getPackageName());
        Log.i(TAG, "Noti. ID : " + sbn.getId());

        String id = Integer.toString(sbn.getId());
        String title = bundle.getString(Notification.EXTRA_TITLE, "");
        String body = bundle.getString(Notification.EXTRA_TEXT, "");
        String topic = bundle.getString(Notification.EXTRA_SUB_TEXT, "");

        Log.i(TAG, "onNotificationPosted .. ");
        Log.i(TAG, "source : " + source);
        Log.i(TAG, "Id : " + id);
        Log.i(TAG, "Title : " + title);
        Log.i(TAG, "Body : " + body);

        Message notiMessage = mProviderSample.createMessage();
        if(notiMessage == null)
        {
            Log.e(TAG, "CreateMessage Failed");
            return;
        }
        Log.i(TAG, "Message ID : " + notiMessage.getMessageId());
        Log.i(TAG, "Provider ID : " + notiMessage.getProviderId());
        notiMessage.setTitle(title);
        notiMessage.setContentText(body);
        notiMessage.setTopic(topic);
        notiMessage.setSourceName(source);
        notiMessage.setTTL(10);
        notiMessage.setTime("12:10");
        MediaContents media = new MediaContents("daasd");
        notiMessage.setMediaContents(media);
        if (mProviderSample != null) {
            mProviderSample.sendMessage(notiMessage);
        } else {
            Log.i(TAG, "providerExample is NULL");
        }
    }

    @Override
    public void onNotificationRemoved(StatusBarNotification sbn) {
        super.onNotificationRemoved(sbn);

        Bundle bundle = sbn.getNotification().extras;

        if (sbn.getPackageName().equals("android")) {
            return;
        }

        Log.i(TAG, "Noti. Package Name : " + sbn.getPackageName());
        Log.i(TAG, "Noti. ID : " + sbn.getId());

        if (mProviderSample.getMsgMap().containsKey(sbn.getId())) {
            if (mProviderSample.getMsgMap().get(sbn.getId()) == 2) {
                org.iotivity.service.ns.common.SyncInfo.SyncType type = org.iotivity.service.ns.common.SyncInfo.SyncType.READ;
                mProviderSample.sendSyncInfo(1, type);
            }
        }
    }
}
