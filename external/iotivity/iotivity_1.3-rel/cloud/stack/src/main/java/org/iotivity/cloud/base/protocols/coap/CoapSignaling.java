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

import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.iotivity.cloud.base.exception.ServerException.BadOptionException;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.protocols.enums.SignalingMethod;

public class CoapSignaling extends CoapMessage {
    private SignalingMethod mSignalingMethod;

    // Option fields (for CSM signaling)
    protected byte[]        server_name         = null;  // CSM option 1
    protected byte[]        max_message_size    = null;  // CSM option 2
    protected boolean       block_wise_tranfer  = false; // CSM option 4

    // Option fields (for PING signaling and PONG signaling)
    protected boolean       custody             = false; // PING / PONG option 2

    // Option fields (for Release signaling)
    protected boolean       bad_server_name     = false; // Release option 2
    protected byte[]        alternative_address = null;  // Release option 4
    protected byte[]        hold_off            = null;  // Release option 6

    // Option fields (for Abort signaling)
    protected byte[]        bad_csm_option      = null;  // Abort option 2

    public CoapSignaling(SignalingMethod signalingMethod) {
        mSignalingMethod = signalingMethod;
    }

    public CoapSignaling(int code) {
        switch (code) {
            case 225: // code 7.01
                mSignalingMethod = SignalingMethod.CSM;
                break;
            case 226: // code 7.02
                mSignalingMethod = SignalingMethod.PING;
                break;
            case 227: // code 7.03
                mSignalingMethod = SignalingMethod.PONG;
                break;
            case 228: // code 7.04
                mSignalingMethod = SignalingMethod.RELEASE;
                break;
            case 229: // code 7.05
                mSignalingMethod = SignalingMethod.ABORT;
                break;
            default:
                throw new IllegalArgumentException(
                        "Unsupported CoAP signaling code " + code);
        }
    }

    @Override
    public int getCode() {
        switch (mSignalingMethod) {
            case CSM: // code 7.01
                return 225;
            case PING: // code 7.02
                return 226;
            case PONG: // code 7.03
                return 227;
            case RELEASE: // code 7.04
                return 228;
            case ABORT: // code 7.05
                return 229;
            default:
                break;
        }
        return 0;
    }

    public SignalingMethod getSignalingMethod() {
        return mSignalingMethod;
    }

    @Override
    public void addOption(int optnum, byte[] value) {
        switch (mSignalingMethod) {
            case CSM: // code 7.01
                addCsmOption(optnum, value);
                break;
            case PING: // code 7.02
            case PONG: // code 7.03
                addPingPongOption(optnum, value);
                break;
            case RELEASE: // code 7.04
                addReleaseOption(optnum, value);
                break;
            case ABORT: // code 7.05
                addAbortOption(optnum, value);
                break;
            default:
                break;
        }
    }

    private void addCsmOption(int optnum, byte[] value) {
        switch (optnum) {
            // SERVER_NAME
            case 1:
                server_name = value;
                break;
            // MAX_MESSAGE_SIZE
            case 2:
                max_message_size = value;
                break;
            // BLOCK_WISE_TRANSFER
            case 4:
                block_wise_tranfer = true;
                break;
            default: {
                if (optnum % 2 == 1) {
                    throw new BadOptionException(
                            "unrecognized critical option # " + optnum
                                    + " received");
                }
            }
        }
    }

    private void addPingPongOption(int optnum, byte[] value) {
        switch (optnum) {
            // CUSTODY
            case 2:
                custody = true;
                break;
            default: {
                if (optnum % 2 == 1) {
                    throw new BadOptionException(
                            "unrecognized critical option # " + optnum
                                    + " received");
                }
            }
        }
    }

    private void addReleaseOption(int optnum, byte[] value) {
        switch (optnum) {
            // BAD_SERVER_NAME
            case 2:
                bad_server_name = true;
                break;
            // ALTERNATIVE_ADDRESS
            case 4:
                alternative_address = value;
                break;
            // HOLD_OFF
            case 6:
                hold_off = value;
                break;
            default: {
                if (optnum % 2 == 1) {
                    throw new BadOptionException(
                            "unrecognized critical option # " + optnum
                                    + " received");
                }
            }
        }
    }

