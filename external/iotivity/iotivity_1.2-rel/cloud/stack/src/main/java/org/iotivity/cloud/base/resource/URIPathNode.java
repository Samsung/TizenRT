/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
package org.iotivity.cloud.base.resource;

import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.base.device.IRequestEventHandler;

public class URIPathNode {

    private HashMap<String, URIPathNode> mChildren       = new HashMap<>();
    private IRequestEventHandler         mRequestHandler = null;

    public URIPathNode addHandler(List<String> pathSegments,
            IRequestEventHandler requestHandler) {
        URIPathNode edgeNode = this;
        for (String pathSegment : pathSegments) {
            URIPathNode node = edgeNode.mChildren.get(pathSegment);
            if (node == null) {
                node = new URIPathNode();
                edgeNode.mChildren.put(pathSegment, node);
            }
            edgeNode = node;
        }
        edgeNode.mRequestHandler = requestHandler;
        return edgeNode;
    }

    // if searched return last node or last found middle node
    public IRequestEventHandler findPathHandler(List<String> pathSegments) {
        URIPathNode edgeNode = this;
        URIPathNode childNode = null;
        for (String pathSegment : pathSegments) {
            childNode = edgeNode.mChildren.get(pathSegment);

            if (childNode == null)
                return edgeNode.mRequestHandler;

            edgeNode = childNode;
        }

        return edgeNode.mRequestHandler;
    }
}
