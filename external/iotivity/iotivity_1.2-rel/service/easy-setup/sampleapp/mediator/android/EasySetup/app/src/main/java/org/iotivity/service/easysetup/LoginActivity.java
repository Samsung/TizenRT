/**
 * ***************************************************************
 * <p/>
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 * <p/>
 * <p/>
 * <p/>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * <p/>
 * http://www.apache.org/licenses/LICENSE-2.0
 * <p/>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p/>
 * ****************************************************************
 */

package org.iotivity.service.easysetup;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.UrlQuerySanitizer;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcException;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.OcProvisioning;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class LoginActivity extends Activity {
    private static final String TAG = "Easysetup Login: ";

    /* Samsung account */
    private WebView mWebView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        /* For Samsung account authentifaction */
        mWebView = (WebView) findViewById(R.id.webView);
        mWebView.setVisibility(View.VISIBLE);
        mWebView.setInitialScale(200);
        mWebView.getSettings().setJavaScriptEnabled(true);
        //mWebView.getSettings().setSupportZoom(true);
        mWebView.getSettings().setBuiltInZoomControls(true);
        mWebView.setWebViewClient(new WebViewClientClass());

        mWebView.loadUrl("https://github.com/login?return_to=%2Flogin%2Foauth%2Fauthorize%3Fclient_id%3Dea9c18f540323b0213d0%26redirect_uri%3Dhttp%253A%252F%252Fwww.example.com%252Foauth_callback%252F");
    }

    public void onDestroy() {
        super.onDestroy();
    }

    private class WebViewClientClass extends WebViewClient {

        @Override
        public void onPageFinished(WebView view, String url) {
            Log.e(TAG, "called!!! url=" + url);

            if(url.contains("http://www.example.com/oauth_callback/")){

                mWebView.setVisibility(View.INVISIBLE);

                //parsing url
                UrlQuerySanitizer sanitizer = new UrlQuerySanitizer();
                sanitizer.setAllowUnregisteredParamaters(true);
                sanitizer.parseUrl(url);

                String mAuthCode = null;
                mAuthCode = sanitizer.getValue("code");

                Intent intent = getIntent();
                intent.putExtra("authCode", mAuthCode);
                intent.putExtra("authProvider", "github");

                setResult(RESULT_OK, intent);

                finish();
            }
        }
    }
}
