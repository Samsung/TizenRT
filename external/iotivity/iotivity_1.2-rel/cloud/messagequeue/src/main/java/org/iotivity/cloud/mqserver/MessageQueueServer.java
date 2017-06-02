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
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.mqserver.resources.MQBrokerResource;
import org.iotivity.cloud.util.Log;

public class MessageQueueServer {

    public static void main(String[] args) throws Exception {
        Log.Init();

        System.out.println("-----MQ SERVER-----");

        if (args.length != 6) {
            Log.e("coap server port, Kafka_zookeeper_Address port"
                    + " and Kafka_broker_Address Port and TLS mode required\n"
                    + "ex) 5686 127.0.0.1 2181 127.0.0.1 9092 0\n");

            return;
        }

        ServerSystem serverSystem = new ServerSystem();

        MQBrokerResource MQBroker = new MQBrokerResource();

        String kafka_zookeeper = args[1] + ":" + args[2];
        String kafka_broker = args[3] + ":" + args[4];
        MQBroker.setKafkaInformation(kafka_zookeeper, kafka_broker);

        serverSystem.addResource(MQBroker);

        serverSystem.addServer(new CoapServer(
                new InetSocketAddress(Integer.parseInt(args[0]))));

        boolean tlsMode = Integer.parseInt(args[5]) == 1;

        serverSystem.startSystem(tlsMode);

        Scanner in = new Scanner(System.in, "UTF-8");

        System.out.println("press 'q' to terminate");

        while (!in.nextLine().equals("q"));

        in.close();

        System.out.println("Terminating...");

        serverSystem.stopSystem();

        System.out.println("Terminated");
    }
}