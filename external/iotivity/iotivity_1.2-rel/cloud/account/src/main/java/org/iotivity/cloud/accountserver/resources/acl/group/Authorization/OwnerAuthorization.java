package org.iotivity.cloud.accountserver.resources.acl.group.Authorization;

import java.util.Arrays;
import java.util.List;

/**
 * This file contains the declaration of owner's authorization
 *
 */
public class OwnerAuthorization extends UserAuthorization {

    private List<String> ownerAddList     = Arrays.asList(GROUP, DEVICES,
            RESOURCES, MASTERS, MEMBERS);

    private List<String> ownerReplaceList = Arrays.asList(OWNER, GNAME);

    private List<String> ownerDeleteList  = Arrays.asList(GROUP, DEVICES,
            RESOURCES, MASTERS, MEMBERS);

    public OwnerAuthorization() {
        super.addList.addAll(ownerAddList);
        super.replaceList.addAll(ownerReplaceList);
        super.deleteList.addAll(ownerDeleteList);
    }
}
