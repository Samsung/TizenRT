package org.iotivity.cloud.testrdserver;

import java.util.ArrayList;
import java.util.HashMap;

import org.iotivity.cloud.base.protocols.IRequest;
import org.iotivity.cloud.base.protocols.MessageBuilder;
import org.iotivity.cloud.base.protocols.enums.ContentFormat;
import org.iotivity.cloud.base.protocols.enums.RequestMethod;
import org.iotivity.cloud.rdserver.Constants;
import org.iotivity.cloud.rdserver.db.MongoDB;
import org.iotivity.cloud.util.Cbor;

public class RDServerTestUtils {
    public static final String DI                 = "98f7483c-5a31-4161-ba7e-9c13e0d";
    public static final String RD_REQ_URI         = Constants.RD_FULL_URI;
    public static final String DISCOVERY_REQ_URI  = Constants.WELL_KNOWN_FULL_URI;
    public static final String DEVICE_PRS_REQ_URI = Constants.DEVICE_PRESENCE_FULL_URI;
    public static final String RES_PRS_URI        = Constants.RESOURCE_PRESENCE_FULL_URI;

    public static IRequest makePublishRequest() throws Exception {

        HashMap<Object, Object> payload = new HashMap<>();
        payload.put(Constants.DEVICE_ID, DI);
        ArrayList<HashMap<Object, Object>> publishLinks = new ArrayList<>();
        HashMap<Object, Object> link = new HashMap<>();
        link.put(Constants.HREF, "/a/light");
        ArrayList<String> rt = new ArrayList<String>();
        rt.add("core.light");
        ArrayList<String> itf = new ArrayList<String>();
        itf.add("oic.if.baseline");
        HashMap<String, Object> policy = new HashMap<>();
        policy.put(Constants.BITMAP, 5);
        link.put(Constants.RESOURCE_TYPE, rt);
        link.put(Constants.INTERFACE, itf);
        link.put(Constants.POLICY, policy);
        link.put(Constants.INS, 0);
        link.put(Constants.RESOURCE_TTL, 3000);
        publishLinks.add(link);
        payload.put(Constants.LINKS, publishLinks);
        Cbor<HashMap<Object, Object>> cbor = new Cbor<>();
        IRequest request = MessageBuilder.createRequest(RequestMethod.POST,
                Constants.RD_URI,
                null,
                ContentFormat.APPLICATION_CBOR,
                cbor.encodingPayloadToCbor(payload));
        return request;
    }

    public static void resetRDDatabase() throws Exception {
        MongoDB mongoDB = new MongoDB("127.0.0.1", Constants.RD_DB_NAME);
        mongoDB.createTable(Constants.RD_TABLE);
        mongoDB.createTable(Constants.PRESENCE_TABLE);
    }
}
