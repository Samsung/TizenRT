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

package org.iotivity.service.sample.androidbundle;

import android.content.Context;
import android.util.Log;

import org.iotivity.service.resourcecontainer.BundleActivator;
import org.iotivity.service.resourcecontainer.BundleResource;
import org.iotivity.service.resourcecontainer.RcsResourceContainerBundleAPI;
import org.iotivity.service.resourcecontainer.ResourceConfig;
import org.iotivity.service.sample.androidbundle.resources.DiscomfortIndexResource;
import org.iotivity.service.sample.androidbundle.resources.GyroscopeResource;
import org.iotivity.service.sample.androidbundle.resources.HumidityResource;
import org.iotivity.service.sample.androidbundle.resources.LightIntensityResource;
import org.iotivity.service.sample.androidbundle.resources.TemperatureResource;

import java.util.List;
import java.util.Vector;

public class SampleActivator extends BundleActivator {
    private List<BundleResource> resources = new Vector<BundleResource>();

    public SampleActivator(RcsResourceContainerBundleAPI bundleAPI, Context appContext){
        super(bundleAPI, appContext);
        Log.d(SampleActivator.class.getName(), "Created activator instance " + bundleAPI
                + "," + appContext);
    }

    @Override
    public void activateBundle() {
        Log.d(SampleActivator.class.getName(), "Activate bundle called");

        Log.d(SampleActivator.class.getName(), "requesting configured bundle resources");

        Log.d(SampleActivator.class.getName(), "Bundle API: " + this.bundleAPI);
        if(this.bundleAPI != null)
        {
            List<ResourceConfig> configuredBundleResources = this.bundleAPI.
                    getConfiguredBundleResources("oic.android.sample");

            if(configuredBundleResources !=null) {
                Log.d(SampleActivator.class.getName(), "configured bundle resources: " +
                        configuredBundleResources.size());

                for (ResourceConfig config : configuredBundleResources) {
                    BundleResource resource = null;
                    Log.d(SampleActivator.class.getName(), "Creating " +
                        config.getResourceType());

                    if ("oic.r.lightsensor".equals(config.getResourceType())) {
                        resource =
                                new LightIntensityResource(this.appContext);
                    } else if ("oic.r.temperature".equals(config.getResourceType())) {
                        resource =
                                new TemperatureResource(this.appContext);

                    } else if ("oic.r.gyroscope".equals(config.getResourceType())) {
                        resource =
                                new GyroscopeResource(this.appContext);
                    } else if ("oic.r.discomfortindex".equals(config.getResourceType())) {
                        resource =
                                new DiscomfortIndexResource(this.appContext);
                    } else if ("oic.r.humidity".equals(config.getResourceType())) {
                        resource =
                                new HumidityResource(this.appContext);
                    }

                    if (resource != null) {
                        resource.setURI(config.getURI());
                        resource.setName(config.getName());
                        bundleAPI.registerResource("oic.android.sample", resource);
                        resources.add(resource);
                        Log.d(SampleActivator.class.getName(), "Registering resource " +
                                config.getURI());
                    }
                }
                Log.d(SampleActivator.class.getName(), "Activate bundle finished");
            }
            else{
                Log.d(SampleActivator.class.getName(), "configured bundle resources is null");
            }
        }
        else{
            Log.d(SampleActivator.class.getName(), "Bundle API is null");
        }


    }

    @Override
    public void deactivateBundle() {
        Log.d(SampleActivator.class.getName(), "De-activate bundle called.");
        for(BundleResource resource : resources){
            bundleAPI.unregisterResource(resource);
        }
    }

    @Override
    public void createResource(ResourceConfig config) {
        BundleResource resource = null;

        if("oic.r.lightsensor".equals(config.getResourceType())){
            resource =
                    new LightIntensityResource(this.appContext);
        } else if("oic.r.temperature".equals(config.getResourceType())){
            resource =
                    new TemperatureResource(this.appContext);
        } else if("oic.r.discomfortindex".equals(config.getResourceType())){
            resource =
                    new DiscomfortIndexResource(this.appContext);
        } else if("oic.r.gyroscope".equals(config.getResourceType())){
            resource =
                    new GyroscopeResource(this.appContext);
        } else if ("oic.r.humidity".equals(config.getResourceType())) {
            resource =
                    new HumidityResource(this.appContext);
        }

        if(resource != null) {
            resource.setURI(config.getURI());
            resource.setName(config.getName());
            bundleAPI.registerResource("oic.android.sample", resource);
            resources.add(resource);
        }
    }

    @Override
    public void destroyResource(BundleResource androidBundleResource) {
        Log.d(SampleActivator.class.getName(), "Destroy resource called.");
        bundleAPI.unregisterResource(androidBundleResource);
    }
}
