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

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;

import org.iotivity.service.RcsException;
import org.iotivity.service.client.RcsAddress;
import org.iotivity.service.client.RcsDiscoveryManager;
import org.iotivity.service.client.RcsRemoteResourceObject;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

/**
 * It contains the discover resource API for Discovering Container Resource
 */

public class ContainerClientActivity extends Activity implements
        RcsDiscoveryManager.OnResourceDiscoveredListener {
    public static final int MSG_ID_ATTRIBUTE_RECEIVED  = 0;
    public static final int MSG_ID_CACHEDATA_RECEIVED  = 1;

    private final String LOG_TAG = "[SampleClient] "
                                         + this.getClass().getSimpleName();
    private final String RESOURCE_DISCOVERY_URI_KEYWORD = "discomfort";

    private Handler    mHandler;

    private ResourceListAdapter mResourceListViewAdapter;

    private ListView mResourceListView;
    private TextView     mLogView;

    private List<String> mFoundResourceUris;
    private List<ResourceListItem> mFoundResources;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_container_client);

        mFoundResourceUris = new ArrayList<String>();
        mFoundResources = new ArrayList<ResourceListItem>();

        mHandler = new ContainerClientHandler(this);

        mResourceListViewAdapter = new ResourceListAdapter(this, R.layout.discovered_resource, mFoundResources);
        mResourceListView = (ListView) findViewById(R.id.ResourceListView);
        mResourceListView.setAdapter(mResourceListViewAdapter);

        mLogView = (TextView) findViewById(R.id.log);
    }

    public void onDiscoverResourceClick(View v) {
        try {
            RcsDiscoveryManager.getInstance().discoverResource(
                    RcsAddress.multicast(),
                    ContainerClientActivity.this);

            mFoundResourceUris.clear();
            mFoundResources.clear();
            mResourceListViewAdapter.notifyDataSetChanged();

            mLogView.setText("");
        } catch (RcsException e) {
            e.printStackTrace();
        }
    }

    private boolean IsFoundResource(String resourceUri)
    {
        if (mFoundResourceUris.contains(resourceUri))
            return true;

        return false;
    }

    @Override
    public void onResourceDiscovered(
            final RcsRemoteResourceObject discoveredResource) {

        runOnUiThread(new Runnable() {
            public void run() {
                try {
                    String uri = discoveredResource.getUri();
                    Log.i(LOG_TAG, "onResourceDiscovered -- " + uri);

                    if (uri.contains(RESOURCE_DISCOVERY_URI_KEYWORD) && !IsFoundResource(uri)) {
                        mFoundResourceUris.add(uri);
                        mFoundResources.add(new ResourceListItem(uri, discoveredResource, ContainerClientActivity.this.getHandler()));
                        mResourceListViewAdapter.notifyDataSetChanged();
                    }
                } catch (RcsException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private void printLog(String message) {
        Log.i(LOG_TAG, message);
        mLogView.setText(message);
    }

    public Handler getHandler() {
        return mHandler;
    }

    private static class ContainerClientHandler extends Handler {
        private WeakReference<ContainerClientActivity> mActivityRef;
        private String logMsg;

        private ContainerClientHandler(ContainerClientActivity activity) {
            mActivityRef = new WeakReference<>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);

            ContainerClientActivity activity = mActivityRef.get();
            if (activity == null) return;

            switch (msg.what) {
                case MSG_ID_ATTRIBUTE_RECEIVED:
                    logMsg = "-- ATTRIBUTE_RECEIVED\n";
                    logMsg += msg.obj.toString();
                    break;

                case MSG_ID_CACHEDATA_RECEIVED:
                    logMsg = "-- CACHEDATA_RECEIVED\n";
                    logMsg += msg.obj.toString();
                    break;
            }
            activity.printLog(logMsg);
        }
    }
}