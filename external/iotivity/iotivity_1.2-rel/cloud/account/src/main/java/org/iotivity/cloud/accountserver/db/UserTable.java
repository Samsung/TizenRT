package org.iotivity.cloud.accountserver.db;

/**
 * This class provides a set of APIs to store user information of authorized
 * users.
 *
 */
public class UserTable {
    private String uuid     = null;
    private String userid   = null;
    private String provider = null;
    private String email    = null;
    private String phone    = null;

    /**
     * API to get user ID
     * 
     * @return user ID as a form of UUID
     */
    public String getUuid() {
        return uuid;
    }

    /**
     * API to set user ID
     * 
     * @param uuid
     *            user ID to be registered as a form of UUID
     */
    public void setUuid(Object uuid) {
        if (uuid != null)
            this.uuid = uuid.toString();
    }

    /**
     * API to get user ID
     * 
     * @return user ID
     */
    public String getUserid() {
        return userid;
    }

    /**
     * API to set user ID
     * 
     * @param userid
     *            user ID to be registered
     */
    public void setUserid(Object userid) {
        if (userid != null)
            this.userid = userid.toString();
    }

    /**
     * API to get the auth provider name
     * 
     * @return auth provider name
     */
    public String getProvider() {
        return provider;
    }

    /**
     * API to set the auth provider name to the DB
     * 
     * @param provider
     *            auth provider name to be registered
     */
    public void setProvider(Object provider) {
        if (provider != null)
            this.provider = provider.toString();
    }

    /**
     * API to get the e-mail address of the user
     * 
     * @return e-mail address
     */
    public String getEmail() {
        return email;
    }

    /**
     * API to set the e-mail address of the user
     * 
     * @param email
     *            e-mail address to be registered
     */
    public void setEmail(Object email) {
        if (email != null)
            this.email = email.toString();
    }

    /**
     * API to get the phone number of the user
     * 
     * @return phone number
     */
    public String getPhone() {
        return phone;
    }

    /**
     * API to set the phone number of the user
     * 
     * @param phone
     *            phone number to be registered
     */
    public void setPhone(Object phone) {
        if (phone != null)
            this.phone = phone.toString();
    }

}