    private void addAbortOption(int optnum, byte[] value) {
        switch (optnum) {
            // BAD_CSM_OPTION
            case 2:
                bad_csm_option = value;
                break;
            default: {
                if (optnum % 2 == 1) {
                    throw new BadOptionException(
                            "unrecognized critical option # " + optnum
                                    + " received");
                }
            }
        }
    }

    public void setCsmServerName(String serverName) {
        if (serverName != null && !serverName.isEmpty()) {
            addOption(1, serverName.getBytes(StandardCharsets.UTF_8));
        }
    }

    public void setCsmMaxMessageSize(long maxMessageSize) {
        if (maxMessageSize != 0) {
            ByteBuffer buf = ByteBuffer.wrap(new byte[4]);
            max_message_size = buf.putInt(0, (int) maxMessageSize).array();
        }
    }

    public void setCsmBlockWiseTransfer(boolean blockWiseTransferOption) {
        block_wise_tranfer = blockWiseTransferOption;
    }

    public String getCsmServerName() {
        if (server_name == null)
            return new String();
        return new String(server_name, Charset.forName("UTF-8"));
    }

    public long getCsmMaxMessageSize() {
        return unsignedIntToLong(max_message_size);
    }

    public boolean getCsmBlockWiseTransfer() {
        return block_wise_tranfer;
    }

    @Override
    public List<byte[]> getOption(int optnum) {
        switch (mSignalingMethod) {
            case CSM: // code 7.01
                return getCsmOption(optnum);
            case PING: // code 7.02
            case PONG: // code 7.03
                return getPingPongOption(optnum);
            case RELEASE: // code 7.04
                return getReleaseOption(optnum);
            case ABORT: // code 7.05
                return getAbortOption(optnum);
            default:
                break;
        }
        return null;
    }

    public List<byte[]> getCsmOption(int optnum) {
        switch (optnum) {
            // SERVER_NAME
            case 1:
                return server_name != null ? Arrays.asList(server_name) : null;
            // MAX_MESSAGE_SIZE
            case 2:
                return max_message_size != null
                        ? Arrays.asList(max_message_size) : null;
            // BLOCK_WISE_TRANSFER
            case 4:
                return block_wise_tranfer == true ? new ArrayList<byte[]>()
                        : null;
        }
        return null;
    }

    public List<byte[]> getPingPongOption(int optnum) {
        switch (optnum) {
            // CUSTODY
            case 2:
                return custody == true ? new ArrayList<byte[]>() : null;
        }
        return null;
    }

    public List<byte[]> getReleaseOption(int optnum) {
        switch (optnum) {
            // BAD_SERVER_NAME
            case 2:
                return bad_server_name == true ? new ArrayList<byte[]>() : null;
            // ALTERNATIVE_ADDRESS
            case 4:
                return alternative_address != null
                        ? Arrays.asList(alternative_address) : null;
            // HOLD_OFF
            case 6:
                return hold_off != null ? Arrays.asList(hold_off) : null;
        }
        return null;
    }

    public List<byte[]> getAbortOption(int optnum) {
        switch (optnum) {
            // BAD_CSM_OPTION
            case 2:
                return bad_csm_option != null ? Arrays.asList(bad_csm_option)
                        : null;
        }
        return null;
    }

    // This signal object does not support request status
    @Override
    public RequestMethod getMethod() {
        // TODO Auto-generated method stub
        return null;
    }

    // This signal object does not support response status
    @Override
    public ResponseStatus getStatus() {
        // TODO Auto-generated method stub
        return null;
    }

    private static final long unsignedIntToLong(byte[] b) {
        long value = 0L;
        if (b != null) {
            for (byte data : b) {
                value <<= 8;
                value += data & 0xFF;
            }
        }
        return value;
    }
}
