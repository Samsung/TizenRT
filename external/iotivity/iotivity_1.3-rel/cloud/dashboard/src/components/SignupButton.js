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
import RaisedButton from 'material-ui/RaisedButton';
import TextField from 'material-ui/TextField';
import FlatButton from 'material-ui/FlatButton';
import Dialog from 'material-ui/Dialog';
import {white} from 'material-ui/styles/colors';

const Client = require('../Client');

const style = {
    basic: {
        margin: 7
    },

    button: {
        color: white
    },

    dialog: {
        width: '500px'
    },

    dialogActions: {
        textAlign: 'center'
    },

    selectfield: {
        textAlign: 'left'
    },

    radiobutton: {
        textAlign: 'center',
        maxWidth: '250px',
        marginBottom: '16px'
    },

    error: {
        textAlign: 'left',
    },

    font: {
        fontWeight: 'bold'
    }
};

const errorText = 'This field is required';

class SignupButton extends React.Component {

    handleOpen = () => {
        this.setState({ open: true });
    };

    handleClose = () => {
        this.setState({ open: false });
    };

    handleAuthcodeButtonClicked = (value) => {
        this.setState({ provider : value, providerError : '' });
    };

    handleTextChange = (event) => {
        var currentProviderErr = this.state.providerError;
        var currentCodeErr = this.state.codeError;

        switch(event.target.id) {
            case "provider":
                this.setState({ provider : event.target.value });
                if (event.target.value.length !== 0) {
                    currentProviderErr = '';
                } else {
                    currentProviderErr = errorText;
                }
                break;
            case "code":
                this.setState({ code : event.target.value });
                if (event.target.value.length !== 0) {
                    currentCodeErr = '';
                } else {
                    currentCodeErr = errorText;
                }
                break;
            default:
                return;
        }
        this.setState({ providerError : currentProviderErr, codeError : currentCodeErr })
    };

    handleSubmit = () => {
        if (this.state.provider.length === 0 || this.state.code.length === 0) {
            return;
        }

        Client.event.on('signup', this.signupCallback);
        Client.signUp(this.state.di, this.state.provider, this.state.code);
    };

    signupCallback = (uid, accesstoken) => {
        this.setState({
            dialogMessage: 'User UUID: ' + uid + '\n Accesstoken: ' + accesstoken,
            signup: true,
        });
    };

    constructor(props, context) {
        super(props, context);
        this.state = {
            di: props.di,
            open: false,
            provider: 'github',
            code: '',
            providerError: '',
            codeError: errorText,
            dialogMessage: '',
            signup: false
        };
    };

    render() {
        var actionBeforeSignup = [
                <RaisedButton label="Github" backgroundColor="#E0E0E0" style={style.basic} onTouchTap={this.handleAuthcodeButtonClicked.bind(this, "github")}
                    href="https://github.com/login?return_to=%2Flogin%2Foauth%2Fauthorize%3Fclient_id%3Dea9c18f540323b0213d0%26redirect_uri%3Dhttp%253A%252F%252Fwww.example.com%252Foauth_callback%252F"
                    target="_blank" />, <br />,
                <TextField
                    floatingLabelText="OAuth provider"
                    id="provider"
                    value={this.state.provider}
                    errorText={this.state.providerError}
                    errorStyle={style.error}
                    onChange={this.handleTextChange}
                />, <br />,
                <TextField
                    floatingLabelText="Device Id"
                    disabled={true}
                    defaultValue={this.state.di}
                />, <br />,
                <TextField
                    floatingLabelText="Auth Code"
                    id="code"
                    errorText={this.state.codeError}
                    errorStyle={style.error}
                    onChange={this.handleTextChange}
                />, <br />, <br />,
                <RaisedButton
                    label="Cancel"
                    style={style.basic}
                    onTouchTap={this.handleClose}
                />,
                <RaisedButton
                    label="Submit"
                    style={style.basic}
                    primary={true}
                    onTouchTap={this.handleSubmit}
                />,
            ];

        var actionAfterSignup = [
            <FlatButton
                label="Close"
                primary={true}
                onTouchTap={this.handleClose}
            />,
        ];

        return (
            <MuiThemeProvider>
                <div>
                    <RaisedButton label="Sign-up" backgroundColor="#9E9E9E" style={style.basic} labelStyle={style.button} onTouchTap={this.handleOpen}/>
                    <Dialog
                        title="Sign-up"
                        titleStyle={style.font}
                        actions={this.state.signup ? actionAfterSignup : actionBeforeSignup}
                        modal={true}
                        open={this.state.open}
                        contentStyle={style.dialog}
                        actionsContainerStyle={style.dialogActions}>
                    {this.state.dialogMessage}
                    </Dialog>
                </div>
            </MuiThemeProvider>
        );
    }
};

export default SignupButton;
