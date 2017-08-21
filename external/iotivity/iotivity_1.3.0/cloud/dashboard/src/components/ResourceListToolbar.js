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
import RaisedButton from 'material-ui/RaisedButton';
import TextField from 'material-ui/TextField';
import {Toolbar, ToolbarGroup, ToolbarSeparator} from 'material-ui/Toolbar';
import ResourceList from './ResourceList';

const Client = require('../Client');

const ERROR = 'error';

class ResourceListToolbar extends React.Component {

    state = {
        inputRT: '',
        inputDI: '',
    };

    handleRTTextChanged = (event, newValue) => {
        this.setState({ inputRT: newValue });
    }

    handleDITextChanged = (event, newValue) => {
        this.setState({ inputDI: newValue });
    }

    onDiscover = (packet) => {
          if(packet.getCode !== 69)
          {
            Client.event.emit(ERROR, "Resource Discover Failed " + packet.getCode);
            return;
          }

          var resources = Client.getResourceList(packet.getPayloadObject);

          document.getElementById('body').children[1].innerHTML = "";
          ReactDOM.render(
              <ResourceList data={resources} />,
              document.getElementById('body').children[1]
          );
    };

    handleButtonClicked = () => {
        var queries = '';
        if (this.state.inputRT.length !== 0) {
            queries += 'rt='+ this.state.inputRT + ';';
        }
        if (this.state.inputDI.length !== 0) {
            queries += 'di='+ this.state.inputDI;
        }

        Client.discoverResource(queries, this.onDiscover);
    }

    render() {
        return (
            <MuiThemeProvider>
                <div>
                    <Toolbar>
                        <ToolbarGroup>
                            <TextField
                                floatingLabelText="Resource type"
                                style={{ textAlign: 'left', margin: 7 }}
                                onChange={this.handleRTTextChanged}
                                value={this.state.inputRT}
                            />
                            <TextField
                                floatingLabelText="Device id"
                                style={{ textAlign: 'left', margin: 7 }}
                                onChange={this.handleDITextChanged}
                                value={this.state.inputDI}
                            />
                            <RaisedButton label="Discover" backgroundColor="#9CCC65" onTouchTap={this.handleButtonClicked}  labelStyle={{ color: '#FFFFFF', fontWeight: 'bold' }}/>
                        </ToolbarGroup>
                        <ToolbarGroup lastChild={true}>
                            <ToolbarSeparator />
                            <RaisedButton label="Refresh" backgroundColor="#8BC34A" onTouchTap={this.handleButtonClicked}  labelStyle={{ color: '#FFFFFF', fontWeight: 'bold' }}/>
                        </ToolbarGroup>
                    </Toolbar>
                </div>
            </MuiThemeProvider>
        );
    }
}

export default ResourceListToolbar;
