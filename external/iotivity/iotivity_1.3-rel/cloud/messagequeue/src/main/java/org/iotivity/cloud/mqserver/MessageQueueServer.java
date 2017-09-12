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
package org.iotivity.cloud.mqserver;

import java.net.InetSocketAddress;
import java.util.Scanner;

import org.iotivity.cloud.base.ServerSystem;
import org.iotivity.cloud.base.resource.CloudPingResource;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.mqserver.resources.MQBrokerResource;
import org.iotivity.cloud.util.Log;

public class MessageQueueServer {

    private static int     coapServerPort;
    private static boolean tlsMode;
    private static String  zookeeperHost;
    private static String  kafkaHost;
    private static String  webLogHost;

    public static void main(String[] args) throws Exception {
        System.out.println("-----MQ SERVER-----");
        Log.Init();

        if (!parseConfiguration(args)) {
            Log.e("\nCoAP-server <Port> Zookeeper <Address> <Port> Kafka <Address> <Port> TLS-mode <0|1> are required. "
                    + "WebSocketLog-Server <Addres> <Port> is optional.\n"
                    + "ex) " + Constants.DEFAULT_COAP_PORT
                    + " 127.0.0.1 2181 127.0.0.1 9092 0\n");
            return;
        }
        if (webLogHost != null)
            Log.InitWebLog(webLogHost,
                    MessageQueueServer.class.getSimpleName().toString());

        ServerSystem serverSystem = new ServerSystem();

        MQBrokerResource MQBroker = new MQBrokerResource();
        MQBroker.setKafkaInformation(zookeeperHost, kafkaHost);

        serverSystem.addResource(MQBroker);
        serverSystem.addResource(new CloudPingResource());

        serverSystem.addServer(
                new CoapServer(new InetSocketAddress(coapServerPort)));

        serverSystem.startSystem(tlsMode);

        Scanner in = new Scanner(System.in, "UTF-8");

        System.out.println("press 'q' to terminate");

        while (!in.nextLine().equals("q"));

        in.close();

        System.out.println("Terminating...");

        serverSystem.stopSystem();

        System.out.println("Terminated");
    }

    private static boolean parseConfiguration(String[] args) {
        // configuration provided by arguments
        if (args.length == 6 || args.length == 8) {
            coapServerPort = Integer.parseInt(args[0]);
            zookeeperHost = args[1] + ":" + args[2];
            kafkaHost = args[3] + ":" + args[4];
            tlsMode = Integer.parseInt(args[5]) == 1;
            if (args.length == 8)
                webLogHost = args[6] + ":" + args[7];
            return true;
        }
        // configuration provided by docker env
        String tlsModeEnv = System.getenv("TLS_MODE");
        if (tlsModeEnv != null) {
            coapServerPort = Constants.DEFAULT_COAP_PORT;
            tlsMode = Integer.parseInt(tlsModeEnv) == 1;
            zookeeperHost = System.getenv("ZOOKEEPER_ADDRESS") + ":"
                    + System.getenv("ZOOKEEPER_PORT");
            kafkaHost = System.getenv("KAFKA_ADDRESS") + ":"
                    + System.getenv("KAFKA_PORT");
            return true;
        }
        return false;
    }
}