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
import org.iotivity.cloud.base.resource.CloudPingResource;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.rdserver.db.DBManager;
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

    private static int     coapServerPort;
    private static boolean tlsMode;
    private static String  databaseHost;
    private static String  webLogHost;

    public static void main(String[] args) throws Exception {
        System.out.println("-----RD SERVER-----");
        Log.Init();

        if (!parseConfiguration(args)) {
            Log.e("\nCoAP-server <Port> Database <Address> <Port> TLS-mode <0|1> are required. WebSocketLog-Server <Addres> <Port> is optional.\n"
                    + "ex) " + Constants.DEFAULT_COAP_PORT
                    + " 127.0.0.1 27017 0\n");
            return;
        }
        if (webLogHost != null)
            Log.InitWebLog(webLogHost,
                    ResourceDirectoryServer.class.getSimpleName().toString());

        DBManager.createInstance(databaseHost);

        ServerSystem serverSystem = new ServerSystem();

        serverSystem.addResource(new CloudPingResource());
        serverSystem.addResource(new ResourceDirectoryResource());
        serverSystem.addResource(new DiscoveryResource());
        serverSystem.addResource(new DevicePresenceResource());
        serverSystem.addResource(new ResPresenceResource());

        serverSystem.addServer(
                new CoapServer(new InetSocketAddress(coapServerPort)));

        serverSystem.startSystem(tlsMode);

        Scanner in = new Scanner(System.in);

        System.out.println("press 'q' to terminate");

        while (!in.nextLine().equals("q"));

        in.close();

        System.out.println("Terminating...");

        serverSystem.stopSystem();

        System.out.println("Terminated");
    }

    private static boolean parseConfiguration(String[] args) {
        // configuration provided by arguments
        if (args.length == 4 || args.length == 6) {
            coapServerPort = Integer.parseInt(args[0]);
            databaseHost = args[1] + ":" + args[2];
            tlsMode = Integer.parseInt(args[3]) == 1;
            if (args.length == 6)
                webLogHost = args[4] + ":" + args[5];
            return true;
        }
        // configuration provided by docker env
        String tlsModeEnv = System.getenv("TLS_MODE");
        if (tlsModeEnv != null) {
            coapServerPort = Constants.DEFAULT_COAP_PORT;
            databaseHost = System.getenv("MONGODB_ADDRESS") + ":"
                    + System.getenv("MONGODB_PORT");
            tlsMode = Integer.parseInt(tlsModeEnv) == 1;
            return true;
        }
        return false;
    }
}
