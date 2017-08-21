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
package org.iotivity.cloud.base.connector;

import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

import org.iotivity.cloud.base.device.IRequestChannel;

public class ConnectorPool {

    static ConcurrentHashMap<String, IRequestChannel> mConnection = new ConcurrentHashMap<>();

    static CoapConnector                    mConnector  = new CoapConnector();

    public ConnectorPool() {

    }

    public static void requestConnection(String connectionName, InetSocketAddress inetAddr,
         boolean tlsMode, boolean keepAlive) throws InterruptedException {
        mConnector.connect(connectionName, inetAddr, tlsMode, keepAlive);
    }

    public static IRequestChannel getConnection(String name) {
        return mConnection.get(name);
    }

    public static ArrayList<IRequestChannel> getConnectionList() {
        return new ArrayList<IRequestChannel>(mConnection.values());
    }

    public static void addConnection(String name, IRequestChannel requestChannel) {
        if (mConnection.containsKey(name))
            mConnection.remove(name);
        mConnection.put(name, requestChannel);
    }
}
