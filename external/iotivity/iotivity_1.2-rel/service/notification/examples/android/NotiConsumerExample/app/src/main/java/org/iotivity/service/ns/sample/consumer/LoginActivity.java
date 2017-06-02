/*
 * ******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

package org.iotivity.service.ns.sample.consumer;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.net.UrlQuerySanitizer;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;

/**
 * This class is for login to the provider. Can be get auth code via web page.
 */
public class LoginActivity extends Activity {
    private static final String TAG           = "OIC_SIMPLE_LOGIN";

    private final Context       context       = this;
    private WebView             mWebView      = null;
    private static String       loginAccount  = null;
    private static String       mAuthProvider = null;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        final Dialog dialog = new Dialog(context);
        dialog.setContentView(R.layout.dialog_auth);
        dialog.setTitle("Login Details");
        final EditText auth = (EditText) dialog.findViewById(R.id.EditTextAuth);
        final EditText url = (EditText) dialog.findViewById(R.id.EditTextUrl);
        if (loginAccount != null && mAuthProvider != null) {
            url.setText(loginAccount);
            auth.setText(mAuthProvider);
        }

        Button dialogButton = (Button) dialog.findViewById(R.id.dialogButtonOK);
        dialog.setCanceledOnTouchOutside(false);
        dialogButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
                loginAccount = url.getText().toString();
                mAuthProvider = auth.getText().toString();

                mWebView = (WebView) findViewById(R.id.webView);
                mWebView.setInitialScale(200);
                mWebView.getSettings().setJavaScriptEnabled(true);
                mWebView.getSettings().setBuiltInZoomControls(true);
                mWebView.setWebViewClient(new WebViewClientClass());

                mWebView.loadUrl(loginAccount);
            }
        });
        dialog.show();
    }

    public void onDestroy() {
        super.onDestroy();
    }

    private class WebViewClientClass extends WebViewClient {

        @Override
        public void onPageFinished(WebView view, String url) {
            Log.i(TAG,
                    "onPageFinished!!! Response received: called url=" + url);

            if (url.contains("code") && url.contains("code_expires_in")) {

                mWebView.setVisibility(View.INVISIBLE);

                // parsing url
                UrlQuerySanitizer sanitizer = new UrlQuerySanitizer();
                sanitizer.setAllowUnregisteredParamaters(true);
                sanitizer.parseUrl(url);

                String mAuthCode = sanitizer.getValue("code");
                Log.i(TAG, "onPageFinished!!! authCode=" + mAuthCode);

                Intent intent = getIntent();
                intent.putExtra("authCode", mAuthCode);
                intent.putExtra("authProvider", mAuthProvider);
                setResult(RESULT_OK, intent);

                finish();
            }
        }
    }
}
