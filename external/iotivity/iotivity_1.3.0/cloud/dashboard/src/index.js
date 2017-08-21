/*
 * //******************************************************************
 * //
 * // Copyright 2017 Samsung Electronics All Rights Reserved.
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
import React from 'react';
import ReactDOM from 'react-dom';
import ConnectDialog from './components/ConnectDialog';
import MainAppBar from './components/MainAppBar';
import MainMenu from './components/MainMenu';
import SigninPage from './components/SigninPage';
import ErrorMessage from './components/ErrorMessage';

import './index.css';

import injectTapEventPlugin from 'react-tap-event-plugin';
injectTapEventPlugin();

const Client = require('./Client');

const CONNECTED = 'connected';
const DISCONNECTED = 'disconnected';
const SIGNUP = 'signup';
const SIGNIN = 'signin';
const SIGNOUT = 'signout';
const ERROR = 'error';

// #1 error cases: render error message.
Client.event.on(ERROR, function(message) {
    ReactDOM.unmountComponentAtNode(document.getElementById('error'));
    ReactDOM.render(
        <ErrorMessage message={message}/>,
        document.getElementById('error')
    );
});

// #2 websocket connected: default page
Client.event.on(CONNECTED, function() {
    renderDefaultPage(false, '', '');

    ReactDOM.render(
        <SigninPage di={getDeviceId()} uid={getUid()} accesstoken={getAccesstoken()} autosignin={getAutosignin()} />,
        document.getElementById('body')
    );
});

// #3 websocket disconnected: connect page
Client.event.on(DISCONNECTED, function() {
    Client.removeClientData(['autoconnect', 'autosignin']);

    unmountDefaultPage();

    ReactDOM.render(
        <ConnectDialog address={getAddress()} autoconnect={getAutoconnect()} />,
        document.getElementById('body')
    );
});

// #4 sign-in: request for resource discovery
Client.event.on(SIGNIN, function() {
    unmountDefaultPage();

    renderDefaultPage(true, getDeviceId(), getAccesstoken());

    ReactDOM.unmountComponentAtNode(document.getElementById('body'));
});

// #5 sign-out: default page
Client.event.on(SIGNOUT, function() {
    Client.removeClientData(['accesstoken']);

    unmountDefaultPage();
    renderDefaultPage(false, '', '');

    ReactDOM.render(
        <SigninPage di={getDeviceId()} uid={getUid()} accesstoken={getAccesstoken()} autosignin={getAutosignin()} />,
        document.getElementById('body')
    );
});

ReactDOM.render(
    <ConnectDialog address={getAddress()} autoconnect={getAutoconnect()} />,
    document.getElementById('body')
);

function renderDefaultPage(signinStatue, di, accesstoken) {
    ReactDOM.render(
        <MainAppBar signin={signinStatue} di={di} accesstoken={accesstoken}/>,
        document.getElementById('appbar')
    );
    ReactDOM.render(
        <MainMenu signin={signinStatue}/>,
        document.getElementById('menu')
    );
};

function unmountDefaultPage() {
    ReactDOM.unmountComponentAtNode(document.getElementById('appbar'));
    ReactDOM.unmountComponentAtNode(document.getElementById('menu'));
};

/**
 * Private API.
 */
 function generateUUID() {
     var d = new Date().getTime();
     var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
         var r = (d + Math.random()*16)%16 | 0;
         d = Math.floor(d/16);
         return (c ==='x' ? r : (r&0x3|0x8)).toString(16);
     });
     return uuid;
 };

// get device id from local storage if exists, or generate id.
function getDeviceId() {
    var deviceId = Client.readClientData('deviceId');

    if (deviceId === undefined || deviceId === null) {
        deviceId = generateUUID();
        Client.writeClientData([['deviceId', deviceId]]);
    }

    return deviceId;
};

function getAddress() {
    var address = Client.readClientData('address');

    return address;
};

function getUid() {
    var uid = Client.readClientData('uid');

    return uid;
};

function getAccesstoken() {
    var accesstoken = Client.readClientData('accesstoken');

    return accesstoken;
};

function getAutoconnect() {
    var autoconnect = (Client.readClientData('autoconnect') === 'true');

    return autoconnect;
};

function getAutosignin() {
    var autosignin = (Client.readClientData('autosignin') === 'true');

    return autosignin;
};
