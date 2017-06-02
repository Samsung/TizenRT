/*
//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

package org.iotivity.service.ns.sample.provider;

import android.app.Dialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;
import android.app.Activity;

import org.iotivity.base.ErrorCode;
import org.iotivity.base.OcAccountManager;
import org.iotivity.base.OcConnectivityType;
import org.iotivity.base.OcException;
import org.iotivity.base.OcHeaderOption;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcRepresentation;
import org.iotivity.base.OcResource;

import java.util.Arrays;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends Activity
        implements OcAccountManager.OnPostListener {

    private final String        TAG                 = "NS_MAIN_ACTIVITY";
    private static final int    CONSUMER_SUBSCRIBED = 1;
    private static final int    MESSAGE_SYNC        = 2;
    private final int           REQUEST_LOGIN       = 1;
    private static final String CI_SERVER_PREFIX    = "coap+tcp://";
    private final Context       context             = this;

    public static String        deviceID            = null;
    public static String        CIServer            = null;
    public static String        RemoteAddress       = null;
    public static String        MQCloudAddress      = null;
    public static String        MQCloudTopic        = null;

    private Button              btnTitle;
    private Button              btnBody;
    private Button              btnTopic;
    private Button              btnSend;
    private Button              btnStart;
    private Button              btnRegister;
    private Button              btnSet;
    private Button              btnStop;
    private Button              btnLog;
    private Button              signUp, signIn, signOut;
    private Button              remoteService, subscribeMQ;
    private EditText            editTextTitle;
    private EditText            editTextBody;
    private EditText            editTextTopic;
    private RadioButton         radioProvider;
    private RadioButton         radioConsumer;
    private static TextView     TvLog;

    private OcAccountManager    mAccountManager;
    String                      mAuthCode;
    String                      mAuthProvider;
    String                      mRefreshtoken;
    String                      mUserID;
    String                      mAccessToken;

    private static int          notiId              = 100;
    private boolean             isStarted           = false;
    private boolean             gAcceptor           = true;
    private boolean             gRemoteService      = true;

    private NotiListener        mNotiListener       = null;
    private ProviderSample      mProviderSample     = null;

    public static Handler       mHandler            = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case CONSUMER_SUBSCRIBED:
                    String ConsumerId = (String) msg.obj;
                    if (ConsumerId != null)
                        TvLog.append("Consumer Subscibed: " + ConsumerId + "\n");
                    break;

                case MESSAGE_SYNC:
                    String sync = (String) msg.obj;
                    if (sync != null)
                        TvLog.append("SyncInfo Received :" + sync + "\n");
                    break;

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

        btnTitle = (Button) findViewById(R.id.BtnTitle);
        btnBody = (Button) findViewById(R.id.BtnBody);
        btnTopic = (Button) findViewById(R.id.BtnTopic);
        btnSend = (Button) findViewById(R.id.BtnCreateNoti);

        btnStart = (Button) findViewById(R.id.BtnStart);
        btnRegister = (Button) findViewById(R.id.BtnRegister);
        btnSet = (Button) findViewById(R.id.BtnSet);
        btnLog = (Button) findViewById(R.id.BtnLog);
        btnStop = (Button) findViewById(R.id.BtnStop);

        signUp = (Button) findViewById(R.id.signup);
        signIn = (Button) findViewById(R.id.signin);
        signOut = (Button) findViewById(R.id.signout);
        remoteService = (Button) findViewById(R.id.remoteService);
        subscribeMQ = (Button) findViewById(R.id.subscribeMQService);

        editTextTitle = (EditText) findViewById(R.id.EditTextTitle);
        editTextBody = (EditText) findViewById(R.id.EditTextBody);
        editTextTopic = (EditText) findViewById(R.id.EditTextTopic);

        radioProvider = (RadioButton) findViewById(R.id.RadioProvider);
        radioConsumer = (RadioButton) findViewById(R.id.RadioConsumer);

        TvLog = (TextView) findViewById(R.id.TvLog);

        btnTitle.setEnabled(false);
        btnBody.setEnabled(false);
        btnTopic.setEnabled(false);

        signIn.setEnabled(false);
        signOut.setEnabled(false);
        remoteService.setEnabled(false);

        btnSend.setOnClickListener(mClickListener);
        btnStart.setOnClickListener(mClickListener);
        btnRegister.setOnClickListener(mClickListener);
        btnSet.setOnClickListener(mClickListener);
        btnLog.setOnClickListener(mClickListener);
        btnStop.setOnClickListener(mClickListener);
        radioProvider.setOnClickListener(mClickListener);
        radioConsumer.setOnClickListener(mClickListener);

        signUp.setOnClickListener(mClickListener);
        signIn.setOnClickListener(mClickListener);
        signOut.setOnClickListener(mClickListener);

        remoteService.setOnClickListener(mClickListener);
        subscribeMQ.setOnClickListener(mClickListener);

        mProviderSample = new ProviderSample(getApplicationContext());
        mProviderSample.setHandler(mHandler);

        mNotiListener = new NotiListener(this);
        Intent intent = new Intent(
                "android.settings.ACTION_NOTIFICATION_LISTENER_SETTINGS");
        startActivity(intent);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    public ProviderSample getProviderSample() {
        return mProviderSample;
    }

    Button.OnClickListener mClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.RadioProvider: {
                    if (isStarted == false) {
                        gAcceptor = true;
                        showToast("Provider as acceptor is " + gAcceptor);
                    } else
                        showToast(
                                "Start ProviderService again to change acceptor as provider");
                }
                    break;

                case R.id.RadioConsumer: {
                    if (isStarted == false) {
                        gAcceptor = false;
                        showToast("Provider as acceptor is " + gAcceptor);
                    } else
                        showToast(
                                "Start ProviderService again to change acceptor as consumer");
                }
                    break;

                case R.id.BtnStart: {
                    if (isStarted == false) {
                        Log.i(TAG, "Start  Provider Service");
                        TvLog.setText("Start Provider Service\n");
                        mProviderSample.start(gAcceptor);
                        isStarted = true;
                        radioProvider.setEnabled(false);
                        radioConsumer.setEnabled(false);
                        // refreshToken();

                    } else {
                        Log.e(TAG, " Provider Service had already started");
                        showToast(" Provider Service had already started");
                    }
                }
                    break;

                case R.id.BtnRegister: {
                    if (isStarted == false) {
                        Log.i(TAG, "Start  Provider Service");
                        TvLog.append("Register Topic : OCF_TOPIC1\n");
                        TvLog.append("Register Topic : OCF_TOPIC2\n");
                        TvLog.append("Register Topic : OCF_TOPIC3\n");
                        TvLog.append("Register Topic : OCF_TOPIC4\n");
                        showToast("Start Provider Service First");
                        break;
                    }
                    mProviderSample.registerTopic();

                }
                    break;

                case R.id.BtnSet: {
                    if (isStarted == false) {
                        Log.i(TAG, "Start Provider Service");
                        TvLog.append("Set Topic : OCF_TOPIC1\n");
                        TvLog.append("Set Topic : OCF_TOPIC2\n");
                        TvLog.append("Set Topic : OCF_TOPIC3\n");
                        TvLog.append("Set Topic : OCF_TOPIC4\n");
                        showToast("Start Provider Service First");
                        break;
                    }
                    if (gAcceptor == false) {
                        showToast(
                                "Operation Not Permitted: \nStart Provider Service with provider as acceptor");
                        break;
                    }
                    mProviderSample.setTopic();

                }
                    break;

                case R.id.BtnCreateNoti: {

                    String id = Integer.toString(notiId); // generate
                                                          // notificaion ID
                    String title = editTextTitle.getText().toString();
                    String body = editTextBody.getText().toString();
                    String topic = editTextTopic.getText().toString();

                    if (isStarted == false) {
                        Log.e(TAG, "Fail to send NSMessage");
                        showToast("Start ProviderService First");
                        break;
                    }

                    // Build android noti object and send it to Notification
                    // service receiver
                    Notification.Builder notiBuilder = new Notification.Builder(
                            getApplicationContext());
                    notiBuilder.setContentTitle(title);
                    notiBuilder.setContentText(body);
                    notiBuilder.setSubText(topic);
                    notiBuilder.setPriority(Notification.PRIORITY_MAX);
                    notiBuilder.setDefaults(Notification.DEFAULT_ALL);
                    notiBuilder.setSmallIcon(R.mipmap.ic_launcher);
                    NotificationManager notiMgr = (NotificationManager) getSystemService(
                            NOTIFICATION_SERVICE);
                    notiMgr.notify(notiId, notiBuilder.build());

                    Log.i(TAG, "#" + notiId + " notified ..");
                    TvLog.append("Send Notitication(Msg ID: " + notiId + ")\n");
                    notiId++;
                }
                    break;

                case R.id.BtnStop: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to stop service");
                        showToast("Already Stopped");
                        break;
                    }

                    mProviderSample.stop();
                    isStarted = false;
                    radioProvider.setEnabled(true);
                    radioConsumer.setEnabled(true);
                    showToast("Stopped ProviderService" + isStarted);
                    TvLog.append("Stop Provider Service\n");
                }
                    break;

                case R.id.BtnLog: {

                    TvLog.setText("");
                }
                    break;
                case R.id.signup: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign Up");
                        showToast("Start ProviderService First");
                        break;
                    }
                    TvLog.append("Initiating SignUp\n");
                    signUp();
                }
                    break;
                case R.id.signin: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign In");
                        showToast("Start ProviderService First");
                        break;
                    }
                    TvLog.append("Initiating SignIn\n");
                    signIn();
                }
                    break;
                case R.id.signout: {
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Sign out");
                        showToast("Start ProviderService First");
                        break;
                    }
                    TvLog.append("Initiating SignOut\n");
                    signOut();
                }
                    break;
                case R.id.remoteService: {
                    remoteService.setEnabled(false);
                    if (isStarted == false) {
                        Log.e(TAG, "Fail to Enable/Disable RemoteService");
                        showToast("Start ProviderService First");
                        break;
                    }
                    if (gRemoteService) {
                        TvLog.append("Enable Remote Service\n");
                        mProviderSample.enableRemoteService(RemoteAddress);
                        remoteService.setText(R.string.disableRemoteService);
                        gRemoteService = false;
                        remoteService.setEnabled(true);
                        TvLog.append("EnableRemoteService success \n");
                    } else {
                        TvLog.append("Disable Remote Service\n");
                        mProviderSample.disableRemoteService(RemoteAddress);
                        remoteService.setText(R.string.enableRemoteService);
                        gRemoteService = true;
                        remoteService.setEnabled(true);
                        TvLog.append("DisableRemoteService success\n");
                    }
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
                            mProviderSample.subscribeMQService(
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
                    remoteService.setEnabled(true);
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
                    remoteService.setEnabled(false);
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
            remoteService.setEnabled(false);
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
                                || mAuthProvider.equals("samsung-us"))

                        {
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
            Log.d(TAG, "onRefreshTokenPost failed.");
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
