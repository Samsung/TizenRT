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
package org.iotivity.service.resourcecontainer;

import android.content.Context;
import java.util.List;

/**
 * Every resource bundle has to provide a bundle activator that can be called
 * by the resource container on bundle startup.
 */
public abstract class BundleActivator {
    protected RcsResourceContainerBundleAPI bundleAPI;
    protected Context appContext;
    
    public BundleActivator(RcsResourceContainerBundleAPI bundleAPI, Context appContext){
        this.bundleAPI = bundleAPI;
        this.appContext = appContext;
    }
    /**
     * Activates the bundle and creates all resources.
     */
    public abstract void activateBundle();

    /**
     * Deactivates the bundle and destroys all resources.
     */
    public abstract void deactivateBundle();

    /**
     * Creates a resources
     * @param resource Instance of a BundleResource
     */
    public abstract void createResource(ResourceConfig resource);

    /**
     * Destroys a resource
     * @param resource Instance of a BundleResource
     */
    public abstract void destroyResource(BundleResource resource);

}