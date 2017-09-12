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
var empty = new Buffer(0)

// a global index for parsing the options and the payload
// we can do this as the parsing is a sync operation
,
    index,
    lengthfield

// last five bits are 1
// 31.toString(2) => '111111'
,
    lowerCodeMask = 31,
    nextMsgId = Math.floor(Math.random() * 65535),
    codes;

codes = {
    'GET' : 1,
    'GET OBSERVE' : 1,
    'GET OBSERVE CANCEL' : 1,
    'POST' : 2,
    'PUT' : 3,
    'DELETE' : 4,

    'CREATED' : 65,
    'DELETED' : 66,
    'VALID' : 67,
    'CHANGED' : 68,
    'CONTENT' : 69,

    'BAD_REQUEST' : 128,
    'UNAUTHORIZED' : 129,
    'BAD_OPTION' : 130,
    'FORBIDDEN' : 131,
    'NOT_FOUND' : 132,
    'METHOD_NOT_ALLOWED' : 133,
    'NOT_ACCEPTABLE' : 134,

    'PRECONDITION_FAILED' : 140,
    'REQUEST_ENTITY_TOO_LARGE' : 141,
    'UNSUPPORTED_CONTENT_FORMAT' : 143,

    'INTERNAL_SERVER_ERROR' : 160,
    'NOT_IMPLEMENTED' : 161,
    'BAD_GATEWAY' : 162,
    'SERVICE_UNAVAILABLE' : 163,
    'GATEWAY_TIMEOUT' : 164,
    'PROXY_NOT_SUPPORTED' : 165,
};

module.exports.path = {
    'ACCOUNT_FULL_URI' : '/oic/account',
    'ACCOUNT_SEARCH_FULL_URI' : '/oic/account/search',
    'ACCOUNT_SESSION_FULL_URI' : '/oic/account/session',
    'ACCOUNT_TOKENREFRESH_FULL_URI' : '/oic/account/tokenrefresh',
    'KEEP_ALIVE_FULL_URI' : '/oic/ping',
    'ROUTE_FULL_URI' : '/oic/route',
    'RD_FULL_URI' : '/oic/rd',
    'WELL_KNOWN_FULL_URI' : '/oic/res',
    'DEVICE_PRESENCE_FULL_URI' : '/oic/prs',
};

module.exports.coap = {

    createRequest : function(requestMethod, uriPath, uriQuery, payload) {
        return createMessage(null, requestMethod, uriPath, uriQuery, payload, null);
    },

    createTokenRequest : function(token, requestMethod, uriPath, uriQuery, payload) {
        return createMessage(token, requestMethod, uriPath, uriQuery, payload, null);
    },

    createResponse : function(request, responseMethod, payload) {
        return createMessage(request.getToken, responseMethod, request.getUriPath, request.getUriQuery, payload, null);
    },

    createNotifyResponse : function(request, responseMethod, payload, seqNum) {
        return createMessage(request.getToken, responseMethod, request.getUriPath, request.getUriQuery, payload, seqNum);
    }
};

module.exports.parse = function parse(buffer) {

    index = 1;
    var result = {
        getCode : getCode(buffer),
        getMethod : getMethod(buffer),
        getToken : getToken(buffer),
        getUriPath : getUriPath(buffer),
        getUriPathSegment : getUriPathSegment(buffer),
        getUriQuery : getUriQuery(buffer),
        getUriQuerySegment : getUriQuerySegment(buffer),
        getPayloadString : getPayloadString(buffer),
        getPayloadObject : getPayloadObject(buffer),
        getSequenceNumber : getSequenceNumber(buffer),

    };

    return result;
};

