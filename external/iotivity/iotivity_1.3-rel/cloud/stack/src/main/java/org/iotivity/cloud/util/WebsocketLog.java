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

package org.iotivity.cloud.util;

import java.io.IOException;
import java.net.URI;

import javax.websocket.ClientEndpoint;
import javax.websocket.CloseReason;
import javax.websocket.ContainerProvider;
import javax.websocket.DeploymentException;
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.WebSocketContainer;

@ClientEndpoint
public class WebsocketLog extends Thread {
    private Session            userSession = null;
    private MessageHandler     messageHandler;
    private WebSocketContainer container;
    private URI                endpointURI;

    public WebsocketLog(URI endpointURI)
            throws DeploymentException, IOException {
        container = ContainerProvider.getWebSocketContainer();
        this.endpointURI = endpointURI;
    }

    @Override
    public void run() {
        startConnect();
    }

    @OnOpen
    public void onOpen(Session userSession) {
        this.userSession = userSession;
    }

    @OnClose
    public void onClose(Session userSession, CloseReason reason)
            throws InterruptedException {
        this.userSession = null;
        Log.d("Disconnected to websocket for Web Log");
        startConnect();
    }

    @OnMessage
    public void onMessage(String message) {
        if (this.messageHandler != null)
            this.messageHandler.handleMessage(message);
    }

    public void addMessageHandler(MessageHandler msgHandler) {
        this.messageHandler = msgHandler;
    }

    public void sendMessage(String message) {
        this.userSession.getAsyncRemote().sendText(message);
    }

    public static interface MessageHandler {
        public void handleMessage(String message);
    }

    public Session getUserSession() {
        return userSession;
    }

    public void startConnect() {
        int retryCount = 0;
        do {
            if (retryCount != 0) {
                Log.d("retry to Connecting... " + retryCount);
            }

            try {
                container.connectToServer(this, endpointURI);
                Log.d("Success to websocket for Web Log");
            } catch (DeploymentException | IOException e) {
                Log.d("Skip Connect to websocket for Web Log");
            }
            retryCount++;
        } while (userSession == null && retryCount < 4);
    }
}
