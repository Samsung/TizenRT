package org.iotivity.cloud.accountserver.resources.acl.group.Authorization;

import java.util.Arrays;
import java.util.List;

/**
 * This file contains the declaration of member's authorization
 *
 */
public class MemberAuthorization extends UserAuthorization {
    private List<String> memberAddList     = Arrays.asList(DEVICES, RESOURCES);

    private List<String> memberReplaceList = Arrays.asList(GNAME);

    private List<String> memberDeleteList  = Arrays.asList(DEVICES, RESOURCES,
            MEMBERS);

    public MemberAuthorization() {
        super.addList.addAll(memberAddList);
        super.replaceList.addAll(memberReplaceList);
        super.deleteList.addAll(memberDeleteList);
    }
}