function createMessage(token, requestMethod, uriPath, uriQuery, payload, seqNum) {
    var tokenValue,
        codeValue = (codes[requestMethod] > -1 ? codes[requestMethod] : toCode(requestMethod)),
        optionsValue = new Array(),
        payloadValue;
    if (token != null) {
        tokenValue = new Buffer(token);
    } else {
        tokenValue = require('crypto').randomBytes(8);
    }

    if (payload != null)
        payloadValue = new Buffer(JSON.stringify(payload));
    console.log();
    console.log("Token : " + tokenValue);
    console.log("Method : " + requestMethod);
    console.log("Request uri : " + uriPath);
    console.log("Request query : " + uriQuery);
    console.log("Request payload : " + payloadValue);
    console.log("Request seqNum : " + seqNum);

    if (seqNum != null) {
        var buf = Buffer.allocUnsafe(3);
        buf.writeUInt8(seqNum & 0xFF, 2);
        buf.writeUInt8((seqNum >> 8) & 0xFF, 1);
        buf.writeUInt8((seqNum >> 16) & 0xFF, 0);
        optionsValue.push({
            name : 'Observe',
            value : buf
        });

    } else {
        if (requestMethod.indexOf('OBSERVE CANCEL') > -1) {
            optionsValue.push({
                name : 'Observe',
                value : Buffer.alloc(1, 1)
            });
        } else if (requestMethod.indexOf('OBSERVE') > -1) {
            optionsValue.push({
                name : 'Observe',
                value : Buffer.alloc(0)
            });
        }
    }

    if (uriPath != null) {
        uriPath = setUriPath(uriPath);
        while (uriPath.length != 0) {
            var path = uriPath.shift();
            optionsValue.push(path);
        }
    }

    optionsValue.push({
        name : 'Content-Format',
        value : Buffer.alloc(1, 50)
    });

    if (uriQuery != null) {
        uriQuery = setUriQuery(uriQuery);
        while (uriQuery.length != 0) {
            var query = uriQuery.shift();
            optionsValue.push(query);
        }
    }

    var packet = encoder({
        token : tokenValue,
        code : codeValue,
        payload : payloadValue,
        options : optionsValue
    });

    return packet;
}

function getkey(value) {
    var flag = false;
    var keyVal;
    for (var key in codes) {
        if (codes[key] == value) {
            flag = true;
            keyVal = key;
            break;
        }
    }
    if (flag) {
        return keyVal;
    } else {
        return false;
    }
}

function getMethod(packet) {
    var value = toCode(decoder(packet).code);
    if (value == 1) {

        var observeOption = getSequenceNumber(packet).pop();
        var observe = 0;
        if(observeOption.length == 3) {
            ;
        } else {
            observe += observeOption & 0xFF;
        }

        if (observe == -1)
            ;
        else if (observe == 1)
            return 'GET OBSERVE CANCEL';
        else
            return 'GET OBSERVE';
    }
    var str = getkey(toCode(decoder(packet).code));
    return str;
}

function getCode(packet) {
    return toCode(decoder(packet).code);
}

function getToken(packet) {
    return decoder(packet).token;
}

function getUriPath(packet) {
    var uriPath = new String(),
        value = getOptionValue(packet, 'Uri-Path');

    while (value.length != 0) {
        uriPath += ("/" + value.pop());
    }

    return uriPath;
}

function getUriPathSegment(packet) {
    var uriPathSegment = new Array(),
        value = getOptionValue(packet, 'Uri-Path');

    while (value.length != 0) {
        uriPathSegment.push(value.pop());
    }
    return uriPathSegment;
}

function getPayloadString(packet) {
    var payload = decoder(packet).payload;
    if (payload.length != 0)
        return decoder(packet).payload;
    return null;
}

function getPayloadObject(packet) {
    var payload = decoder(packet).payload;
    if (payload.length != 0)
        return JSON.parse(decoder(packet).payload);
    return null;
}

function getPayloadValue(packet, property) {
    if (payload.length != 0) {
        var payload = JSON.parse(decoder(packet).payload);
        if (payload.property != null) {
            return payload.property;
        }
    }
    return null;
}

