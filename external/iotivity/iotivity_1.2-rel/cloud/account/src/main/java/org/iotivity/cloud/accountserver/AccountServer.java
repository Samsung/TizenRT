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
package org.iotivity.cloud.accountserver;

import java.net.InetSocketAddress;
import java.util.Scanner;

import org.iotivity.cloud.accountserver.resources.account.AccountResource;
import org.iotivity.cloud.accountserver.resources.account.session.SessionResource;
import org.iotivity.cloud.accountserver.resources.account.tokenrefresh.TokenRefreshResource;
import org.iotivity.cloud.accountserver.resources.acl.group.GroupResource;
import org.iotivity.cloud.accountserver.resources.acl.id.AclResource;
import org.iotivity.cloud.accountserver.resources.acl.verify.AclVerifyResource;
import org.iotivity.cloud.accountserver.resources.acl.invite.InviteResource;
import org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateResource;
import org.iotivity.cloud.accountserver.resources.credprov.crl.CrlResource;
import org.iotivity.cloud.base.ServerSystem;
import org.iotivity.cloud.base.server.CoapServer;
import org.iotivity.cloud.util.Log;

/**
 *
 * This class is in charge of running of account server.
 *
 */
public class AccountServer {

    public static void main(String[] args) throws Exception {
        Log.Init();

        System.out.println("-----Account SERVER-----");

        if (args.length != 2) {
            Log.e("coap server port and TLS mode required\n" + "ex) 5685 0\n");
            return;
        }

        ServerSystem serverSystem = new ServerSystem();

        serverSystem.addResource(new AccountResource());

        serverSystem.addResource(new SessionResource());

        serverSystem.addResource(new TokenRefreshResource());

        serverSystem.addResource(new GroupResource());

        serverSystem.addResource(new AclResource());

        serverSystem.addResource(new AclVerifyResource());

        serverSystem.addResource(new CertificateResource());

        serverSystem.addResource(new CrlResource());

        serverSystem.addResource(new AclResource());

        serverSystem.addResource(new InviteResource());

        serverSystem.addServer(new CoapServer(
                new InetSocketAddress(Integer.parseInt(args[0]))));

        boolean tlsMode = Integer.parseInt(args[1]) == 1;

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
