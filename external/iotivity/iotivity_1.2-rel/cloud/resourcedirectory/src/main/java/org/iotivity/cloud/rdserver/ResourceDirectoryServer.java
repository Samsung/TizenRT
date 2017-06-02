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
package org.iotivity.cloud.rdserver;

import java.net.InetSocketAddress;
import java.util.Scanner;

import org.iotivity.cloud.base.ServerSystem;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.rdserver.resources.directory.rd.ResourceDirectoryResource;
import org.iotivity.cloud.rdserver.resources.directory.res.DiscoveryResource;
import org.iotivity.cloud.rdserver.resources.presence.device.DevicePresenceResource;
import org.iotivity.cloud.rdserver.resources.presence.resource.ResPresenceResource;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class is in charge of running of resource directory server.
 *
 */
public class ResourceDirectoryServer {

    public static void main(String[] args) throws Exception {
        Log.Init();

        System.out.println("-----RD SERVER-----");

        if (args.length != 2) {
            Log.e("coap server port and TLS mode required\n" + "ex) 5684 0\n");
            return;
        }

        ServerSystem serverSystem = new ServerSystem();

        serverSystem.addResource(new ResourceDirectoryResource());
        serverSystem.addResource(new DiscoveryResource());
        serverSystem.addResource(new DevicePresenceResource());
        serverSystem.addResource(new ResPresenceResource());

        serverSystem.addServer(new CoapServer(
                new InetSocketAddress(Integer.parseInt(args[0]))));

        boolean tlsMode = Integer.parseInt(args[1]) == 1;

        serverSystem.startSystem(tlsMode);

        Scanner in = new Scanner(System.in);

        System.out.println("press 'q' to terminate");

        while (!in.nextLine().equals("q"));

        in.close();

        System.out.println("Terminating...");

        serverSystem.stopSystem();

        System.out.println("Terminated");
    }
}
