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

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

public class CoapEncoder extends MessageToByteEncoder<CoapMessage> {

    private boolean isboolWebSocket = false;

    @Override
    protected void encode(ChannelHandlerContext ctx, CoapMessage msg,
            ByteBuf out) throws Exception {

        CoapMessage coapMessage = (CoapMessage) msg;

        ByteBuf optBuf = Unpooled.directBuffer();

        /**
         * encode options
         */
        encodeOptions(optBuf, coapMessage);

        long length = optBuf.readableBytes();

        if (coapMessage.getPayloadSize() > 0) {
            // + 1 means 8bits delimiter
            length += 1 + coapMessage.getPayloadSize();
        }

        if (isboolWebSocket) {
            length = 0;
        }

        calcShimHeader(coapMessage, out, length);

        out.writeByte(coapMessage.getCode());

        if (coapMessage.getTokenLength() > 0) {
            out.writeBytes(coapMessage.getToken());
        }

        // Write option
        if (optBuf.readableBytes() > 0) {
            out.writeBytes(optBuf);
        }

        optBuf.release();

        if (coapMessage.getPayloadSize() > 0) {
            out.writeByte(255);
            out.writeBytes(coapMessage.getPayload());
        }
    }

    public void encode(CoapMessage msg, ByteBuf out, boolean isWebsocket)
            throws Exception {
        isboolWebSocket = isWebsocket;
        encode(null, msg, out);
    }

    private void calcShimHeader(CoapMessage coapMessage, ByteBuf byteBuf,
            long length) {
        if (length < 13) {
            byteBuf.writeByte(((int) length & 0x0F) << 4
                    | (coapMessage.getTokenLength() & 0x0F));
        } else if (length < 269) {
            byteBuf.writeShort((13 & 0x0F) << 12
                    | (coapMessage.getTokenLength() & 0x0F) << 8
                    | (((int) length - 13) & 0xFF));
        } else if (length < 65805) {
            byteBuf.writeByte(
                    (14 & 0x0F) << 4 | (coapMessage.getTokenLength() & 0x0F));
            byteBuf.writeShort(((int) length - 269) & 0xFFFF);
        } else if (length < 4294967294L) {
            byteBuf.writeByte(
                    (15 & 0x0F) << 4 | (coapMessage.getTokenLength() & 0x0F));
            byte[] size = new byte[4];
            long payload = length - 65805;
            for (int i = 3; i > -1; i--) {
                size[i] = (byte) (payload & 0xFF);
                payload >>= 8;
            }
            byteBuf.writeBytes(size);
        } else {
            throw new IllegalArgumentException(
                    "Length must be less than 4GB " + length);
        }
    }

    private void encodeOptions(ByteBuf byteBuf, CoapMessage coapMessage)
            throws Exception {
        int preOptionNum = 0;

        for (CoapOption opt : CoapOption.values()) {
            int optionNum = opt.getvalue();
            List<byte[]> values = coapMessage.getOption(optionNum);
            if (values != null) {
                if (values.size() > 0) {
                    for (byte[] value : values) {
                        writeOption(optionNum - preOptionNum,
                                value != null ? value.length : 0, byteBuf,
                                value);
                        preOptionNum = optionNum;
                    }

                } else {
                    writeOption(optionNum - preOptionNum, 0, byteBuf, null);
                    preOptionNum = optionNum;
                }
            }
        }
    }

    private void writeOption(int optionDelta, int optionLength, ByteBuf byteBuf,
            byte[] value) {

        if (optionDelta < 13) {
            if (optionLength < 13) {
                byteBuf.writeByte(
                        ((optionDelta & 0xFF) << 4) | (optionLength & 0xFF));
            } else if (optionLength < 269) {
                byteBuf.writeByte(((optionDelta << 4) & 0xFF) | (13 & 0xFF));
                byteBuf.writeByte((optionLength - 13) & 0xFF);
            } else {
                byteBuf.writeByte(((optionDelta << 4) & 0xFF) | (14 & 0xFF));
                byteBuf.writeByte(((optionLength - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionLength - 269) & 0xFF);
            }
        }

        else if (optionDelta < 269) {
            if (optionLength < 13) {
                byteBuf.writeByte(((13 & 0xFF) << 4) | (optionLength & 0xFF));
                byteBuf.writeByte((optionDelta - 13) & 0xFF);
            } else if (optionLength < 269) {
                byteBuf.writeByte(((13 & 0xFF) << 4) | (13 & 0xFF));
                byteBuf.writeByte((optionDelta - 13) & 0xFF);
                byteBuf.writeByte((optionLength - 13) & 0xFF);
            } else {
                byteBuf.writeByte((13 & 0xFF) << 4 | (14 & 0xFF));
                byteBuf.writeByte((optionDelta - 13) & 0xFF);
                byteBuf.writeByte(((optionLength - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionLength - 269) & 0xFF);
            }
        }

        else if (optionDelta < 65805) {

            if (optionLength < 13) {
                byteBuf.writeByte(((14 & 0xFF) << 4) | (optionLength & 0xFF));
                byteBuf.writeByte(((optionDelta - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionDelta - 269) & 0xFF);
            }

            else if (optionLength < 269) {
                byteBuf.writeByte(((14 & 0xFF) << 4) | (13 & 0xFF));
                byteBuf.writeByte(((optionDelta - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionDelta - 269) & 0xFF);
                byteBuf.writeByte((optionLength - 13) & 0xFF);
            }

            else {
                byteBuf.writeByte(((14 & 0xFF) << 4) | (14 & 0xFF));
                byteBuf.writeByte(((optionDelta - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionDelta - 269) & 0xFF);
                byteBuf.writeByte(((optionLength - 269) & 0xFF00) >>> 8);
                byteBuf.writeByte((optionLength - 269) & 0xFF);
            }
        } else {
            throw new IllegalArgumentException(
                    "Unsupported option delta " + optionDelta);
        }

        if (optionLength > 0) {
            byteBuf.writeBytes(value);
        }
    }
}
