package org.iotivity.cloud.accountserver.resources.acl.group;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.iotivity.cloud.accountserver.Constants;
import org.iotivity.cloud.accountserver.db.AccountDBManager;
import org.iotivity.cloud.accountserver.db.GroupTable;
import org.iotivity.cloud.accountserver.resources.acl.group.Authorization.MasterAuthorization;
import org.iotivity.cloud.accountserver.resources.acl.group.Authorization.MemberAuthorization;
import org.iotivity.cloud.accountserver.resources.acl.group.Authorization.OwnerAuthorization;
import org.iotivity.cloud.accountserver.resources.acl.group.Authorization.UserAuthorization;
import org.iotivity.cloud.accountserver.util.TypeCastingManager;
import org.iotivity.cloud.base.exception.ServerException.BadRequestException;

/**
 * This class provides a set of APIs to verify Authorization
 *
 */
public class GroupPolicyManager {

    private TypeCastingManager<GroupTable> mTypeGroup = new TypeCastingManager<>();

    /**
     * API to verify user's authorization
     * 
     * @param gid
     *            group id
     * @param mid
     *            user id
     * @param operation
     *            post message operation (add / delete)
     * @param properties
     *            payload properies
     */
    public void verifyOperationAuthorization(String gid, String mid,
            UserOperation operation, ArrayList<String> properties) {

        UserAuthorization userAuthz = getUserGrade(gid, mid);
        checkPayloadProperties(userAuthz, properties);
        checkOperationAuthz(userAuthz, operation, properties);
    }

    private UserAuthorization getUserGrade(String gid, String mid) {
        HashMap<String, Object> condition = new HashMap<>();
        condition.put(Constants.KEYFIELD_GID, gid);
        HashMap<String, Object> record = AccountDBManager.getInstance()
                .selectOneRecord(Constants.GROUP_TABLE, condition);

        if (record.isEmpty()) {
            throw new BadRequestException("gid doesn't exist");

        }
        GroupTable groupInfo = new GroupTable();
        mTypeGroup.convertMaptoObject(record, groupInfo);

        UserAuthorization userAuthz = null;
        if (groupInfo.getOwner() != null && groupInfo.getOwner().equals(mid)) {
            userAuthz = new OwnerAuthorization();
        }

        else if (groupInfo.getMasters() != null
                && groupInfo.getMasters().contains(mid)) {
            userAuthz = new MasterAuthorization();
        }

        else if (groupInfo.getMembers() != null
                && groupInfo.getMembers().contains(mid)) {
            userAuthz = new MemberAuthorization();
        }

        else {
            throw new BadRequestException("mid is not members");
        }

        return userAuthz;

    }

    private void checkPayloadProperties(UserAuthorization userAuthz,
            List<String> properties) {
        List<String> possibleProperties = userAuthz.getProperties();
        if (!possibleProperties.containsAll(properties)) {
            throw new BadRequestException("payload include invalid property");
        }

    }

    private void checkOperationAuthz(UserAuthorization userAuthz,
            UserOperation operation, List<String> properties) {
        List<String> opAuthz = userAuthz.getUserAuthz(operation);

        for (String property : properties) {
            if (!opAuthz.contains(property))
                throw new BadRequestException(
                        "user doesn't have authorization about '" + property
                                + "' property");
        }
    }
}
