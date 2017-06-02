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
import java.util.Vector;

/**
 * This class provides APIs for managing the container and bundles in the
 * container. The container provides such an interface to the bundle developer
 * to access the configuration and to register/unregister resources.
 */
public interface RcsResourceContainerBundleAPI{

    public void registerResource(String bundleId, BundleResource resource);

    public void unregisterResource(BundleResource resource);

    public List<ResourceConfig> getConfiguredBundleResources(String bundleId);
}
