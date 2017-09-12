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
import Checkbox from 'material-ui/Checkbox';
import {white} from 'material-ui/styles/colors';
import Paper from 'material-ui/Paper';

import SignupButton from './SignupButton';

const Client = require('../Client');

const style = {
    basic: {
        margin: 7
    },

    button: {
        color: white
    },

    checkbox: {
        marginLeft: '100px',
        width: '200px'
    },

    error: {
        textAlign: 'left',
    },

    font: {
        fontWeight: 'bold'
    },

    paper: {
        textAlign: 'center',
        margin: 'auto',
        position: 'absolute',
        top: '20%',
        left: '38%',
    }
};

const errorText = 'This field is required';

class SigninPage extends React.Component {

    handleChange = (event) => {
        var currentUidError = this.state.uidError;
        var currentAccesstokenError = this.state.accesstokenError;

        switch(event.target.id) {
            case 'uid':
                this.setState({ uid : event.target.value });
                if (event.target.value.length !== 0) {
                    currentUidError = '';
                } else {
                    currentUidError = errorText;
                }
                break;
            case 'accesstoken':
                this.setState({ accesstoken : event.target.value });
                if (event.target.value.length !== 0) {
                    currentAccesstokenError = '';
                } else {
                    currentAccesstokenError = errorText;
                }
                break;
            default:
                break;
        }
        this.setState({ uidError : currentUidError, accesstokenError : currentAccesstokenError })
    };

    handleSignin = () => {
        if (this.state.uid.length === 0 || this.state.accesstoken.length === 0) {
            return;
        }

        console.log("di: " + this.state.di);
        console.log("uid: " + this.state.uid);
        console.log("accesstoken: " + this.state.accesstoken);
        Client.writeClientData([["uid", this.state.uid], ["accesstoken", this.state.accesstoken]]);
        Client.signIn(this.state.di, this.state.uid, this.state.accesstoken);
    };

    onAutoSigninChecked = (event, isInputChecked) => {
        this.setState({ autosignin: isInputChecked });

        Client.writeClientData([['autosignin', isInputChecked]]);
    };

    constructor(props, context) {
        super(props, context);
        this.state = {
                    autosignin: props.autosignin,
                    di: props.di,
                    uid: props.uid,
                    accesstoken: props.accesstoken,
                    uidError: '',
                    accesstokenError: '',
               };

        if(this.state.autosignin === true) {
         Client.signIn(this.state.di, this.state.uid, this.state.accesstoken);
        }
    };

    render() {
        const actions = (
            <div style={{ padding: '30px 50px 30px 50px' }}>
                <p style={{ fontWeight: 'bold', fontSize: 25 }}> Sign In </p>
                <TextField
                    floatingLabelText="Device Id"
                    disabled={true}
                    defaultValue={this.state.di}
                    style={{ width: '400px' }}
                /> <br />
                <TextField
                    id='uid'
                    floatingLabelText="User UUID"
                    defaultValue={this.state.uid}
                    style={{ width: '400px' }}
                    errorText={this.state.uidError}
                    errorStyle={style.error}
                    onChange={this.handleChange.bind(this)}
                /> <br />
                <TextField
                    id='accesstoken'
                    floatingLabelText="Access token"
                    defaultValue={this.state.accesstoken}
                    style={{ width: '400px' }}
                    errorText={this.state.accesstokenError}
                    errorStyle={style.error}
                    onChange={this.handleChange.bind(this)}
                /> <br />
                <Checkbox
                    label="Auto Sign-In"
                    defaultChecked={this.state.autosignin}
                    style={style.checkbox}
                    onCheck={this.onAutoSigninChecked}
                /> <br /> <br />
                <div style={{ display: 'flex' }}>
                    <SignupButton di={this.state.di} />
                    <RaisedButton
                        label="Sign-In"
                        style={style.basic}
                        onTouchTap={this.handleSignin.bind(this)}
                    />
                </div>
            </div>
        );

        return (
            <MuiThemeProvider>
                <div style={{ height: '100%' }}>
                    <Paper style={style.paper} zDepth={1}>
                    {actions}
                    </Paper>
                </div>
            </MuiThemeProvider>
        );
    }
};

export default SigninPage;
