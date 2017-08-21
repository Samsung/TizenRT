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
var EventEmitter = require('events').EventEmitter;
var WS = require('websocket').w3cwebsocket;
var convertBlob = require('blob-to-buffer');
var HashMap = require('hashmap');
var coapTokenMap = new HashMap();
var localStorage = window.localStorage;
if (typeof localStorage === "undefined" || localStorage === null) {
    var LocalStorage = require('node-localstorage').LocalStorage;
    localStorage = new LocalStorage('./client');
}

const coap = require('./components/CoapWebsocketCodec').coap;
const parse = require('./components/CoapWebsocketCodec').parse;
const path = require('./components/CoapWebsocketCodec').path;

const CONNECTED = 'connected';
const DISCONNECTED = 'disconnected';
const SIGNUP = 'signup';
const SIGNIN = 'signin';
const SIGNOUT = 'signout';
const ERROR = 'error';

var Client = new function() {

    var ws;
    this.event = new EventEmitter();

    // coap websocket client initialize.
    this.init = function(address) {
        console.log('client init: ' + address);

        /* TODO websocket secured */
        var serverURL = "ws://" + address + "/.well-known/coap";
        this.ws = new WS(serverURL, 'coap');
        this.ws.onopen = function() {
            console.debug('Connected to server ' + address);
            Client.event.emit(CONNECTED);
        };
        this.ws.onclose = function() {
            console.debug('Disconnected from server ' + address);
            Client.event.emit(DISCONNECTED);
        };
        this.ws.onerror = function() {
            console.error('Error occurs');
            Client.event.emit(ERROR, 'Error occurs while websocket connection');
            Client.event.emit(DISCONNECTED);
        };

        // response callback.
        this.ws.onmessage = function(event) {
            console.debug('Message received -');

            convertBlob(event.data, function (err, buffer) {
                if (err) throw err;

                var packet = parse(buffer);
                console.debug(packet);
                console.debug(packet.getPayloadObject);

                var func = coapTokenMap.get(packet.getToken.toString());
                func(packet);
                if(packet.getSequenceNumber === -1){
                  coapTokenMap.remove(packet.getToken.toString());
                }
            });
        };
    }

    // coap websocket client close.
    this.close = function() {
        console.log('client close');
        this.ws.close();
        Client.event.emit(DISCONNECTED);
    }

    // send sign-up request.
    this.onSignUp = function(packet) {
        if(packet.getCode === 68) {
          Client.event.emit(SIGNUP, packet.getPayloadObject.uid,
            packet.getPayloadObject.accesstoken);
        } else {
          Client.event.emit(ERROR, "SignUp Failed" + packet.getCode);
        }
    }
    this.signUp = function(di, provider, authcode) {
        console.log('client signUp');
        var payload = {
            di: di,
            authprovider: provider,
            authcode: authcode,
            devicetype: "device"
        };
        this.ws.send(this.doRequest("POST", path.ACCOUNT_FULL_URI, null, payload, this.onSignUp));
    }

    // send sign-in request.
    this.onSignIn = function(packet) {
        if(packet.getCode === 68) {
          Client.event.emit(SIGNIN);
        } else {
          Client.event.emit(ERROR, "SignIn Failed" + packet.getCode);
        }
    }
    this.signIn = function(di, uid, accesstoken) {
        console.log('client signIn');
        var payload = {
            di: di,
            uid: uid,
            accesstoken: accesstoken,
            login: true
        };
        this.ws.send(this.doRequest("POST", path.ACCOUNT_SESSION_FULL_URI, null, payload, this.onSignIn));
    }

    // send sign-out request.
    this.onSignOut = function(packet) {
        if(packet.getCode === 68) {
          Client.event.emit(SIGNOUT);
        } else {
          Client.event.emit(ERROR, "SignOut Failed" + packet.getCode);
        }
    }
    this.signOut = function(di,  accesstoken) {
        console.log('client signOut');
        var payload = {
            di: di,
            accesstoken: accesstoken,
            login: false
        };
        this.ws.send(this.doRequest("POST", path.ACCOUNT_SESSION_FULL_URI, null, payload, this.onSignOut));
    }

    // send resource discovery request.
    this.discoverResource = function(queries, response) {
        console.log('client discoverResource ' + queries);
        this.ws.send(this.doRequest("GET", path.WELL_KNOWN_FULL_URI, queries, null, response));
    }

    // send control message.
    this.sendMessage = function(uri, method, payload, queries, response) {
        console.log('client sendMessage');
        this.ws.send(this.doRequest(method, uri, queries, payload, response));
    }

    this.doRequest = function(method, uri, query, payload, response)
    {
      var newCoaptoken = require('crypto').randomBytes(8);
      coapTokenMap.set(newCoaptoken.toString(), response);
      return coap.createTokenRequest(newCoaptoken, method, uri, query, payload);
    }


    // erase data in local storage
    this.removeClientData = function(keyArray) {
        for (var i = 0; i < keyArray.length; i++) {
            localStorage.removeItem(keyArray[i]);
        }
    };

    this.writeClientData = function(keyValArray) {
        for (var i = 0; i < keyValArray.length; i++) {
            localStorage.setItem(keyValArray[i][0], keyValArray[i][1]);
        }
    };

    this.readClientData = function(key) {
        return localStorage.getItem(key);
    };


    this.getResourceList = function(discoveryPayload) {
        if (discoveryPayload === null) {
            return [];
        }

        var resourceList = [];

        console.debug("Discovered devices: " + discoveryPayload.length);
        for (var i = 0; i < discoveryPayload.length; i++) {
            for (var j = 0; j < discoveryPayload[i].links.length; j++) {
                console.debug("[" + i + "] " + discoveryPayload[i].links[j].href);
                resourceList.push({
                    n:   discoveryPayload[i].n,
                    di:  discoveryPayload[i].di,
                    uri: discoveryPayload[i].links[j].href,
                    rts: this.buildArrayString(discoveryPayload[i].links[j].rt),
                    ifs: this.buildArrayString(discoveryPayload[i].links[j].if) });
            }
        }

        return resourceList;
    }

    this.buildArrayString = function(array) {
        var result = '';
        var seperates = ', ';

        for (var i = 0; i < array.length; i++) {
            // last data
            if (i === array.length -1) seperates = '';
            result += array[i] + seperates;
        }

        return result;
    }
}();

module.exports = Client;
