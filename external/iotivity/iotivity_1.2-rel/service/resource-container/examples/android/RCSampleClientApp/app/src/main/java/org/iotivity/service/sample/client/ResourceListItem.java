/******************************************************************
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 * <p>
 * <p>
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.service.sample.client;

import android.os.Handler;
import android.util.Log;

import org.iotivity.service.RcsException;
import org.iotivity.service.RcsResourceAttributes;
import org.iotivity.service.client.RcsRemoteResourceObject;

public class ResourceListItem {
    private final String LOG_TAG = "[RCSampleClient] "
            + this.getClass().getSimpleName();

    public String mResourceUri;
    public RcsRemoteResourceObject mResource;
    public RemoteResourceGetListener mGetListener;
    public RemoteResourceCacheListener mCacheListener;
    public boolean mCacheStarted;

    private Handler mHandler;

    public ResourceListItem(String resourceUri, RcsRemoteResourceObject resource, Handler handler)
    {
        mResourceUri = resourceUri;
        mResource = resource;

        mGetListener = new RemoteResourceGetListener();
        mCacheListener = new RemoteResourceCacheListener();
        mCacheStarted = false;

        mHandler = handler;
    }

    private class RemoteResourceGetListener implements RcsRemoteResourceObject.OnRemoteAttributesReceivedListener
    {

        @Override
        public void onAttributesReceived(final RcsResourceAttributes rcsResourceAttributes, int eCode) {
            Log.i(LOG_TAG, "onAttributesReceived -- " + mResourceUri);

            new Thread(new Runnable() {

                @Override
                public void run() {
                    try {
                        String logMessage;
                        logMessage = "-- " + mResourceUri + "\n";
                        logMessage += Utils.resourceAttributes(rcsResourceAttributes);

                        mHandler.obtainMessage(ContainerClientActivity.MSG_ID_ATTRIBUTE_RECEIVED, logMessage).sendToTarget();
                    } catch (RcsException e) {
                        e.printStackTrace();
                    }
                 }
            }).run();
        }
    }

    private class RemoteResourceCacheListener implements RcsRemoteResourceObject.OnCacheUpdatedListener
    {

        @Override
        public void onCacheUpdated(final RcsResourceAttributes rcsResourceAttributes) {
            Log.i(LOG_TAG, "onCacheUpdated -- " + mResourceUri);

            new Thread(new Runnable() {

                @Override
                public void run() {
                    try {
                        String logMessage;
                        logMessage = "-- " + mResourceUri + "\n";
                        logMessage += Utils.resourceAttributes(rcsResourceAttributes);

                        mHandler.obtainMessage(ContainerClientActivity.MSG_ID_CACHEDATA_RECEIVED, logMessage).sendToTarget();
                    } catch (RcsException e) {
                        e.printStackTrace();
                    }
                }
            }).run();
        }
    }
}