function setUriPath(path) {
    var options = new Array();
    var pathArray = path.split('/');
    for (var i = 0; i < pathArray.length; i++) {
        var value = {
            name : 'Uri-Path',
            value : new Buffer(pathArray[i])
        };
        if (pathArray[i].length != 0) {
            options.push(value);
        }
    }
    return options;
}

function setUriQuery(query) {
    var options = new Array();
    var queryArray = query.split(';');
    for (var i = 0; i < queryArray.length; i++) {
        var value = {
            name : 'Uri-Query',
            value : new Buffer(queryArray[i])
        };
        if (queryArray[i].length != 0 && queryArray[i] != null) {
            options.push(value);
        }
    }
    return options;
}

function getUriQuery(packet) {
    var uriQuery = new String(),
        value = getOptionValue(packet, 'Uri-Query');

    if (value.length == 0 || value == null)
        return null;

    while (value.length != 0) {
        uriQuery += value.pop();
        if (value.length != 0)
            uriQuery += ";";
    }
    return uriQuery;
}

function getSequenceNumber(packet) {
    var value = getOptionValue(packet, 'Observe');

    if (value.length == 0)
        return -1;

    return value;
}

function getUriQuerySegment(packet) {
    var uriQuerySegment = new Array(),
        value = getOptionValue(packet, 'Uri-Path');

    if (value.length == 0)
        return null;

    while (value.length != 0) {
        var valueArray = value.pop().toString().split('=');
        uriQuerySegment.push(valueArray[1]);
    }
    return uriQuerySegment;
}

function getOptionValue(packet, optionName) {
    var value = new Array(),
        options = decoder(packet).options;
    while (options.length != 0) {
        var option = options.pop();
        if (option.name == optionName) {
            value.push(option.value);
        }
    }
    return value;
}

function encoder(packet) {
    var buffer,
        byte,
        pos = 0,
        options,
        i,
        bufferLength,
        length;

    packet = fillGenDefaults(packet);
    options = prepareOptions(packet);
    length = calculateLength(packet, options);
    bufferLength = calculateBuffer(packet, length);

    console.log("length for Option and Payload : " + length);

    if (bufferLength > 4294967295 + 65535 + 1 + 14)
        throw new Error('Max packet size is ' + (4294967295 + 65535 + 1 + 14) + ': current is ' + bufferLength);

    buffer = new Buffer(bufferLength);

    byte = 0;
    // The Length (Len) field MUST be set to zero
    // because the WebSockets frame containsthe length.
    // byte |= calulateExtendedLength(length) << 4;
    // first four bits are Length
    byte |= packet.token.length;
    // last four bits are Token Length
    buffer.writeUInt8(byte, pos++);

    // if (length < 13) {
        // ;
    // } else if (length < 256 + 13 + 1) {
        // buffer.writeUInt8(length - 13, pos++);
    // } else if (length < 65535 + 269 + 1) {
        // buffer.writeUInt16BE(length - 269, pos);
        // pos += 2;
    // } else if (length < 4294967295 + 65535 + 1) {
        // buffer.writeUInt8(length - 65805, pos);
        // pos += 2;
    // } else {
        // console.log('Length must be less than 4GB, current is ' + length);
    // }

    // code can be humized or not
    if (codes[packet.code])
        buffer.writeUInt8(codes[packet.code], pos++);
    else
        buffer.writeUInt8(toCode(packet.code), pos++);

    // the token might be an empty buffer
    packet.token.copy(buffer, pos);
    pos += packet.token.length;

    // write the options
    for ( i = 0; i < options.length; i++) {
        options[i].copy(buffer, pos);
        pos += options[i].length;
    }

    if (packet.code !== '0.00' && packet.payload != '') {

        // payload separator
        buffer.writeUInt8(255, pos++);
        packet.payload.copy(buffer, pos);
    }

    return buffer;
}

