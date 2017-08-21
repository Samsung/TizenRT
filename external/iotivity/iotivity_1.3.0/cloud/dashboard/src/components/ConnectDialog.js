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
import TextField from 'material-ui/TextField';
import Checkbox from 'material-ui/Checkbox';
import RaisedButton from 'material-ui/RaisedButton';
import Dialog from 'material-ui/Dialog';

const Client = require('../Client');

const style = {
    dialog: {
        width: '500px'
    },

    actions: {
        textAlign: 'center',
    },

    actionCheckbox: {
        marginLeft: '100px',
        width: '200px'
    },

    error: {
        textAlign: 'left',
    }
}

const errorText = 'This field is required';

class ConnectDialog extends React.Component {

    handleChange = (event) => {
        if (event.target.value.length !== 0) {
            this.setState({ error : '' });
        } else {
            this.setState({ error : errorText });
        }

        this.setState({
          address: event.target.value,
        });
    };

    handleSubmit = () => {
        if (this.state.address.length === 0) {
            this.setState({ error : errorText });
            return;
        }

        Client.writeClientData([['address', this.state.address]]);

        console.log('input address: ' + this.state.address);
        Client.init(this.state.address);
    };

    onAutoConnectChecked = (event, isInputChecked) => {
        this.setState({ autoconnect: isInputChecked });

        Client.writeClientData([['autoconnect', isInputChecked]]);
    };

    constructor(props, context) {
        super(props, context);
        this.state = {
                   open: true,
                   autoconnect: props.autoconnect,
                   error: '',
                   address: props.address,
               };

        if(this.state.autoconnect === true) {
          Client.init(this.state.address);
        }
    };

    render() {
        const actions = [
            <TextField
                floatingLabelText="Server address"
                defaultValue={this.state.address}
                hintText="127.0.0.1:80"
                errorText={this.state.error}
                errorStyle={style.error}
                onChange={this.handleChange.bind(this)}
            />, <br />, <br />,
            <Checkbox
                label="Auto Connect"
                defaultChecked={this.state.autoconnect}
                style={style.actionCheckbox}
                onCheck={this.onAutoConnectChecked}
            />, <br />, <br />,
            <RaisedButton
                label="Connect"
                primary={true}
                onTouchTap={this.handleSubmit.bind(this)}
            />,
        ];

        return (
            <MuiThemeProvider>
                <div>
                    <Dialog
                        title="Connect to cloud"
                        titleStyle={{ fontWeight: 'bold' }}
                        actions={actions}
                        modal={true}
                        open={this.state.open}
                        contentStyle={style.dialog}
                        actionsContainerStyle={style.actions}>
                    </Dialog>
                </div>
            </MuiThemeProvider>
        );
    }
};

export default ConnectDialog;
