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
package org.iotivity.cloud.base.device;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;

import org.iotivity.cloud.base.connector.CoapClient;
import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.exception.ClientException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.coap.CoapRequest;
import org.iotivity.cloud.base.protocols.coap.CoapResponse;
import org.iotivity.cloud.base.protocols.enums.Observe;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Bytes;
import org.iotivity.cloud.util.Log;

import io.netty.channel.ChannelHandlerContext;

public class CoapDevice extends Device {
    private CoapClient                 mCoapClient         = null;
    private String                     mUserId             = null;
    private String                     mDeviceId           = null;
    private String                     mAccessToken        = null;
    private Date                       mIssuedTime         = null;
    private int                        mExpiredPolicy      = 0;
    private ArrayList<IRequestChannel> mObserveChannelList = new ArrayList<>();
    private HashMap<Long, IRequest>    mObserveRequestList = new HashMap<>();

    private static final int           INFINITE_TIME       = -1;

    public CoapDevice(ChannelHandlerContext ctx) {
        super(ctx);
    }

    public void updateDevice(String did, String uid, String accesstoken) {
        mDeviceId = did;
        mUserId = uid;
        mAccessToken = accesstoken;
    }

    @Override
    public String getDeviceId() {
        return mDeviceId;
    }

    @Override
    public String getUserId() {
        return mUserId;
    }

    @Override
    public String getAccessToken() {
        return mAccessToken;
    }

    public Date getIssuedTime() {
        return mIssuedTime;
    }

    public int getExpiredPolicy() {
        return mExpiredPolicy;
    }

    public void setExpiredPolicy(int expiredPolicy) {
        mIssuedTime = new Date();
        this.mExpiredPolicy = expiredPolicy;
    }

    public void setUerId(String userId) {
        this.mUserId = userId;
    }

    public void setAccessToken(String accessToken) {
        this.mAccessToken = accessToken;
    }

    public void addObserveChannel(IRequestChannel channel) {
        mObserveChannelList.add(channel);
    }

    public void removeObserveChannel(IRequestChannel channel)
            throws ClientException {
        if (mObserveChannelList.contains(channel)) {

            Iterator<Long> iterator = mObserveRequestList.keySet().iterator();
            while (iterator.hasNext()) {
                Long token = iterator.next();
                CoapClient coapClient = (CoapClient) channel;
                if (coapClient.isObserveRequest(token) != null) {
                    coapClient.removeObserve(token);
                    CoapRequest coapRequest = (CoapRequest) mObserveRequestList
                            .get(token);
                    coapRequest.setObserve(Observe.UNSUBSCRIBE);
                    IResponse response = MessageBuilder.createResponse(
                            coapRequest, ResponseStatus.CONTENT, null, null);
                    sendResponse(response);
                }
            }
            mObserveChannelList.remove(channel);
        }
    }

    public void addObserveRequest(Long token, IRequest request) {

        mObserveRequestList.put(token, request);
    }

    public void removeObserveRequest(Long token) {

        mObserveRequestList.remove(token);
    }

    // This is called by cloud resource model
    @Override
    public void sendResponse(IResponse response) {
        // This message must converted to CoapResponse
        CoapResponse coapResponse = (CoapResponse) response;

        Iterator<Long> iterator = mObserveRequestList.keySet().iterator();
        while (iterator.hasNext()) {
            Long token = iterator.next();
            Long respToken = Bytes.bytesToLong(coapResponse.getToken());
            if (respToken.equals(token)
                    && coapResponse.getObserve() == Observe.NOTHING) {
                iterator.remove();
            }
        }
        ctx.channel().writeAndFlush(response);
    }

    public IRequestChannel getRequestChannel() {
        if (mCoapClient == null) {
            mCoapClient = new CoapClient(ctx.channel());
        }

        return mCoapClient;
    }

    public boolean isExpiredTime() {

        if (mExpiredPolicy == INFINITE_TIME) {
            return false;
        }

        Date currentTime = new Date();
        long difference = currentTime.getTime() - mIssuedTime.getTime();
        long remainTime = mExpiredPolicy - difference / 1000;

        if (remainTime < 0) {

            Log.w("accessToken is expired..");
            return true;
        }

        return false;
    }

    @Override
    public void onConnected() {
        mObserveChannelList.addAll(ConnectorPool.getConnectionList());
    }

    @Override
    public void onDisconnected() {
        Iterator<Long> iterator = mObserveRequestList.keySet().iterator();
        while (iterator.hasNext()) {
            Long token = iterator.next();
            for (IRequestChannel serverChannel : mObserveChannelList) {
                CoapClient coapClient = (CoapClient) serverChannel;
                if (coapClient != null
                        && coapClient.isObserveRequest(token) != null) {
                    CoapRequest coapRequest = (CoapRequest) mObserveRequestList
                            .get(token);
                    coapRequest.setObserve(Observe.UNSUBSCRIBE);
                    coapRequest.setToken(Bytes.longTo8Bytes(token));
                    serverChannel.sendRequest(MessageBuilder.modifyRequest(
                            coapRequest, null, null, null, null), this);
                }
            }
        }
    }
}
