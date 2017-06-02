package org.iotivity.cloud.accountserver.resources.acl.group.Authorization;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.resources.acl.group.UserOperation;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;

/**
 * This class provides a set of API to handle user's authorization
 *
 */
public class UserAuthorization {

    static final String GROUP       = Constants.KEYFIELD_GROUP;
    static final String DEVICES     = Constants.KEYFIELD_GROUP_DEVICES;
    static final String RESOURCES   = Constants.KEYFIELD_GROUP_RESOURCES;
    static final String MASTERS     = Constants.KEYFIELD_GROUP_MASTERS;
    static final String MEMBERS     = Constants.KEYFIELD_GROUP_MEMBERS;
    static final String OWNER       = Constants.KEYFIELD_GROUP_OWNER;
    static final String GNAME       = Constants.KEYFIELD_GROUP_NAME;

    public List<String> addList     = new ArrayList<String>();
    public List<String> replaceList = new ArrayList<String>();
    public List<String> deleteList  = new ArrayList<String>();
    public List<String> properties  = new ArrayList<String>();

    public UserAuthorization() {
        properties.addAll(Arrays.asList(GROUP, GNAME, MASTERS, MEMBERS, OWNER,
                DEVICES, RESOURCES));
    }

    public List<String> getProperties() {
        return properties;
    }

    public List<String> getUserAuthz(UserOperation operation) {
        switch (operation) {
            case ADD:
                return addList;
            case REPLACE:
                return replaceList;
            case DELETE:
                return deleteList;
            default:
                throw new BadRequestException("operation type is not support");
        }
    }
}