module.exports.generater = function generater(packet) {
    var buffer,
        byte,
        pos = 0,
        options,
        i,
        bufferLength,
        length;

    /* fillGenDefaults : add default values to generate packet */
    packet = fillGenDefaults(packet);
    options = prepareOptions(packet);
    length = calculateLength(packet, options);
    bufferLength = calculateBuffer(packet, length);

    console.log("length for Option and Payload : " + length);

    if (bufferLength > 4294967295 + 65535 + 1 + 14)
        throw new Error('Max packet size is ' + (4294967295 + 65535 + 1 + 14) + ': current is ' + bufferLength);

    buffer = new Buffer(bufferLength);

    // first byte
    byte = 0;
    // byte |= calulateExtendedLength(length) << 4;
    // first four bits are Length
    byte |= packet.token.length;
    // last four bits are Token Length
    buffer.writeUInt8(byte, pos++);

    // if (length < 13) {
        // ;
    // } else if (length < 256 + 13 + 1) {
        // buffer.writeUInt8(length - 13, pos++);
    // } else if (length < 65535 + 269 + 1) {
        // buffer.writeUInt16BE(length - 269, pos);
        // pos += 2;
    // } else if (length < 4294967295 + 65535 + 1) {
        // buffer.writeUInt8(length - 65805, pos);
        // pos += 2;
    // } else {
        // console.log('Length must be less than 4GB, current is ' + length);
    // }


    // code can be humized or not
    if (codes[packet.code])
        buffer.writeUInt8(codes[packet.code], pos++);
    else
        buffer.writeUInt8(toCode(packet.code), pos++);

    // the token might be an empty buffer
    packet.token.copy(buffer, pos);
    pos += packet.token.length;

    // write the options
    for ( i = 0; i < options.length; i++) {
        options[i].copy(buffer, pos);
        pos += options[i].length;
    }

    if (packet.code !== '0.00' && packet.payload != '') {

        // payload separator
        buffer.writeUInt8(255, pos++);
        packet.payload.copy(buffer, pos);
    }

    return buffer;
};

// module.exports.decoder = function decoder(buffer) {
function decoder(buffer) {
    index = 1;
    lengthfield = parseLength(buffer);

    if (lengthfield >= 13) {
        var extendedLength = parseExtendedLength(buffer, lengthfield);
    }

    var result = {
        code : parseCode(buffer),
        token : parseToken(buffer),
        options : null,
        payload : null,
    };

    if (result.code !== '0.00') {
        result.options = parseOptions(buffer);
        result.payload = buffer.slice(index + 1);
    } else {
        if (buffer.length != 4)
            throw new Error('Empty messages must be empty');

        result.options = [];
        result.payload = empty;
    }

    return result;
};

function parseLength(buffer) {
    var length = buffer.readUInt8(0) >> 4;

    // if (length == 0)
        // throw new Error('Message Length is 0');

    // console.log('length : ' + length);

    return length;
}

function parseExtendedLength(buffer, lengthfield) {
    var extendedLength;

    if (lengthfield == 13) {
        extendedLength = buffer.slice(index, index++) + 13;
    } else if (lengthfield == 14) {
        extendedLength = buffer.slice(index, index + 2) + 269;
        index += 2;
    } else if (lengthfield == 15) {
        extendedLength = buffer.slice(index, index + 4) + 65805;
        index += 4;
    } else
        throw new Error('lengthfield is invalid : current lengthfield is ' + lengthfield);

    // console.log('extendedLength : ' + extendedLength);

    return extendedLength;
}

function parseCode(buffer) {
    var byte = buffer.readUInt8(index),
        code = '' + (byte >> 5) + '.';

    index++;

    byte = byte & lowerCodeMask;

    if (byte < 10)
        code += '0' + byte;
    else
        code += byte;

    // console.log('code : ' + code);

    return code;
}

function parseToken(buffer) {
    var length = buffer.readUInt8(0) & 0x0F,
        token;

    if (length > 8) {
        throw new Error('Token length not allowed');
    }

    token = buffer.slice(index, index + length);

    index += length;

    // console.log('token : ' + token);

    return token;
}

