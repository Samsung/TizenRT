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

import java.util.Date;

import org.iotivity.cloud.base.protocols.IResponse;

import io.netty.channel.ChannelHandlerContext;

public class HttpDevice extends Device {

    private String mUserId         = null;
    private String mDeviceId       = null;
    private String mAccessToken    = null;

    private Date   mIssuedTime     = null;
    private int    mExpiredTime    = 0; //in second

    /*
     * Timestamp(milliseconds) =
     * System.currentTimeMillis()+Max-Age(seconds)*1000;
     * 
     * The default session timeout value
     * without setSessionExpiredTime() = 1 Day
     * In other words, Max-Age is not specified in the set-cookie.
     */
    private long   mSessionExpiredTime
    = System.currentTimeMillis() + (86400L * 1000L);

    public HttpDevice(ChannelHandlerContext ctx) {
        super(ctx);
    }

    public void replaceCtx(ChannelHandlerContext ctx) {
        super.ctx = ctx;
    }

    public void updateDevice(String did, String uid, String accesstoken) {
        mDeviceId = did;
        mUserId = uid;
        mAccessToken = accesstoken;
    }

    public void setSessionExpiredTime(long sessionTimeout) {
        long sessionExpiredTime = System.currentTimeMillis()
                + (sessionTimeout * 1000);
        mSessionExpiredTime = sessionExpiredTime;
    }

    public long getSessionExpiredTime() {
        return mSessionExpiredTime;
    }

    public void setExpiredTime(int expiredTime) {
        mIssuedTime = new Date();
        this.mExpiredTime = expiredTime;
    }

    public Date getIssuedTime() {
        return mIssuedTime;
    }

    public int getExpiredTime() {
        return mExpiredTime;
    }

    @Override
    public void sendResponse(IResponse response) {
        /*
         * If the response is CoAP response (actually always for now),
         * then it will be translated into HTTP response
         * by the next handler: HCProxyHandler.
         */
        ctx.channel().writeAndFlush(response);
    }

    @Override
    public void onConnected() {
        /* 
         * This function is used for Observe logic in CoAP message
         * to remember Observe channel and request info.
         * In HTTP protocol support, there is no Observe logic
         * so this function is not in use.
         */
    }

    @Override
    public void onDisconnected() {
        /* 
         * This function is used for Observe logic in CoAP message
         * to automatically send Observe.UNSUBSCRIBE request.
         * In HTTP protocol support, there is no Observe logic
         * so this function is not in use.
         */
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
}
