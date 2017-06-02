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
package org.iotivity.cloud.accountserver.db;

public class InviteTable {

    String gid;
    String inviteUser;
    String invitedUser;

    public InviteTable() {
    }

    /**
     * API to initialize an instance of the group invitation table
     * 
     * @param inviteUser
     *            user ID who requests the group invitation for the
     *            "invitedUser"
     * @param gid
     *            group ID to be invited
     * @param invitedUser
     *            invited user ID
     */
    public InviteTable(String inviteUser, String gid, String invitedUser) {
        this.inviteUser = inviteUser;
        this.gid = gid;
        this.invitedUser = invitedUser;
    }

    /**
     * API to get the group ID to be invited
     * 
     * @return group ID
     */
    public String getGid() {
        return gid;
    }

    /**
     * API to set the group ID to invite
     * 
     * @param gid
     *            group ID
     */
    public void setGid(String gid) {
        this.gid = gid;
    }

    /**
     * API to get the user ID who requests the group invitation
     * 
     * @return user ID
     */
    public String getInviteUser() {
        return inviteUser;
    }

    /**
     * API to set the user ID to be invited
     * 
     * @param inviteUser
     *            user ID to be invited
     */
    public void setInviteUser(String inviteUser) {
        this.inviteUser = inviteUser;
    }

    /**
     * API to get the user Id to be invited
     * 
     * @return user ID to be invited
     */
    public String getInvitedUser() {
        return invitedUser;
    }

    public void setInvitedUser(String invitedUser) {
        this.invitedUser = invitedUser;
    }

}