var numMap = {
    '1' : 'If-Match',
    '3' : 'Uri-Host',
    '4' : 'ETag',
    '5' : 'If-None-Match',
    '6' : 'Observe',
    '7' : 'Uri-Port',
    '8' : 'Location-Path',
    '11' : 'Uri-Path',
    '12' : 'Content-Format',
    '14' : 'Max-Age',
    '15' : 'Uri-Query',
    '17' : 'Accept',
    '20' : 'Location-Query',
    '35' : 'Proxy-Uri',
    '39' : 'Proxy-Scheme',
    '60' : 'Size1'
};

var optionNumberToString = (function genOptionParser() {

    var code = Object.keys(numMap).reduce(function(acc, key) {

        acc += 'case ' + key + ':\n';
        acc += '  return \'' + numMap[key] + '\'\n';

        return acc;
    }, 'switch(number) {\n');

    code += 'default:\n';
    code += 'return \'\' + number';
    code += '}\n';

    return new Function('number', code);
})();

function parseOptions(buffer) {

    var byte,
        number = 0,
        delta,
        length,
        nextOption = true,
        options = [],
        option;

    while (index < buffer.length) {
        byte = buffer.readUInt8(index);

        if (byte === 255 || index > buffer.length) {
            break;
        }

        delta = byte >> 4;
        length = byte & 15;

        index += 1;

        if (delta === 13) {
            delta = buffer.readUInt8(index) + 13;
            index += 1;
        } else if (delta === 14) {
            delta = buffer.readUInt16BE(index) + 269;
            index += 2;
        } else if (delta === 15) {
            throw new Error('Wrong option delta');
        }

        if (length === 13) {
            length = buffer.readUInt8(index) + 13;
            index += 1;
        } else if (length === 14) {
            length = buffer.readUInt16BE(index) + 269;
            index += 2;
        } else if (length === 15) {
            throw new Error('Wrong option length');
        }

        number += delta;

        var name = optionNumberToString(number),
            value = buffer.slice(index, index + length);

        // console.log("name : " + name);
        // console.log("value : " + value.toString());
        // console.log();

        options.push({
            name : name,
            value : value
        });

        index += length;
    }

    return options;
}

function toCode(code) {
    var split = code.split && code.split('.'),
        by = 0;

    if (split && split.length === 2) {
        by |= parseInt(split[0]) << 5;
        by |= parseInt(split[1]);
    } else {

        if (!split)
            code = parseInt(code);

        by |= (code / 100) << 5;
        by |= (code % 100);
    }

    return by;
}

function fillGenDefaults(packet) {
    if (!packet)
        packet = {};

    if (!packet.payload)
        packet.payload = empty;

    if (!packet.token)
        packet.token = empty;

    if (packet.token.length > 8)
        throw new Error('Token too long');

    if (!packet.code)
        packet.code = '0.01';

    if (!packet.options)
        packet.options = [];

    return packet;
}

function calculateBuffer(packet, length) {
    var extendedLengthValue = calulateExtendedLength(length),
        extendLength;

    if (extendedLengthValue < 13) {
        extendLength = 0;
    } else if (extendedLengthValue == 13) {
        extendLength = 1;
    } else if (extendedLengthValue == 14) {
        extendLength = 2;
    } else if (extendedLengthValue == 15) {
        extendLength = 4;
    } else {
        console.log('Length must be less than 4GB, current is ' + length);
    }

    length += (2 + packet.token.length); //+ extendLength);

    return length;
}

function calculateLength(packet, options) {

    var length = packet.payload.length,
        i,
        lengthValue;

    if (packet.payload != '')
        length += 1;

    for ( i = 0; i < options.length; i++) {
        length += options[i].length;
    }

    return length;
}

