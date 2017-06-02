/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

package org.iotivity.service.sample.container;

import android.content.Context;
import android.os.Message;

import android.os.PowerManager;
import android.util.Log;

import org.iotivity.service.resourcecontainer.RcsBundleInfo;
import org.iotivity.service.resourcecontainer.RcsResourceContainer;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * For calling the Resource Container APIs as per user selection on UI and for
 * updating the UI
 *
 * It contains all the Resource Container APIs.
 */
public class ResourceContainer {

    private RcsResourceContainer             containerInstance;
    public static String                     logMessage;

    private static ResourceContainerActivity resourceContainerActivityInstance;
    private static Message                   msg;
    public static boolean                    startBundleFlag;
    private static boolean                   isStarted     = false;
    PowerManager pm = null;
    PowerManager.WakeLock wl = null;

    // constructor
    public ResourceContainer() {
        resourceContainerActivityInstance = ResourceContainerActivity
                .getResourceContainerActivityObj();
        containerInstance = new RcsResourceContainer(
                resourceContainerActivityInstance.getApplicationContext());
        pm = (PowerManager) resourceContainerActivityInstance.getApplicationContext().
                getSystemService(Context.POWER_SERVICE);
    }

    // Start Container
    public void startContainer(String sdCardPath) {

        String configFile = sdCardPath + "/ResourceContainerConfig.xml";
        Log.i("startContainer : config path : ", configFile);

        if (!isStarted) {
            wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK,
                    "ResourceContainer sample");
            wl.acquire();
            containerInstance.startContainer(configFile);
            isStarted = true;
            logMessage = "Container Started ";

            ResourceContainerActivity.setMessageLog(logMessage);
            msg = Message.obtain();
            msg.what = 1;
            resourceContainerActivityInstance.getHandler().sendMessage(msg);

            msg = Message.obtain();
            msg.what = 0;
            resourceContainerActivityInstance.getHandler().sendMessage(msg);

            // initialize the information of the bundles
            // which registered at the starting time of the container
            List<RcsBundleInfo> bundleList = containerInstance.listBundles();
            Iterator<RcsBundleInfo> it = bundleList.iterator();

            while (it.hasNext())
            {
                String id = ((RcsBundleInfo) it.next()).getID();
                if (id.equals(ExampleBundleDescription.DIBundle.mBundleId)) {
                    ExampleBundleDescription.DIBundle.isStarted = true;
                }
                else if (id.equals(ExampleBundleDescription.DIAndroidBundle.mBundleId)) {
                    ExampleBundleDescription.DIAndroidBundle.isStarted = true;
                }
            }
        }
    }

    // Stop Container
    public void stopContainer() {

        if (isStarted) {
            containerInstance.stopContainer();
            logMessage = "Container stopped";
            isStarted = false;
            wl.release();
        } else {
            logMessage = "Container not started";
        }

        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    // List Bundle Resources
    public void listBundleResources(String bundleId) {

        List<String> bundleResources = containerInstance
                .listBundleResources(bundleId);
        Iterator<String> it = bundleResources.iterator();
        logMessage = "";

        if (0 == bundleResources.size()) {
            logMessage = logMessage + "No resource found in the bundle" + "\n";
        } else {
            while (it.hasNext()) {
                String element = (String) it.next();
                logMessage = logMessage + element + "\n";
            }
        }
        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    // List Bundles
    public void listBundles() {

        List<RcsBundleInfo> bundleList = containerInstance.listBundles();
        Iterator<RcsBundleInfo> it = bundleList.iterator();
        int i = 0;
        logMessage = "";
        logMessage = logMessage + "size of bundleList : " + bundleList.size()
                + "\n\n";

        while (it.hasNext()) {
            i++;
            RcsBundleInfo object = (RcsBundleInfo) it.next();
            logMessage += "Bundle : " + i + " -: \n";
            logMessage += "ID : " + object.getID() + "\n";
            logMessage += "Lib Path: " + object.getPath() + "\n";
            if (!(object.getVersion().equalsIgnoreCase("")))
                logMessage += "version : " + object.getVersion() + "\n\n";
            else
                logMessage += "\n";
        }
        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    public boolean bundleExists(String bundleId)
    {
        List<RcsBundleInfo> bundleList = containerInstance.listBundles();
        Iterator<RcsBundleInfo> it = bundleList.iterator();

        while (it.hasNext())
        {
            if (it.next().getID().equals(bundleId))
                return true;
        }

        return false;
    }

    // add Bundles
    public void addBundle(BundleInformation bundle) {
        if (bundleExists(bundle.mBundleId))
            logMessage = "Bundle \'" + bundle.mBundleId + "\' already added" + "\n";

        else {
            containerInstance
                    .addBundle(bundle.mBundleId, bundle.mBundleUri,
                            bundle.mBundlePath, bundle.mActivator, bundle.mBundleParams);

            logMessage = "bundle to add : " + "\n";
            logMessage = logMessage + "ID : " + bundle.mBundleId + "\n";
            logMessage = logMessage + "Uri: " + bundle.mBundleUri + "\n";
            logMessage = logMessage
                    + "Path : "
                    + bundle.mBundlePath
                    + "\n\n";
            logMessage = logMessage + "bundle added successfully" + "\n";
        }

        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    // remove Bundles
    public void removeBundle(BundleInformation bundle) {

        if (!bundleExists(bundle.mBundleId))
            logMessage = "Bundle \'" + bundle.mBundleId + "\' not added" + "\n";

        else {
            containerInstance.removeBundle(bundle.mBundleId);

            bundle.isStarted = false;
            logMessage = "bundle to remove : " + "\n";
            logMessage = logMessage + "ID : " + bundle.mBundleId + "\n\n";
            logMessage = logMessage + " bundle removed  successfully" + "\n";
        }

        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    // start Bundles
    public void startBundle(BundleInformation bundle) {

        if (!bundleExists(bundle.mBundleId)) {
            logMessage = "Bundle \'" + bundle.mBundleId + "\' not added" + "\n";
        } else if (bundle.isStarted) {
            logMessage = "Bundle \'" + bundle.mBundleId + "\' already started" + "\n";
        } else {
            bundle.isStarted = true;
            containerInstance.startBundle(bundle.mBundleId);

            logMessage = " bundle to start" + "\n";
            logMessage += " ID : " + bundle.mBundleId + "\n\n";
            logMessage += " bundle started successfully" + "\n";
        }

        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }

    // Stop Bundles
    public void stopBundle(BundleInformation bundle) {

        if (!bundleExists(bundle.mBundleId)) {
            logMessage = "Bundle \'" + bundle.mBundleId + "\' not added" + "\n";
        }
        else if (!bundle.isStarted) {
            logMessage = "Bundle \'" + bundle.mBundleId + "\' is not Started" + "\n";
        } else {
            containerInstance.stopBundle(bundle.mBundleId);
            bundle.isStarted = false;
            logMessage = " bundle to stop" + "\n";
            logMessage = logMessage + " ID : " + bundle.mBundleId  + "\n\n";
            logMessage = logMessage + " bundle stopped successfully" + "\n";
        }

        ResourceContainerActivity.setMessageLog(logMessage);
        msg = Message.obtain();
        msg.what = 1;
        resourceContainerActivityInstance.getHandler().sendMessage(msg);
    }
}

class ExampleBundleDescription
{
    static final BundleInformation BMIBundle =
            new BundleInformation ("oic.bundle.BMISensor", "",
                    "/data/data/org.iotivity.service.sample.resourcecontainer/files/" +
                            "libBMISensorBundle.so",
                    "bmisensor", new HashMap<String, String>());

    static final BundleInformation DIBundle =
            new BundleInformation ("oic.bundle.discomfortIndexSensor", "",
                    "/data/data/org.iotivity.service.sample.resourcecontainer/files/" +
                            "libDISensorBundle.so",
                    "disensor", new HashMap<String, String>());

    static final BundleInformation DIAndroidBundle =
            new BundleInformation ("oic.android.sample", "",
                    "org.iotivity.service.sample.androidbundle.apk",
                    "org.iotivity.service.sample.androidbundle.SampleActivator",
                    new HashMap<String, String>());
}

class BundleInformation
{
    String mBundleId;
    String mBundleUri;
    String mBundlePath;
    String mActivator;
    Map<String, String> mBundleParams;

    Boolean isStarted;

    public BundleInformation(String id, String uri, String path, String activator, Map<String, String> params)
    {
        mBundleId = id;
        mBundleUri = uri;
        mBundlePath = path;
        mActivator = activator;
        mBundleParams = params;
        isStarted = false;
    }
}