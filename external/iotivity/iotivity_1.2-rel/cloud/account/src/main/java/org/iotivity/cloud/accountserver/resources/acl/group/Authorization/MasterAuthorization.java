package org.iotivity.cloud.accountserver.resources.acl.group.Authorization;

import java.util.Arrays;
import java.util.List;

/**
 * This file contains the declaration of masters's authorization
 *
 */
public class MasterAuthorization extends UserAuthorization {
    private List<String> masterAddList     = Arrays.asList(GROUP, DEVICES,
            RESOURCES, MASTERS, MEMBERS);

    private List<String> masterReplaceList = Arrays.asList(GNAME);

    private List<String> masterDeleteList  = Arrays.asList(DEVICES, RESOURCES,
            MASTERS, MEMBERS);

    public MasterAuthorization() {
        super.addList.addAll(masterAddList);
        super.replaceList.addAll(masterReplaceList);
        super.deleteList.addAll(masterDeleteList);
    }
}
