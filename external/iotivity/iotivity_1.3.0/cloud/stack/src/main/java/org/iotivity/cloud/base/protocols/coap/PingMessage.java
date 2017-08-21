package org.iotivity.cloud.base.protocols.coap;

import org.iotivity.cloud.base.OICConstants;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;

public final class PingMessage extends CoapRequest {
    public PingMessage() {
        super(RequestMethod.PUT);
        setUriPath(OICConstants.KEEP_ALIVE_FULL_URI);
    }

    public static PingMessage build() {
        return new PingMessage();
    }
}

