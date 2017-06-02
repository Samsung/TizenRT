/******************************************************************
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 * <p>
 * <p>
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************/

package org.iotivity.service.ns.sample.consumer;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.iotivity.base.ErrorCode;
import org.iotivity.base.OcAccountManager;
import org.iotivity.base.OcConnectivityType;
import org.iotivity.base.OcException;
import org.iotivity.base.OcHeaderOption;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcRepresentation;
import org.iotivity.base.OcResource;
import org.iotivity.service.ns.common.TopicsList;
import org.iotivity.service.ns.common.Topic;

import java.util.Arrays;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends Activity
        implements OcAccountManager.OnPostListener {
    private final String        TAG                 = "NS_MAIN_ACTIVITY";
    private final int           REQUEST_LOGIN       = 1;
    private static final String CI_SERVER_PREFIX    = "coap+tcp://";
    private final Context       context             = this;

    public static String        deviceID            = null;
    public static String        CIServer            = null;
    public static String        RemoteAddress       = null;
    public static String        MQCloudAddress      = null;
    public static String        MQCloudTopic        = null;

    private Button              btnStart;
    private Button              btnStop;
    private Button              btnRescan;
    private Button              btnSubscribe;
    private Button              btnUnsubscribe;
    private Button              btnEnableRemoteService;
    private Button              btnGetTopicList;
    private Button              btnUpdateTopicList;
    private Button              btnClearLog;
    private static TextView     TvLog;
    private Button              signUp, signIn, signOut;
    private Button              subscribeMQ;

    private boolean             isStarted           = false;

    private ConsumerSample      mConsumerSample     = null;
    private OcAccountManager    mAccountManager;
    String                      mAuthCode;
    String                      mAuthProvider;
    String                      mRefreshtoken;
    String                      mUserID;
    String                      mAccessToken;

    private static final int    PROVIDER_DISCOVERED = 1;
    private static final int    STATE_CHANGED       = 2;
    private static final int    MESSAGE_RECEIVED    = 3;
    private static final int    SYNCINFO_RECEIVED   = 4;
    private static final int    TOPICS_RECEIVED     = 5;

    public static Handler       mHandler            = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case PROVIDER_DISCOVERED: {
                    String providerId = (String) msg.obj;
                    if (providerId != null) {
                        TvLog.append(providerId + "\n");
                    }
                    break;
                }
                case STATE_CHANGED: {
                    String state = (String) msg.obj;
                    if (state != null) {
                        TvLog.append(state + "\n");
                    }
                    break;
                }
                case MESSAGE_RECEIVED: {
                    String message = (String) msg.obj;
                    if (message != null) {
                        TvLog.append(message + "\n");
                    }
                    break;
                }
                case SYNCINFO_RECEIVED: {
                    String sync = (String) msg.obj;
                    if (sync != null) {
                        TvLog.append(sync + "\n");
                    }
                    break;
                }
                case TOPICS_RECEIVED: {
                    String topicList = (String) msg.obj;
                    if (topicList != null) {
                        TvLog.append(topicList + "\n");
                    }
                    break;
                }
                default:
                    break;
            }
        }
    };

    public void showToast(final String toast) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), toast,
                        Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnStart = (Button) findViewById(R.id.BtnStart);
        btnStop = (Button) findViewById(R.id.BtnStop);
        btnRescan = (Button) findViewById(R.id.BtnRescan);
        btnSubscribe = (Button) findViewById(R.id.BtnSubscribe);
        btnUnsubscribe = (Button) findViewById(R.id.BtnUnsubscribe);
        btnEnableRemoteService = (Button) findViewById(
                R.id.BtnEnableRemoteService);
        btnGetTopicList = (Button) findViewById(R.id.BtnGetTopicList);
        btnUpdateTopicList = (Button) findViewById(R.id.BtnUpdateTopicList);
        btnClearLog = (Button) findViewById(R.id.BtnClearLog);

        signUp = (Button) findViewById(R.id.signup);
        signIn = (Button) findViewById(R.id.signin);
        signOut = (Button) findViewById(R.id.signout);
        subscribeMQ = (Button) findViewById(R.id.subscribeMQService);
        TvLog = (TextView) findViewById(R.id.TvLog);

        btnStart.setOnClickListener(mClickListener);
        btnStop.setOnClickListener(mClickListener);
        btnRescan.setOnClickListener(mClickListener);
        btnEnableRemoteService.setOnClickListener(mClickListener);
        btnGetTopicList.setOnClickListener(mClickListener);
        btnUpdateTopicList.setOnClickListener(mClickListener);
        btnClearLog.setOnClickListener(mClickListener);

        signIn.setEnabled(false);
        signOut.setEnabled(false);
        btnEnableRemoteService.setEnabled(false);

        btnSubscribe.setOnClickListener(mClickListener);
        btnUnsubscribe.setOnClickListener(mClickListener);
        signUp.setOnClickListener(mClickListener);
        signIn.setOnClickListener(mClickListener);
        signOut.setOnClickListener(mClickListener);
        subscribeMQ.setOnClickListener(mClickListener);

        mConsumerSample = new ConsumerSample(getApplicationContext());
        mConsumerSample.setHandler(mHandler);
    }

    @Override
    protected void onDestroy() {
        if (isStarted)
            mConsumerSample.stopNotificationConsumer();
        super.onDestroy();
    }

    Button.OnClickListener mClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.BtnStart: {
                    if (!isStarted) {
                        Log.i(TAG, "Start NS Consumer Service");

                        TvLog.setText("Start NS-Consumer\n");
                        mConsumerSample.startNotificationConsumer();
                        isStarted = true;
                    } else {
                        Log.e(TAG, "NS Consumer Service has already started");
                        showToast("Error : Service has already started");
                    }
                }
                    break;

                case R.id.BtnStop: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to stop service. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("Stop NS-Consumer\n");
                    mConsumerSample.stopNotificationConsumer();
                    isStarted = false;
                }
                    break;
                case R.id.BtnRescan: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to rescan. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("Rescan NS-Consumer\n");
                    mConsumerSample.rescanProvider();
                }
                    break;
                case R.id.BtnSubscribe: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to Subscribe. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("Subscribe NS-Consumer\n");
                    mConsumerSample.subscribe();
                }
                break;
                case R.id.BtnUnsubscribe: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to Unsubscribe. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("Unsubscribe NS-Consumer\n");
                    mConsumerSample.unsubscribe();
                }
                break;
                case R.id.BtnEnableRemoteService: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to Enable RemoteService. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("EnableRemoteService NS-Consumer\n");
                    mConsumerSample.enableRemoteService(RemoteAddress);
                }
                    break;
                case R.id.BtnGetTopicList: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to GetTopicList. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    TvLog.append("GetTopicList NS-Consumer\n");
                    mConsumerSample.getTopicsList();
                }
                    break;
                case R.id.BtnUpdateTopicList: {
                    if (!isStarted) {
                        Log.e(TAG,
                                "Fail to UpdateTopicList. Service has not been started");
                        showToast("Error : Service has not been started");
                        break;
                    }
                    if (mConsumerSample.getAcceptor()) {
                        Log.e(TAG,
                                "Operation Not Allowed. ProviderService Acceptor is not Consumer");
                        showToast(
                                "Operation Not Allowed. ProviderService Acceptor is not Consumer");
                        break;
                    }
                    TvLog.append("UpdateTopicList NS-Consumer\n");

                    TopicsList topicList = new TopicsList();
                    topicList.addTopic("OCF_TOPIC1",
                            Topic.TopicState.SUBSCRIBED);
                    topicList.addTopic("OCF_TOPIC2",
                            Topic.TopicState.SUBSCRIBED);
                    topicList.addTopic("OCF_TOPIC3",
                            Topic.TopicState.UNSUBSCRIBED);

                    mConsumerSample.updateTopicList(topicList);
                }
                    break;
                case R.id.BtnClearLog: {
                    TvLog.setText("");
                }
                    break;
                case R.id.signup: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign Up");
                        showToast("Start ConsumerService First");
                        break;
                    }
                    TvLog.append("Initiating SignUp\n");
                    signUp();
                }
                    break;
                case R.id.signin: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign In");
                        showToast("Start ConsumerService First");
                        break;
                    }
                    TvLog.append("Initiating SignIn\n");
                    signIn();
                }
                    break;
                case R.id.signout: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign out");
                        showToast("Start ConsumerService First");
                        break;
                    }
                    TvLog.append("Initiating SignOut\n");
                    signOut();
                }
                    break;
                case R.id.subscribeMQService: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to SubscribeMQService");
                        showToast("Start ProviderService First");
                        break;
                    }
                    final Dialog dialog = new Dialog(context);
                    dialog.setContentView(R.layout.dialog_mq);
                    dialog.setTitle("MQ Cloud Service Details");

                    final EditText ip = (EditText) dialog
                            .findViewById(R.id.EditTextIp);
                    final EditText mqTopic = (EditText) dialog
                            .findViewById(R.id.EditTextMqTopic);
                    if (MQCloudAddress != null && MQCloudTopic != null) {
                        ip.setText(MQCloudAddress);
                        mqTopic.setText(MQCloudTopic);
                    }

                    Button dialogButton = (Button) dialog
                            .findViewById(R.id.mqButtonOK);

                    dialogButton.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            dialog.dismiss();
                            MQCloudAddress = ip.getText().toString();
                            MQCloudTopic = mqTopic.getText().toString();
                            mConsumerSample.subscribeMQService(
                                    MQCloudAddress, MQCloudTopic);
                            TvLog.append("SubscribeMQService success\n");
                        }
                    });
                    dialog.show();
                }
                    break;
            }
        }
    };

    public void logMessage(final String text) {
        runOnUiThread(new Runnable() {
            public void run() {
                Message msg = new Message();
                msg.obj = text;
                TvLog.append(text + "\n");
            }
        });
        Log.i(TAG, text);
    }

    OcAccountManager.OnPostListener onSignUp  = new OcAccountManager.OnPostListener() {
        @Override
        public synchronized void onPostCompleted(List<OcHeaderOption> list,
                OcRepresentation ocRepresentation) {
            logMessage("signUp was successful");
            runOnUiThread(new Runnable() {
                public void run() {
                    signIn.setEnabled(true);
                    signUp.setEnabled(false);
                }
            });
            try {
                mUserID = ocRepresentation.getValue("uid");
                mAccessToken = ocRepresentation.getValue("accesstoken");
                mRefreshtoken = ocRepresentation.getValue("refreshtoken");

                logMessage("\tuserID: " + mUserID);
                logMessage("\taccessToken: " + mAccessToken);
                logMessage("\trefreshToken: " + mRefreshtoken);

                if (ocRepresentation.hasAttribute("expiresin")) {
                    int expiresIn = ocRepresentation.getValue("expiresin");
                    logMessage("\texpiresIn: " + expiresIn);
                }
            } catch (OcException e) {
                Log.e(TAG, e.toString());
            }
        }

        @Override
        public synchronized void onPostFailed(Throwable throwable) {
            logMessage("Failed to signUp");
            if (throwable instanceof OcException) {
                OcException ocEx = (OcException) throwable;
                Log.e(TAG, ocEx.toString());
                ErrorCode errCode = ocEx.getErrorCode();
                logMessage("Error code: " + errCode);
            }
        }
    };

    OcAccountManager.OnPostListener onSignIn  = new OcAccountManager.OnPostListener() {
        @Override
        public synchronized void onPostCompleted(List<OcHeaderOption> list,
                OcRepresentation ocRepresentation) {
            logMessage("signIn was successful");
            runOnUiThread(new Runnable() {
                public void run() {
                    signIn.setEnabled(false);
                    signOut.setEnabled(true);
                    btnEnableRemoteService.setEnabled(true);
                }
            });

        }

        @Override
        public synchronized void onPostFailed(Throwable throwable) {
            logMessage("Failed to signIn");
            if (throwable instanceof OcException) {
                OcException ocEx = (OcException) throwable;
                Log.e(TAG, ocEx.toString());
                ErrorCode errCode = ocEx.getErrorCode();
                logMessage("Error code: " + errCode);
                if (ErrorCode.UNAUTHORIZED_REQ != errCode) {
                    refreshToken();
                }
            }
        }
    };

    OcAccountManager.OnPostListener onSignOut = new OcAccountManager.OnPostListener() {
        @Override
        public synchronized void onPostCompleted(List<OcHeaderOption> list,
                OcRepresentation ocRepresentation) {
            logMessage("signOut was successful");
            runOnUiThread(new Runnable() {
                public void run() {
                    signIn.setEnabled(true);
                    signOut.setEnabled(false);
                    btnEnableRemoteService.setEnabled(false);
                }
            });

        }

        @Override
        public synchronized void onPostFailed(Throwable throwable) {
            logMessage("Failed to signOut");
            if (throwable instanceof OcException) {
                OcException ocEx = (OcException) throwable;
                Log.e(TAG, ocEx.toString());
                ErrorCode errCode = ocEx.getErrorCode();
                logMessage("Error code: " + errCode);
                if (ErrorCode.UNAUTHORIZED_REQ != errCode) {
                    refreshToken();
                }
            }
        }
    };


    @Override
    public void onPostCompleted(List<OcHeaderOption> ocHeaderOptions,
            OcRepresentation ocRepresentation) {

    }

    @Override
    public void onPostFailed(Throwable throwable) {

    }

    private void signIn() {
        try {
            if (mAccountManager == null) {
                mAccountManager = OcPlatform.constructAccountManagerObject(
                        CIServer,
                        EnumSet.of(OcConnectivityType.CT_ADAPTER_TCP));
            }

            mAccountManager.signIn(mUserID, mAccessToken, onSignIn);
        } catch (OcException e) {
            e.printStackTrace();
        }
    }

    private void signOut() {
        try {
            logMessage("signOut");
            if (mAccountManager == null) {
                try {
                    mAccountManager = OcPlatform.constructAccountManagerObject(
                            CIServer,
                            EnumSet.of(OcConnectivityType.CT_ADAPTER_TCP));
                } catch (OcException e) {
                    e.printStackTrace();
                }
            }
            mAccountManager.signOut(mAccessToken, onSignOut);
            signIn.setEnabled(false);
            signUp.setEnabled(true);
            btnEnableRemoteService.setEnabled(false);
            logMessage("signOut Successful");
        } catch (OcException e) {
            e.printStackTrace();
        }
    }

    private void signUp() {
        Intent intentLogin = new Intent(this, LoginActivity.class);
        startActivityForResult(intentLogin, REQUEST_LOGIN);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == Activity.RESULT_OK && requestCode == REQUEST_LOGIN) {
            mAuthCode = data.getStringExtra("authCode");
            mAuthProvider = data.getStringExtra("authProvider");
            logMessage("authCode: " + mAuthCode);
            logMessage("authProvider: " + mAuthProvider);

            final Dialog dialog = new Dialog(context);
            dialog.setContentView(R.layout.dialog_entry);
            dialog.setTitle("SignUp Acccount IP Address");
            final EditText ip = (EditText) dialog
                    .findViewById(R.id.EditTextEntry);
            if (RemoteAddress != null) {
                ip.setText(RemoteAddress);
            }
            Button dialogButton = (Button) dialog
                    .findViewById(R.id.entryButtonOK);
            dialogButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    dialog.dismiss();
                    RemoteAddress = ip.getText().toString();
                    CIServer = CI_SERVER_PREFIX + RemoteAddress;
                    logMessage("server address for signup is: \n" + CIServer);
                    try {
                        mAccountManager = OcPlatform
                                .constructAccountManagerObject(CIServer, EnumSet
                                        .of(OcConnectivityType.CT_ADAPTER_TCP));
                        logMessage("Calling signup API");

                        if (mAuthProvider.equals("samsung")
                                || mAuthProvider.equals("samsung-us")) {
                            logMessage("Auth provider is samsung");
                            Map<String, String> options = new HashMap<>();
                            options.put("auth_server_url",
                                    "us-auth2.samsungosp.com");
                            options.put("api_server_url",
                                    "us-auth2.samsungosp.com");
                            mAccountManager.signUp(mAuthProvider, mAuthCode,
                                    options, onSignUp);
                        } else {
                            mAccountManager.signUp(mAuthProvider, mAuthCode,
                                    onSignUp);
                        }
                    } catch (OcException e) {
                        e.printStackTrace();
                    }
                }
            });
            dialog.show();
        }
    }

    OcResource.OnPostListener onRefreshTokenPost = new OcResource.OnPostListener() {
        @Override
        public void onPostCompleted(List<OcHeaderOption> list,
                OcRepresentation ocRepresentation) {
            logMessage("RefreshToken Completed.");
            try {
                mAccessToken = ocRepresentation.getValue("accesstoken");
                mRefreshtoken = ocRepresentation.getValue("refreshtoken");
            } catch (OcException e) {
                e.printStackTrace();
            }
            signIn();
        }

        @Override
        public void onPostFailed(Throwable throwable) {
            logMessage("RefreshToken failed.");
            Log.d(TAG, "onRefreshTokenPost failed..");
        }
    };

    public void refreshToken() {

        if (deviceID == null) {
            final Dialog dialog = new Dialog(context);
            dialog.setContentView(R.layout.dialog_entry);
            dialog.setTitle("Enter Device Id");
            dialog.setCanceledOnTouchOutside(false);
            final EditText id = (EditText) dialog
                    .findViewById(R.id.EditTextEntry);

            Button dialogButton = (Button) dialog
                    .findViewById(R.id.entryButtonOK);
            dialogButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    dialog.dismiss();
                    deviceID = id.getText().toString();
                }
            });
            dialog.show();
        }
        try {
            OcResource authResource = OcPlatform.constructResourceObject(
                    CIServer, "/.well-known/ocf/account/tokenrefresh",
                    EnumSet.of(OcConnectivityType.CT_ADAPTER_TCP,
                            OcConnectivityType.CT_IP_USE_V4),
                    false, Arrays.asList("oic.wk.account"),
                    Arrays.asList(OcPlatform.DEFAULT_INTERFACE));
            OcRepresentation rep = new OcRepresentation();

            showToast("RefreshToken in progress..");
            logMessage("RefreshToken in progress..");
            rep.setValue("di", deviceID);
            rep.setValue("granttype", "refresh_token");
            rep.setValue("refreshtoken", mRefreshtoken);
            rep.setValue("uid", mUserID);
            authResource.post(rep, new HashMap<String, String>(),
                    onRefreshTokenPost);
        } catch (OcException e) {
            e.printStackTrace();
        }

        Log.d(TAG, "No error while executing login");
    }
}
