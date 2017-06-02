/**
 * ***************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ****************************************************************
 */

package org.iotivity.service.easysetup.mediator;

import android.util.Log;

import org.iotivity.service.easysetup.mediator.enums.OAUTH_TOKENTYPE;

import org.iotivity.base.OcException;
import org.iotivity.base.OcRepresentation;

/**
 * This class contains cloud server properties to be delivered to Enrollee
 */
public class CloudProp {
    private static final String TAG = CloudProp.class.getName();
    protected OcRepresentation mRep = null;
    protected String mCloudID = null;
    protected int mCredID ;

    /**
     * Constructor
     */
    public CloudProp() {
        mRep = new OcRepresentation();
        mCloudID = "";
    }

    public void setCloudProp(String authCode, String authProvider, String ciServer)
    {
        if(authCode == null)
        {
            authCode = "";
        }
        if(authProvider == null)
        {
            authProvider = "";
        }
        if(ciServer == null)
        {
            ciServer = "";
        }
        try {
            mRep.setValue(ESConstants.OC_RSRVD_ES_AUTHCODE, authCode);
            mRep.setValue(ESConstants.OC_RSRVD_ES_AUTHPROVIDER, authProvider);
            mRep.setValue(ESConstants.OC_RSRVD_ES_CISERVER, ciServer);
        } catch (OcException e) {
            Log.e(TAG, "setCloudProp is failed.");
        }
    }

    public void setCloudPropWithAccessToken(String accessToken, OAUTH_TOKENTYPE tokenType,
                                        String authProvider, String ciServer)
    {
        if(accessToken == null)
        {
            accessToken = "";
        }
        if(authProvider == null)
        {
            authProvider = "";
        }
        if(ciServer == null)
        {
            ciServer = "";
        }
        try {
            mRep.setValue(ESConstants.OC_RSRVD_ES_ACCESSTOKEN, accessToken);
            mRep.setValue(ESConstants.OC_RSRVD_ES_ACCESSTOKEN_TYPE, tokenType.getValue());
            mRep.setValue(ESConstants.OC_RSRVD_ES_AUTHPROVIDER, authProvider);
            mRep.setValue(ESConstants.OC_RSRVD_ES_CISERVER, ciServer);
        } catch (OcException e) {
            Log.e(TAG, "setCloudPropWithAccessToken is failed.");
        }
    }

    public void setCloudID(String cloudID)
    {
        mCloudID = cloudID;
    }

    public void setCredID(int credID)
    {
        mCredID = credID;
    }
    /**
     * This method returns the authCode used for the first registration to IoTivity cloud
     * @return AuthCode for sign-up to IoTivity cloud
     */
    public String getAuthCode()
    {
        if(mRep == null)
        {
            return null;
        }

        try
        {
            if (mRep.hasAttribute(ESConstants.OC_RSRVD_ES_AUTHCODE))
                return mRep.getValue(ESConstants.OC_RSRVD_ES_AUTHCODE);
        }
        catch (OcException e)
        {
            Log.e(TAG, "getAuthCode is failed.");
        }
        return null;
    }

    /**
     * This method returns the auth provider which issued the given AuthCode
     * @return Auth provider which issued the given AuthCode
     */
    public String getAuthProvider()
    {
        if(mRep == null)
        {
            return null;
        }

        try
        {
            if (mRep.hasAttribute(ESConstants.OC_RSRVD_ES_AUTHPROVIDER))
                return mRep.getValue(ESConstants.OC_RSRVD_ES_AUTHPROVIDER);
        }
        catch (OcException e)
        {
            Log.e(TAG, "getAuthProvider is failed.");
        }
        return null;
    }

	/**
     * This method returns the Cloud Interface server's URL to be registered
     * @return CI server's URL to be registered
     */
    public String getCiServer()
    {
        if(mRep == null)
        {
            return null;
        }

        try
        {
            if (mRep.hasAttribute(ESConstants.OC_RSRVD_ES_CISERVER))
                return mRep.getValue(ESConstants.OC_RSRVD_ES_CISERVER);
        }
        catch (OcException e)
        {
            Log.e(TAG, "getCiServer is failed.");
        }
        return null;
    }

    /**
     * This method returns the Cloud Interface server's UUID
     * @return CI server's UUID
     */
    public String getCloudID()
    {
        return mCloudID;
    }

    /**
     * This method returns the Cloud Certificate's Cred ID
     * @return CI server's Credential ID of Certificate
     */
    public int getCredID()
    {
        return mCredID;
    }

    /**
     * This method returns an accessToken used for the first registration to IoTivity cloud
     * @return accessToken for sign-up to IoTivity cloud
     */
    public String getAccessToken()
    {
        if(mRep == null)
        {
            return null;
        }

        try
        {
            if (mRep.hasAttribute(ESConstants.OC_RSRVD_ES_ACCESSTOKEN))
                return mRep.getValue(ESConstants.OC_RSRVD_ES_ACCESSTOKEN);
        }
        catch (OcException e)
        {
            Log.e(TAG, "getAccessToken is failed.");
        }
        return null;
    }

    /**
     * This method returns an access token type
     * @return tokenType of access token
     */
    public OAUTH_TOKENTYPE getAccessTokenType()
    {
        if(mRep == null)
        {
            return null;
        }

        try
        {
            if (mRep.hasAttribute(ESConstants.OC_RSRVD_ES_ACCESSTOKEN_TYPE))
                return OAUTH_TOKENTYPE.fromInt((int)mRep.getValue(ESConstants.OC_RSRVD_ES_ACCESSTOKEN_TYPE));
        }
        catch (OcException e)
        {
            Log.e(TAG, "getAccessTokenType is failed.");
        }
        return OAUTH_TOKENTYPE.NONE_OAUTH_TOKENTYPE;
    }

    public OcRepresentation toOCRepresentation()
    {
        return mRep;
    }
}
