/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 */

package oic.simulator.serviceprovider.manager;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import oic.simulator.serviceprovider.model.Resource;
import oic.simulator.serviceprovider.model.SingleResource;

public class Data {
    private Map<String, SingleResource> singleResourceMap;

    public Data() {
        singleResourceMap = new HashMap<String, SingleResource>();
    }

    public synchronized void addResource(Resource resource) {
        if (null == resource) {
            return;
        }
        if (resource instanceof SingleResource) {
            singleResourceMap.put(resource.getResourceURI(),
                    (SingleResource) resource);
        }
    }

    public synchronized void deleteResource(Resource resource) {
        if (null == resource) {
            return;
        }
        if (resource instanceof SingleResource) {
            singleResourceMap.remove(resource.getResourceURI());
        }
    }

    public List<SingleResource> getSingleResources() {
        List<SingleResource> resources;
        synchronized (singleResourceMap) {
            if (singleResourceMap.isEmpty()) {
                return null;
            }
            resources = new ArrayList<SingleResource>();
            Set<String> uriSet = singleResourceMap.keySet();
            Iterator<String> itr = uriSet.iterator();
            while (itr.hasNext()) {
                resources.add(singleResourceMap.get(itr.next()));
            }
        }
        return resources;
    }

    public List<Resource> getResources() {
        if (singleResourceMap.isEmpty()) {
            return null;
        }
        List<Resource> resourceList = new ArrayList<Resource>();
        synchronized (singleResourceMap) {
            Set<String> uriSet = singleResourceMap.keySet();
            Iterator<String> itr = uriSet.iterator();
            while (itr.hasNext()) {
                resourceList.add(singleResourceMap.get(itr.next()));
            }
        }
        return resourceList;
    }

    public synchronized boolean isResourceExist(String resourceURI) {
        if (null == resourceURI || resourceURI.isEmpty()) {
            return false;
        }
        if ((null != singleResourceMap && singleResourceMap
                .containsKey(resourceURI))) {
            return true;
        }
        return false;
    }

    public synchronized boolean isAnyResourceExist() {
        return ((null != singleResourceMap && !singleResourceMap.isEmpty()));
    }

    public void changeResourceURI(Resource resource, String curURI,
            String newURI) {
        if (null == resource || null == curURI || null == newURI) {
            return;
        }
        if (resource instanceof SingleResource) {
            synchronized (singleResourceMap) {
                singleResourceMap.remove(curURI);
                singleResourceMap.put(newURI, (SingleResource) resource);
            }
        }
        resource.setResourceURI(newURI);
    }

    public SingleResource getSingleResourceByURI(String resourceURI) {
        if (null == resourceURI) {
            return null;
        }
        SingleResource res;
        synchronized (singleResourceMap) {
            res = singleResourceMap.get(resourceURI);
        }
        return res;
    }

    public Resource getResourceByURI(String resourceURI) {
        Resource res = getSingleResourceByURI(resourceURI);
        return res;
    }

    public synchronized int getResourceCount() {
        return singleResourceMap.size();
    }
}
