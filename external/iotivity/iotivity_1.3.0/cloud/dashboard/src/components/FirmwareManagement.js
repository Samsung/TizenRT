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

import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import {List, ListItem} from 'material-ui/List';
import RaisedButton from 'material-ui/RaisedButton';
import TextField from 'material-ui/TextField';

const Client = require('../Client');

const style = {
    basic: {
        padding: 30
    },

    button: {
        margin: 7,
    },
}

class FirmwareHandler extends React.Component {
    responseCallback = (packet) => {
        console.debug("response received from " + this.state.uri);
        var response = packet.getMethod + '\n';
        if (packet.getPayloadObject !== null) {
            response += packet.getPayloadString;
        }
        this.setState({ response: response });
    }

    handleFirmwareChanged = (event, newValue) => {
        this.setState({ firmwarelink: newValue })
    };

    handleNewVersionChanged = (event, newValue) => {
        this.setState({ newversion: newValue })
    };

    handleButtonClicked = (method) => {
        var payload = '{"packageuri":"' + this.state.firmwarelink
        + '", "newversion": "' + this.state.newversion + '"}';
        Client.sendMessage(this.state.uri, method, JSON.parse(payload), null, this.responseCallback);
    };

    onFirmware = (packet) => {
        console.debug("response received from " + this.state.uri);
        var response = packet.getMethod + '\n';
        if (packet.getPayloadObject !== null) {
            response += packet.getPayloadString;
        }
        this.setState({ response: response });
    }

    constructor(props, context) {
        super(props, context);
        this.state = {
            uri: props.uri,
            firmwarelink: null,
            newversion: null,
            currentversion: null,
            response: '',
        }

        Client.sendMessage(this.state.uri, "GET", null, null, this.onFirmware);
    }

    render() {
        return (
            <div>
                <TextField
                    floatingLabelText="Firmware Link"
                    fullWidth={true}
                    multiLine={true}
                    rows={1}
                    rowsMax={1}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    onChange={this.handleFirmwareChanged}
                />
                <TextField
                    floatingLabelText="New version"
                    fullWidth={true}
                    multiLine={true}
                    rows={1}
                    rowsMax={1}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    onChange={this.handleNewVersionChanged}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label="Register" onTouchTap={this.handleButtonClicked.bind(this, "POST")}
                />
                <TextField
                    floatingLabelText="response"
                    fullWidth={true}
                    multiLine={true}
                    rows={3}
                    rowsMax={3}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    value={this.state.response}
                />
            </div>
        );
    }
};

class FirmwareManagement extends React.Component {

  constructor(props, context) {
      super(props, context);
      this.state = {
          devices: props.devices
      };
  };

  makeListItem = (uri) => {
      var handler =
          <FirmwareHandler uri={uri} />;

      return handler;
  }

    render() {
        return (
          <MuiThemeProvider>
              <div>
              <List>
                  <b>Devices</b>
                  {this.state.devices.map( (row, index) => {
                      if (row.size === 0) {
                          return (<p key='notfound' style={{ fontWeight: 'bold' }}> No Device found </p>);
                      }
                      return ( <ListItem key={index} primaryText={index + ' ' + row.n}
                          secondaryText={ <p> uri: {row.uri} <br /> di: {row.di} </p> }
                          secondaryTextLines={2}
                          nestedItems={[<ListItem key={index + '_item'} disabled={true}>
                          {this.makeListItem(row.uri)} </ListItem>]}
                          nestedListStyle={{ textAlign:'right' }} primaryTogglesNestedList={true}
                          innerDivStyle={{ fontWeight: 'bold' }}/>
                      );})
                  }
              </List>
              </div>
          </MuiThemeProvider>
        );
    }
}

export default FirmwareManagement;
