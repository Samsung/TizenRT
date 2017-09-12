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
package org.iotivity.cloud.accountserver.resources.credprov.crl;

import static org.iotivity.cloud.accountserver.resources.credprov.cert.CertificateConstants.BASE_64;
import static org.iotivity.cloud.accountserver.resources.credprov.crl.CrlManager.CRL_MANAGER;

import java.io.IOException;
import java.security.cert.CRLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.bouncycastle.operator.OperatorCreationException;
import org.bouncycastle.util.encoders.Base64;
import org.bouncycastle.util.encoders.DecoderException;
import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.base.device.Device;
import org.iotivity.cloud.base.exception.ServerException;
import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.IResponse;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.ResponseStatus;
import org.iotivity.cloud.base.resource.Resource;
import org.iotivity.cloud.util.Cbor;
import org.iotivity.cloud.util.Log;

/**
 * Class is used working with POST and GET requests and handles CRL requests.
 */
public class CrlResource extends Resource {

    /**
     * CBOR container with help of map presentation.
     */
    private static final Cbor<Map<String, Object>> MAP_CBOR = new Cbor<>();

    /**
     * Creates resource for handling CRL requests(GET and POST)
     */
    public CrlResource() {
        super(Arrays.asList(Constants.PREFIX_OIC, Constants.CREDPROV_URI,
                Constants.REQ_CRL));

    }

    @Override
    public void onDefaultRequestReceived(Device srcDevice, IRequest request)
            throws ServerException {
        IResponse response;
        switch (request.getMethod()) {
            case POST:
                response = handlePostRequest(request);
                break;
            case GET:
                response = handleGetRequest(request);
                break;
            default:
                response = MessageBuilder.createResponse(request,
                        ResponseStatus.BAD_REQUEST);
        }
        srcDevice.sendResponse(response);
    }

    /**
     * Method handles GET requests with specified format: GET
     * /oic/credprov/crl?lu=20170701000000 Checks if “lu” value is not after the
     * latest update. If so, response with the latest CRL, otherwise response
     * error (e.g. 4.04 Not Found) And response of next format: 2.05 CONTENTS {
     * “tu” : “20160711000000”, “nu” : “20161011000000”, “crl” : { “encoding” :
     * “oic.sec.encoding.base64”, “data” : “<Base64 encoded CRL Binary>” } }
     */
    private IResponse handleGetRequest(IRequest request)
            throws ServerException {
        HashMap<String, List<String>> queryData = request.getUriQueryMap();
        IResponse iResponse = MessageBuilder.createResponse(request,
                ResponseStatus.NOT_FOUND);
        if (queryData != null) {
            List<String> lastUpdateList = queryData
                    .get(Constants.REQ_LAST_UPDATE);
            if (lastUpdateList != null && !lastUpdateList.isEmpty()) {
                try {
                    Map<String, Object> payload = CRL_MANAGER
                            .getPayload(lastUpdateList.get(0));
                    if (!payload.isEmpty()) {
                        iResponse = MessageBuilder.createResponse(request,
                                ResponseStatus.CONTENT,
                                ContentFormat.APPLICATION_CBOR,
                                MAP_CBOR.encodingPayloadToCbor(payload));
                    }
                } catch (CRLException e) {
                    Log.e(e.getMessage());
                }
            }
        }
        return iResponse;
    }

    /**
     * Handles POST requests of next formats: POST /oic/credprov/crl { “tu” :
     * “20160727000000”, “nu” : “20161027000000”, “rcsn” : “123456” } AND POST
     * /oic/credprov/crl { “tu” : “20160727000000”, “nu” : “20161027000000”,
     * “crl” : { “encoding” : “oic.sec.encoding.base64”, “data” : “<Base64
     * encoded New CRL Binary>” } } And responds back with 2.04 CHANGED if
     * everything is ok, and PRECONDITION_FAILED - otherwise
     */
    private IResponse handlePostRequest(IRequest request)
            throws ServerException {
        byte[] requestPayload = request.getPayload();
        IResponse response = MessageBuilder.createResponse(request,
                ResponseStatus.PRECONDITION_FAILED);
        if (requestPayload != null) {
            Map<String, Object> payloadData = MAP_CBOR
                    .parsePayloadFromCbor(request.getPayload(), HashMap.class);
            if (payloadData != null) {
                Object thisUpdate = payloadData.get(Constants.REQ_THIS_UPDATE);
                Object nextUpdate = payloadData.get(Constants.REQ_NEXT_UPDATE);
                if (thisUpdate != null && thisUpdate instanceof String
                        && nextUpdate != null && nextUpdate instanceof String) {
                    Date thisUpdateDate;
                    try {
                        thisUpdateDate = new SimpleDateFormat("yyyyMMddHHmmss")
                                .parse(thisUpdate.toString());
                        Object reqSerialNumber = payloadData
                                .get(Constants.REQ_SERIAL_NUMBER);
                        Object crl = payloadData.get(Constants.REQ_CRL);
                        if (reqSerialNumber != null
                                && reqSerialNumber instanceof List) {
                            CRL_MANAGER.revoke(((List<String>) reqSerialNumber)
                                    .toArray(new String[] {}));
                            response = MessageBuilder.createResponse(request,
                                    ResponseStatus.CHANGED);
                        } else if (crl != null && crl instanceof Map) {
                            Object encoding = ((Map<String, Object>) crl)
                                    .get(Constants.ENCODING);
                            Object crlData = ((Map<String, Object>) crl)
                                    .get(Constants.DATA);
                            if (encoding != null && encoding instanceof String
                                    && crlData != null
                                    && crlData instanceof byte[]) {
                                try {
                                    if (encoding.equals(BASE_64)) {
                                        crlData = Base64
                                                .decode((byte[]) crlData);
                                    }
                                    CRL_MANAGER.update(thisUpdateDate,
                                            (byte[]) crlData);
                                    response = MessageBuilder.createResponse(
                                            request, ResponseStatus.CHANGED);
                                } catch (DecoderException e) {
                                    Log.e(e.getMessage() + e.getClass());
                                }
                            }
                        }
                    } catch (CRLException | IOException
                            | OperatorCreationException | ParseException e) {
                        Log.e(e.getMessage() + e.getClass());
                    }
                }
            }
        }
        return response;
    }
}