function calulateExtendedLength(length) {
    var lengthValue;
    if (length < 13) {
        lengthValue = length;
    } else if (length < 256 + 13 + 1) {
        lengthValue = 13;
    } else if (length < 65535 + 269 + 1) {
        lengthValue = 14;
    } else if (length < 4294967295 + 65535 + 1) {
        lengthValue = 15;
    } else {
        console.log('Length must be less than 4GB, current is ' + length);
    }
    return lengthValue;
}

var optionStringToNumber = (function genOptionParser() {

    var code = Object.keys(numMap).reduce(function(acc, key) {

        acc += 'case \'' + numMap[key] + '\':\n';
        acc += '  return \'' + key + '\'\n';

        return acc;
    }, 'switch(string) {\n');

    code += 'default:\n';
    code += 'return parseInt(string)';
    code += '}\n';

    return new Function('string', code);
})();

var nameMap = Object.keys(numMap).reduce(function(acc, key) {
    acc[numMap[key]] = key;
    return acc;
}, {});

function optionSorter(a, b) {
    a = a.name;
    b = b.name;

    a = parseInt(nameMap[a] || a);
    b = parseInt(nameMap[b] || b);
    console.log("a : " + a + ", b : " + b);
    if (a < b)
        return -1;
    if (a > b)
        return 1;

    return 0;
}

function prepareOptions(packet) {
    var options = [],
        total = 0,
        delta,
        buffer,
        byte,
        option,
        i,
        bufferSize,
        pos,
        value;


    // packet.options.sort(optionSorter);

    //console.log("path in prepareOptions : " + getUriPath(packet));

    /* add delta, length and value to buffer for each option in order */
    console.log();
    console.log('packet.options.length : ' + packet.options.length);
    console.log();

    for ( i = 0; i < packet.options.length; i++) {
        pos = 0;
        option = packet.options[i].name;
        delta = optionStringToNumber(option) - total;
        value = packet.options[i].value;

        // console.log('turn : ' + i);
        // console.log('optionStringToNumber(option) : ' + optionStringToNumber(option));
        // console.log('delta : ' + delta);
        // console.log('value.length : ' + value.length);
        // console.log('value : ' + value);

        // max option length is 1 header, 2 ext numb, 2 ext length
        buffer = new Buffer(value.length + 5);

        byte = 0;

        if (delta < 13) {
            byte |= delta << 4;
        } else if (delta < 255 + 13 + 1) {
            byte |= 13 << 4;
        } else if (delta < 65535 + 269 + 1) {
            byte |= 14 << 4;
        } else {
            byte != 15 << 4;
            console.log("Message Format Error");
        }

        if (value.length < 13) {
            byte |= value.length;
        } else if (value.length < 255 + 13 + 1) {
            byte |= 13;
        } else if (value.length < 65535 + 269 + 1) {
            byte |= 14;
        } else {
            byte != 15 << 4;
            console.log("Message Format Error");
        }

        // console.log("byte : " + byte);
        buffer.writeUInt8(byte, pos++);

        if (delta < 13) {
            ;
        } else if (delta < 255 + 13 + 1) {
            buffer.writeUInt8(delta - 13, pos++);
        } else if (delta < 65535 + 269 + 1) {
            buffer.writeUInt16BE(delta - 269, pos);
            pos += 2;
        } else {
            console.log("Message Format Error");
        }

        if (value.length < 13) {
            ;
        } else if (value.length < 269) {
            buffer.writeUInt8(value.length - 13, pos++);
        } else if (value.length < 65535 + 269 + 1) {
            buffer.writeUInt16BE(value.length - 269, pos);
            pos += 2;
        } else {
            console.log("Message Format Error");
        }

        value.copy(buffer, pos);
        pos += value.length;
        total += delta;
        options.push(buffer.slice(0, pos));
        // console.log('total : ' + total);
        // console.log('pos : ' + pos);
        // console.log('buffer : ' + buffer);
        // console.log();
    }

    return options;
}