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
import Subheader from 'material-ui/Subheader';
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

/* TODO handle payload json syntax error */
// Message handler & react component for each found resource.
class MessageHandler extends React.Component {
    responseCallback = (packet) => {
        console.debug("response received from " + this.state.uri);
        var response = packet.getMethod + '\n';
        if (packet.getPayloadObject !== null) {
            response += packet.getPayloadString;
        }
        this.setState({ response: response });
    }

    handleQueryChanged = (event, newValue) => {
        this.setState({ queries: newValue })
    };

    handlePayloadChanged = (event, newValue) => {
        this.setState({ payload: newValue })
    };

    handleButtonClicked = (method) => {
        Client.sendMessage(this.state.uri, method, JSON.parse(this.state.payload), this.state.queries, this.responseCallback);
        if(method === "GET OBSERVE"){
          this.setState({observeState : "GET OBSERVE CANCEL"})
        }
        else if(method === "GET OBSERVE CANCEL"){
          this.setState({observeState : "GET OBSERVE"})
        }
    };

    constructor(props, context) {
        super(props, context);
        this.state = {
            uri: props.uri,
            payload: null,
            queries: null,
            response: '',
            observeState: "GET OBSERVE"
        }
    }

    render() {
        return (
            <div>
                <TextField
                    key={this.state.uri + 'queries'}
                    floatingLabelText="additional query"
                    fullWidth={true}
                    multiLine={true}
                    rows={1}
                    rowsMax={1}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    onChange={this.handleQueryChanged}
                />
                <TextField
                    key={this.state.uri + 'payload'}
                    floatingLabelText="payload"
                    fullWidth={true}
                    multiLine={true}
                    rows={5}
                    rowsMax={5}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    onChange={this.handlePayloadChanged}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label="GET" onTouchTap={this.handleButtonClicked.bind(this, "GET")}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label="PUT" onTouchTap={this.handleButtonClicked.bind(this, "PUT")}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label="POST" onTouchTap={this.handleButtonClicked.bind(this, "POST")}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label="DELETE" onTouchTap={this.handleButtonClicked.bind(this, "DELETE")}
                />
                <RaisedButton backgroundColor="#BDBDBD" labelColor="#FFFFFF" style={style.button}
                    label={this.state.observeState} onTouchTap={this.handleButtonClicked.bind(this, this.state.observeState)}
                />
                <TextField
                    floatingLabelText="response"
                    fullWidth={true}
                    multiLine={true}
                    rows={5}
                    rowsMax={5}
                    floatingLabelFixed={true}
                    style={{ textAlign: 'left'}}
                    value={this.state.response}
                />
            </div>
        );
    }
};

// List of found resources.
class ResourceList extends React.Component {

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

    constructor(props, context) {
        super(props, context);
        this.state = {
            data: props.data,
            inputRT: '',
            inputDI: '',
        };
    };

    makeListItem = (uri) => {
        var handler =
            <MessageHandler uri={uri} />;

        return handler;
    }

    render() {
        return (
            <MuiThemeProvider>
                <div>
                    <List style={style.basic}>
                        <Subheader style={{ fontWeight: 'bold' }} >Resources</Subheader>
                        {this.state.data.map( (row, index) => {
                            if (row.size === 0) {
                                return (<p key='notfound' style={{ fontWeight: 'bold' }}> No Resource found </p>);
                            }
                            return ( <ListItem primaryText={row.uri}
                                secondaryText={ <p> rt: {row.rts} <br /> if: {row.ifs} </p> }
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

export default ResourceList;
