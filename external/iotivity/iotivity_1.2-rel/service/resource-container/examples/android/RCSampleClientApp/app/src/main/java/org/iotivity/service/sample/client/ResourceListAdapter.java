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
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ToggleButton;

import org.iotivity.service.RcsException;

import java.util.List;

public class ResourceListAdapter extends ArrayAdapter<ResourceListItem> {

    private Context mContext;
    private int mLayoutId;
    private List<ResourceListItem> foundResources;

    public ResourceListAdapter(Context context, int layoutId, List<ResourceListItem> foundResources)
    {
        super(context, layoutId, foundResources);

        this.mLayoutId = layoutId;
        this.mContext = context;
        this.foundResources = foundResources;
    }

    @Override
    public View getView(final int position, View convertView, final ViewGroup parent) {
        if (convertView == null)
        {
            LayoutInflater inflater = ((Activity) mContext).getLayoutInflater();
            convertView = inflater.inflate(mLayoutId, parent, false);
        }

        String resourceUri = foundResources.get(position).mResourceUri;

        TextView textViewUri = (TextView) convertView.findViewById(R.id.textView_uri);
        textViewUri.setText(resourceUri);

        textViewUri.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                try {
                    TextView textView = (TextView) ((Activity) mContext).findViewById(R.id.log);
                    textView.setText(Utils.resourceInfo(foundResources.get(position).mResource));
                } catch (RcsException e) {
                    e.printStackTrace();
                }
            }
        });

        Button buttonGet = (Button) convertView.findViewById(R.id.button_get);
        buttonGet.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                ResourceListItem selectedResource = foundResources.get(position);
                try {
                    TextView textView = (TextView) ((Activity) mContext).findViewById(R.id.log);
                    textView.setText("Get Remote Attributes from \'" + selectedResource.mResourceUri + "\'");
                    selectedResource.mResource.getRemoteAttributes(selectedResource.mGetListener);
                } catch (RcsException e) {
                    e.printStackTrace();
                }
            }
        });

        ToggleButton buttonObserve = (ToggleButton) convertView.findViewById(R.id.button_observe);
        buttonObserve.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                ResourceListItem selectedResource = foundResources.get(position);
                TextView textView = (TextView) ((Activity) mContext).findViewById(R.id.log);

                try {
                    if (!selectedResource.mCacheStarted) {
                        textView.setText("Start Observe \'" + selectedResource.mResourceUri + "\'");
                        selectedResource.mResource.startCaching(selectedResource.mCacheListener);
                        selectedResource.mCacheStarted = true;
                        ((ToggleButton) view).setChecked(true);
                    } else {
                        textView.setText("Stop Observing \'" + selectedResource.mResourceUri + "\'");
                        selectedResource.mResource.stopCaching();
                        selectedResource.mCacheStarted = false;
                        ((ToggleButton) view).setChecked(false);
                    }
                } catch (RcsException e) {
                    e.printStackTrace();
                }
            }
        });

        return convertView;
    }
}