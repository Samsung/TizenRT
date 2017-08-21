/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
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
package org.iotivity.cloud.base.protocols.coap;

import java.util.List;

import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.util.Log;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;

public class CoapDecoder extends ByteToMessageDecoder {

    private enum ParsingState {
        SHIM_HEADER, OPTION_PAYLOAD_LENGTH, CODE_TOKEN_OPTION, PAYLOAD, FINISH
    }

    private ParsingState nextState           = ParsingState.SHIM_HEADER;
    private int          bufferToRead        = 1;
    private int          tokenLength         = 0;
    private int          optionPayloadLength = 0;
    private CoapMessage  partialMsg          = null;
    private int          websocketLength     = -1;

    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in,
            List<Object> out) {
        try {

            // TODO: need exceptional case handling
            while (in.isReadable(bufferToRead)) {

                switch (nextState) {
                    case SHIM_HEADER:
                        int shimHeader = in.readByte();
                        bufferToRead = (shimHeader >>> 4) & 0x0F;
                        tokenLength = (shimHeader) & 0x0F;
                        switch (bufferToRead) {
                            case 13:
                                bufferToRead = 1;
                                nextState = ParsingState.OPTION_PAYLOAD_LENGTH;
                                break;
                            case 14:
                                bufferToRead = 2;
                                nextState = ParsingState.OPTION_PAYLOAD_LENGTH;
                                break;
                            case 15:
                                bufferToRead = 4;
                                nextState = ParsingState.OPTION_PAYLOAD_LENGTH;
                                break;
                            default:
                                optionPayloadLength = bufferToRead;
                                bufferToRead += 1 + tokenLength; // code + tkl
                                nextState = ParsingState.CODE_TOKEN_OPTION;
                                break;
                        }
                        break;

                    case OPTION_PAYLOAD_LENGTH:
                        switch (bufferToRead) {
                            case 1:
                                optionPayloadLength = 13
                                        + (in.readByte() & 0xFF);
                                break;

                            case 2:
                                optionPayloadLength = 269
                                        + (((in.readByte() & 0xFF) << 8)
                                                + (in.readByte() & 0xFF));
                                break;

                            case 4:
                                optionPayloadLength = 65805
                                        + (((in.readByte() & 0xFF) << 24)
                                                + ((in.readByte() & 0xFF) << 16)
                                                + ((in.readByte() & 0xFF) << 8)
                                                + (in.readByte() & 0xFF));
                                break;
                        }
                        nextState = ParsingState.CODE_TOKEN_OPTION;
                        bufferToRead = 1 + tokenLength + optionPayloadLength; // code
                                                                              // +
                                                                              // tkl
                        break;

                    case CODE_TOKEN_OPTION:
                        int code = in.readByte() & 0xFF;
                        if (code <= 31) {
                            partialMsg = new CoapRequest(code);
                        } else if (code > 224) {
                            partialMsg = new CoapSignaling(code);
                        } else {
                            partialMsg = new CoapResponse(code);
                        }

                        if (tokenLength > 0) {
                            byte[] token = new byte[tokenLength];
                            in.readBytes(token);
                            partialMsg.setToken(token);
                        }

                        if (websocketLength != -1) {
                            optionPayloadLength = websocketLength
                                    - (bufferToRead + 1); // shimheader + code +
                                                          // tokenLength
                        }

                        if (optionPayloadLength > 0) {
                            int optionLen = parseOptions(partialMsg, in,
                                    optionPayloadLength);
                            if (optionPayloadLength > optionLen) {
                                nextState = ParsingState.PAYLOAD;
                                bufferToRead = optionPayloadLength - optionLen;
                                continue;
                            }
                        }

                        nextState = ParsingState.FINISH;
                        bufferToRead = 0;

                        break;

                    case PAYLOAD:
                        byte[] payload = new byte[bufferToRead];
                        in.readBytes(payload);
                        partialMsg.setPayload(payload);
                        nextState = ParsingState.FINISH;
                        bufferToRead = 0;
                        break;

                    case FINISH:
                        nextState = ParsingState.SHIM_HEADER;
                        bufferToRead = 1;
                        out.add(partialMsg);
                        break;

                    default:
                        break;
                }
            }
            in.discardReadBytes();
        } catch (Throwable t) {
            ResponseStatus responseStatus = t instanceof ServerException
                    ? ((ServerException) t).getErrorResponse()
                    : ResponseStatus.INTERNAL_SERVER_ERROR;
            Log.f(ctx.channel(), t);
            ctx.writeAndFlush(
                    MessageBuilder.createResponse(partialMsg, responseStatus));
            ctx.close();
        }
    }

    public void decode(ByteBuf in, List<Object> out, int length)
            throws Exception {
        websocketLength = length;
        decode(null, in, out);
    }

    private int parseOptions(CoapMessage coapMessage, ByteBuf byteBuf,
            int maxLength) {

        int preOptionNum = 0;

        int startPos = byteBuf.readerIndex();

        int firstByte = byteBuf.readByte() & 0xFF;

        while (firstByte != 0xFF && maxLength > 0) {
            int optionDelta = (firstByte & 0xF0) >>> 4;
            int optionLength = firstByte & 0x0F;

            if (optionDelta == 13) {
                optionDelta = 13 + byteBuf.readByte() & 0xFF;
            } else if (optionDelta == 14) {
                optionDelta = 269 + ((byteBuf.readByte() & 0xFF) << 8)
                        + (byteBuf.readByte() & 0xFF);
            }

            if (optionLength == 13) {
                optionLength = 13 + byteBuf.readByte() & 0xFF;
            } else if (optionLength == 14) {
                optionLength = 269 + ((byteBuf.readByte() & 0xFF) << 8)
                        + (byteBuf.readByte() & 0xFF);
            }

            int curOptionNum = preOptionNum + optionDelta;
            byte[] optionValue = new byte[optionLength];
            byteBuf.readBytes(optionValue);

            coapMessage.addOption(curOptionNum, optionValue);

            preOptionNum = curOptionNum;
            if (maxLength > byteBuf.readerIndex() - startPos) {
                firstByte = byteBuf.readByte() & 0xFF;
            } else {
                break;
            }
        }

        // return option length
        return byteBuf.readerIndex() - startPos;
    }
}
