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
import MuiThemeProvider from 'material-ui/styles/MuiThemeProvider';
import {List, ListItem} from 'material-ui/List';
import Divider from 'material-ui/Divider';
import {orange700, white} from 'material-ui/styles/colors';

import ResourceList from './ResourceList';
import ResourceListToolbar from './ResourceListToolbar';
import FirmwareManagement from './FirmwareManagement';

const Client = require('../Client');

const ERROR = 'error';

const style = {
    list: {
        backgroundColor: orange700,
        height: '100%'
    },

    text: {
        color: white,
    }
}

function appendChildren(id, numChildren) {
    var parent = document.getElementById(id);
    while (parent.firstChild) {
      parent.removeChild(parent.firstChild);
    }

    for (var i = 0; parent.childElementCount <= numChildren; i++) {
        parent.appendChild(document.createElement('child' + i));
    }
};

class MainMenu extends React.Component {

  onDiscover = (packet) => {
    if(packet.getCode !== 69)
    {
      Client.event.emit(ERROR, "Resource Discover Failed " + packet.getCode);
      return;
    }

    var resources = Client.getResourceList(packet.getPayloadObject);

    appendChildren('body', 2);
    ReactDOM.render(
        <ResourceListToolbar />,
        document.getElementById('body').children[0]
    );
    ReactDOM.render(
        <ResourceList data={resources} />,
        document.getElementById('body').children[1]
    );
  };

  onFirmwareResources = (packet) =>{
    if(packet.getCode !== 69)
    {
      Client.event.emit(ERROR, "Firmware Discover Failed " + packet.getCode);
      return;
    }

    var resources = Client.getResourceList(packet.getPayloadObject);

    appendChildren('body', 1);
    ReactDOM.render(
        <FirmwareManagement devices={resources} />,
        document.getElementById('body').children[0]
    );
  };

    handleDiscovery = () => {
        Client.discoverResource(null, this.onDiscover);
    };

    handleFirmware = () => {
        Client.discoverResource("rt=x.org.iotivity.firmware", this.onFirmwareResources);
    };

    constructor(props, context) {
        super(props, context);
        this.state = {
            signin: props.signin
        }
    };

    render() {
        return (
            /* TODO add log page */
            <MuiThemeProvider>
                <List style={style.list}>
                    <ListItem primaryText='Resource list' style={style.text} disabled={!this.state.signin} onTouchTap={this.handleDiscovery} />
                    <Divider />
                    <ListItem primaryText='Firmware management' style={style.text} disabled={!this.state.signin} onTouchTap={this.handleFirmware} />
                    <Divider />
                    <ListItem primaryText='Log view' style={style.text} />
                </List>
            </MuiThemeProvider>
        );
    };
};

export default MainMenu;
