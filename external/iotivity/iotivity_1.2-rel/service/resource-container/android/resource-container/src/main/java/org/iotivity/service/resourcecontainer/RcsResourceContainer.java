//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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


/**
 * @file
 * This file contains the Resource Container APIs
 */
package org.iotivity.service.resourcecontainer;

import java.util.List;

import java.util.Map;
import java.util.Enumeration;
import android.util.Log;
import android.content.Context;
import java.util.Vector;

import dalvik.system.DexFile;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import dalvik.system.PathClassLoader;
import java.net.URLClassLoader;

import java.util.Hashtable;
import java.io.File;
import java.net.URL;

import java.lang.reflect.InvocationTargetException;

/**
 * This class provides APIs for managing the container and bundles in the
 * container.
 */
public class RcsResourceContainer implements RcsResourceContainerBundleAPI {

    private static final String TAG = RcsResourceContainer.class.getSimpleName();

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("oc_logger");
        System.loadLibrary("connectivity_abstraction");
        System.loadLibrary("ca-interface");
        System.loadLibrary("octbstack");
        System.loadLibrary("oc");
        System.loadLibrary("rcs_client");
        System.loadLibrary("rcs_server");
        System.loadLibrary("rcs_common");
        System.loadLibrary("rcs_container");
        System.loadLibrary("resource_container_jni");
    }

    private Context appContext;

    private native void nativeStartContainer(String configFile);

    private native void nativeStopContainer();

    private native void nativeAddBundle(String bundleId, String bundleUri,
            String bundlePath, String activator, Map<String, String> params);

    private native void nativeRemoveBundle(String bundleId);

    private native List<RcsBundleInfo> nativeListBundles();

    private native void nativeStartBundle(String bundleId);

    private native void nativeStopBundle(String bundleId);

    private native void nativeAddResourceConfig(String bundleId,
            String resourceUri, Map<String, String> params);

    private native void nativeRemoveResourceConfig(String bundleId,
            String resourceUri);

    private native List<String> nativeListBundleResources(String bundleId);
    
    private native void nativeRegisterBundleResource(BundleResource resource,
        String[] attributes, String bundleId, String uri,
        String resourceType, String name);
    
    private native void nativeUnregisterBundleResource(BundleResource resource,
        String uri);
    
    private native int nativeGetNumberOfConfiguredResources(String bundleId);
        
    private native String[] nativeGetConfiguredResourceParams(String bundleId,
        int resId);  
    
    public RcsResourceContainer(Context appContext){
        this.appContext = appContext;
    }
    
    private Hashtable<String, BundleActivator> activators = new Hashtable<String, BundleActivator>();

    /**
     * API for starting the Container
     *
     * <p>
     * This API start the container with the provided Configuration file.
     *
     * @param configFile
     *            configuration File that contains the Bundle/Bundles
     *            information.
     *
     */
    public List<RcsBundleInfo> startContainer(String configFile) {
        if(configFile == null || configFile.isEmpty()){
            throw new IllegalArgumentException(
		"Configuration file is null or empty.");
	}
        nativeStartContainer(configFile);
        Log.d(TAG, "startContainer in Java");
        List<RcsBundleInfo> bundles = listBundles();
        Log.d(TAG, "startContainer. There are " + bundles.size() + " bundles.");
        for(RcsBundleInfo bundleInfo : bundles){
            Log.d(TAG, "bundle-id: " + bundleInfo.getID() + ", " + bundleInfo.getPath());
            if(bundleInfo.getPath().endsWith(".apk")){ // load classes from standalone application
                startBundleFromStandaloneApp(bundleInfo);
            }else if(bundleInfo.getPath().endsWith(".jar")){ // load classes from library
                startBundleFromJar(bundleInfo);
            }
        }
        return bundles;
    }

    private void startBundleFromStandaloneApp(RcsBundleInfo bundleInfo){
        if(bundleInfo == null){
            throw new IllegalArgumentException(
		"bundleInfo parameter is null or empty.");
        }
        String packageName = bundleInfo.getPath().replace(".apk", "");
        try{
            PackageManager packageManager = appContext.getPackageManager();
            ApplicationInfo appInfo = packageManager.getApplicationInfo(packageName, 0);
            DexFile df = new DexFile(appInfo.sourceDir);
            ClassLoader cl = appContext.getClassLoader();
            for (Enumeration<String> iter = df.entries(); iter.hasMoreElements(); ) {
                String classN = iter.nextElement();
                if (classN.contains(packageName)) {
                    Log.d(TAG,"Class: " + classN);
                    df.loadClass(classN, cl);
                }
            }
            String className = bundleInfo.getActivatorName();
            Log.d(TAG, "Loading activator: " + className);
            Class activatorClass = df.loadClass(className, cl);
            activateBundle(activatorClass, bundleInfo);
        }
        catch(Exception e){
            Log.e(TAG, e.getMessage(), e);
        }
        Log.d(TAG, "Have to register android bundle");
    }
    
    private void startBundleFromJar(RcsBundleInfo bundleInfo){
        if(bundleInfo == null){
            throw new IllegalArgumentException(
		"bundleInfo parameter is null");
        }
        try{
            Log.e(TAG, "Loading from .jar file.");
            
            PathClassLoader classLoader = new PathClassLoader(bundleInfo.getPath(),
                    RcsResourceContainer.class.getClassLoader());
           
            String className = bundleInfo.getActivatorName().replace('/', '.');
            Log.d(TAG, "Loading activator: " + className);
            Class activatorClass = Class.forName(className, true, classLoader);

            activateBundle(activatorClass, bundleInfo);
        }
        catch(Exception e){
            Log.e(TAG, e.getMessage(), e);
        }
        Log.d(TAG, "Have to register android bundle");
    }
    
    private void activateBundle(Class activatorClass, RcsBundleInfo bundleInfo) throws
        NoSuchMethodException, InstantiationException, IllegalAccessException, 
        InvocationTargetException{
        if(activatorClass == null){
            throw new IllegalArgumentException(
		"activatorClass is null.");
        }
        if(bundleInfo == null){
            throw new IllegalArgumentException(
		"bundleInfo is null.");
        }
        if(activatorClass!= null){
            BundleActivator activator = (BundleActivator) activatorClass.
                    getConstructor(RcsResourceContainerBundleAPI.class, Context.class).
                    newInstance(this, appContext);
            activator.activateBundle();
            activators.put(bundleInfo.getID(), activator);
            bundleInfo.setActivated(true);
        }else{
            Log.e(TAG, "Activator is null.");
        }
    }

    /**
     * API for stopping the Container
     */
    public void stopContainer() {
        // stop all android bundles
        for(BundleActivator activator :activators.values()){
            activator.deactivateBundle();
        }
        nativeStopContainer();
    }

    /**
     * API for getting the list of all bundles in the container
     *
     * @return list<RCSBundleInfo> -List of BundleInfo objects each associated
     *         with a bundle
     *
     *         {@link RcsBundleInfo}
     */
    public List<RcsBundleInfo> listBundles() {
        return nativeListBundles();
    }

    /**
     * API for adding the bundle to the Container
     *
     * @param bundleId
     *            Id of the Bundle
     * @param bundleUri
     *            Uri of the bundle
     * @param bundlePath
     *            Path of the bundle
     * @param activator
     *            Activation prefix for .so bundles, or activator class name for
     *            .jar bundles
     * @param params
     *            key-value pairs in string form for other Bundle parameters
     *
     *            <p>
     *            It is dynamic configuration
     */
    public void addBundle(String bundleId, String bundleUri, String bundlePath,
            String activator, Map<String, String> params) {
        if(bundleId == null){
            throw new IllegalArgumentException(
		"bundleId parameter is null.");
        }
	if(bundleUri == null){
            throw new IllegalArgumentException(
		"bundleUri is null.");
        }
	if(bundlePath == null){
            throw new IllegalArgumentException(
		"bundlePath is null.");
        }
	if(activator == null){
            throw new IllegalArgumentException(
		"activator is null.");
        }
        nativeAddBundle(bundleId, bundleUri, bundlePath, activator, params);
    }

    /**
     * API for removing the bundle from the container
     *
     * @param bundleId
     *            Id of the Bundle
     *
     */
    public void removeBundle(String bundleId) {
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        if(activators.contains(bundleId)){
            // deactivate android bundle
            activators.get(bundleId).deactivateBundle();
        }
        nativeRemoveBundle(bundleId);
    }

    /**
     * API for starting the bundle.
     *
     * @param bundleId
     *            Id of the Bundle
     *
     */
    public void startBundle(String bundleId) {
        Log.d(TAG, "startBundle");
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        List<RcsBundleInfo> bundles = listBundles();

        for(RcsBundleInfo bundleInfo : bundles){
            if(bundleInfo.getID().equals(bundleId) && bundleInfo.getPath().endsWith(".apk")){
                Log.d(TAG, "Have to start android bundle");
                Log.d(TAG, "bundle-id: " + bundleInfo.getID() + ", " + bundleInfo.getPath());
                if(bundleInfo.getPath().endsWith(".apk")){
                    startBundleFromStandaloneApp(bundleInfo);
                }else if(bundleInfo.getID().equals(bundleId) &&
                        bundleInfo.getPath().endsWith(".jar")){ // load classes from library
                    startBundleFromJar(bundleInfo);
                }
            }else{
                nativeStartBundle(bundleId);
            }
        }
    }

    /**
     * API for Stopping the bundle
     *
     * @param bundleId
     *            Id of the Bundle
     *
     */
    public void stopBundle(String bundleId){
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        nativeStopBundle(bundleId);
    }

    /**
     * API for adding the Resource configuration information to the bundle
     *
     * @param bundleId
     *            Id of the Bundle
     * @param resourceUri
     *            URI of the resource
     * @param params
     *            key-value pairs in string form for other Bundle parameters
     *
     */
    public void addResourceConfig(String bundleId, String resourceUri,
            Map<String, String> params) {
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
	if(resourceUri == null || resourceUri.isEmpty()){
            throw new IllegalArgumentException(
		"resourceUri parameter is null or empty.");
        }
        nativeAddResourceConfig(bundleId, resourceUri, params);
    }

    /**
     * API for removing the Resource configuration information from the bundle
     *
     * @param bundleId
     *            Id of the Bundle
     * @param resourceUri
     *            URI of the resource
     *
     */
    public void removeResourceConfig(String bundleId, String resourceUri) {
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
	if(resourceUri == null || resourceUri.isEmpty()){
            throw new IllegalArgumentException(
		"resourceUri parameter is null or empty.");
        }
        nativeRemoveResourceConfig(bundleId, resourceUri);
    }

    /**
     * API for getting the list of Bundle Resources
     *
     * @param bundleId
     *            Id of the Bundle
     *
     * @return List<String> All the bundle resources
     */
    public List<String> listBundleResources(String bundleId) {
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        return nativeListBundleResources(bundleId);
    }

    /**
     * Registers a bundle resource
     *
     * @param bundleId
     *            Id of the Bundle
     * @param resource
     *            resource to be registered
     */
    public void registerResource(String bundleId, BundleResource resource){
        Log.d(TAG, "register Resource");
        // bundleResources.add(resource);
        
        if(bundleId == null || bundleId.isEmpty()){
		throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
	if(resource == null){
		throw new IllegalArgumentException(
		"resource parameter is null.");
        }
        nativeRegisterBundleResource(resource, resource.getAttributeKeys(), bundleId,
                        resource.getURI(), resource.getResourceType(),
                        resource.getName());
    }

    /**
     * Returns the bundle configuration for the resources
     *
     * @param bundleId
     *            Id of the Bundle
     *
     * @return List<ResourceConfig> All the resource configurations for the given bundle
     */
    public List<ResourceConfig> getConfiguredBundleResources(String bundleId) {
        Log.d(TAG, "getConfiguredBundleResource " + bundleId);
        Vector<ResourceConfig> configs = new Vector<ResourceConfig>();
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        int configuredResources = getNumberOfConfiguredResources(bundleId);
        Log.d(TAG, "configured resources " + configuredResources);

        for (int i = 0; i < configuredResources; i++) {
                String[] resourceParams = getConfiguredResourceParams(bundleId, i);
                ResourceConfig config = new ResourceConfig(resourceParams);
                configs.add(config);
        }
        return configs;
    }

    /**
     * Unregisters a bundle resource
     *
     * @param resource
     *            Resource to be unregistered
     */
    public void unregisterResource(BundleResource resource){
        Log.d(TAG, "unregister Resource");
        if(resource == null){
        	throw new IllegalArgumentException(
		"resource is null.");
        }
        nativeUnregisterBundleResource(resource, resource.getURI());
    }

    /**
     * Returns the number of configured resources
     *
     * @param bundleId
     *            Id of the Bundle
     * @return number of configured resources
     */
    public int getNumberOfConfiguredResources(String bundleId){
        Log.d(TAG, "getNumberOfConfiguredResources");
        if(bundleId == null || bundleId.isEmpty()){
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
        return nativeGetNumberOfConfiguredResources(bundleId);
    }

    /**
     * Provides the configured resource parameter
     *
     * @param bundleId
     *            Id of the Bundle
     * @param resId
                  Continuous numeric identifier within the bundle
     * @return resource paramaters such as URI, resource type, name, etc. for the resource
     */
    public String[] getConfiguredResourceParams(String bundleId, int resId){
        Log.d(TAG, "getConfiguredResourceParams");
        if(bundleId == null || bundleId.isEmpty()){ 
            throw new IllegalArgumentException(
		"bundleId parameter is null or empty.");
        }
	if(resId < 0){
	    throw new IllegalArgumentException(
		"resId paramater has to be non-negative.");
	}
        return nativeGetConfiguredResourceParams(bundleId, resId);
    }
}
