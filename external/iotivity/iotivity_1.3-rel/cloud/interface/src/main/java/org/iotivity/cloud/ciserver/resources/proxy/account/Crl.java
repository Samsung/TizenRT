package org.iotivity.cloud.ciserver.resources.proxy.account;

import java.util.Arrays;

import org.iotivity.cloud.base.connector.ConnectorPool;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.device.IRequestChannel;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.ciserver.Constants;

public class Crl extends Resource {

    public Crl() {
        super(Arrays.asList(Constants.PREFIX_OIC,
                Constants.CREDPROV_URI, Constants.REQ_CRL));
    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        // Token exchange is done by CoapClient
        ConnectorPool.getConnection("account").sendRequest(request, srcDevice);
    }
}