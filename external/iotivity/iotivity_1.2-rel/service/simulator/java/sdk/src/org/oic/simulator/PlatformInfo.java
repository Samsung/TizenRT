/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.oic.simulator;

/**
 * This class contains remote device platform information and provide APIs to
 * access and update it.
 */
public class PlatformInfo {
    private String mPlatformId;
    private String mManufacturerName;
    private String mManufacturerUrl;
    private String mModelNumber;
    private String mDateOfManufacture;
    private String mPlatformVersion;
    private String mOperationSystemVersion;
    private String mHardwareVersion;
    private String mFirmwareVersion;
    private String mSupportUrl;
    private String mSystemTime;

    /**
     * This method is for getting platform id.
     *
     * @return Platform id.
     */
    public String getPlatformID() {
        return mPlatformId;
    }

    /**
     * This method is for setting platform id.
     *
     * @param mPlatformId
     *            Platform identifier.
     */
    public void setPlatformID(String mPlatformId) {
        this.mPlatformId = mPlatformId;
    }

    /**
     * This method is for getting manufacturer name.
     *
     * @return Manufacturer name.
     */
    public String getManufacturerName() {
        return mManufacturerName;
    }

    /**
     * This method is for setting manufacturer name.
     *
     * @param m_manufacturerName
     *            Manufacturer name.
     */
    public void setManufacturerName(String m_manufacturerName) {
        this.mManufacturerName = m_manufacturerName;
    }

    /**
     * This method is for getting manufacturer URL.
     *
     * @return Manufacturer URL.
     */
    public String getManufacturerUrl() {
        return mManufacturerUrl;
    }

    /**
     * This method is for setting manufacturer URL.
     *
     * @param m_manufacturerUrl
     *            Manufacturer URL.
     */
    public void setManufacturerUrl(String m_manufacturerUrl) {
        this.mManufacturerUrl = m_manufacturerUrl;
    }

    /**
     * This method is for getting model number.
     *
     * @return Model number.
     */
    public String getModelNumber() {
        return mModelNumber;
    }

    /**
     * This method is for setting model number.
     *
     * @param m_modelNumber
     *            Model number.
     */
    public void setModelNumber(String m_modelNumber) {
        this.mModelNumber = m_modelNumber;
    }

    /**
     * This method is for getting date of manufacture.
     *
     * @return Date of manufacture.
     */
    public String getDateOfManufacture() {
        return mDateOfManufacture;
    }

    /**
     * This method is for setting date of manufacture.
     *
     * @param m_dateOfManufacture
     *            Date of manufacture.
     */
    public void setDateOfManufacture(String m_dateOfManufacture) {
        this.mDateOfManufacture = m_dateOfManufacture;
    }

    /**
     * This method is for getting platform version.
     *
     * @return Platform version.
     */
    public String getPlatformVersion() {
        return mPlatformVersion;
    }

    /**
     * This method is for setting platform version.
     *
     * @param m_platformVersion
     *            Platform version.
     */
    public void setPlatformVersion(String m_platformVersion) {
        this.mPlatformVersion = m_platformVersion;
    }

    /**
     * This method is for getting operating system version.
     *
     * @return Operation system version.
     */
    public String getOperationSystemVersion() {
        return mOperationSystemVersion;
    }

    /**
     * This method is for setting operating system version.
     *
     * @param m_operationSystemVersion
     *            Operation system version.
     */
    public void setOperationSystemVersion(String m_operationSystemVersion) {
        this.mOperationSystemVersion = m_operationSystemVersion;
    }

    /**
     * This method is for getting hardware version.
     *
     * @return Hardware version.
     */
    public String getHardwareVersion() {
        return mHardwareVersion;
    }

    /**
     * This method is for setting hardware version.
     *
     * @param m_hardwareVersion
     *            Hardware version.
     */
    public void setHardwareVersion(String m_hardwareVersion) {
        this.mHardwareVersion = m_hardwareVersion;
    }

    /**
     * This method is for getting firmware version.
     *
     * @return Firmware version.
     */
    public String getFirmwareVersion() {
        return mFirmwareVersion;
    }

    /**
     * This method is for setting firmware version.
     *
     * @param m_firmwareVersion
     *            Firmware version.
     */
    public void setFirmwareVersion(String m_firmwareVersion) {
        this.mFirmwareVersion = m_firmwareVersion;
    }

    /**
     * This method is for getting support link URL.
     *
     * @return URL of support link.
     */
    public String getSupportUrl() {
        return mSupportUrl;
    }

    /**
     * This method is for setting support link URL.
     *
     * @param m_supportUrl
     *            URL of support link.
     */
    public void setSupportUrl(String m_supportUrl) {
        this.mSupportUrl = m_supportUrl;
    }

    /**
     * This method is for getting system time.
     *
     * @return System time.
     */
    public String getSystemTime() {
        return mSystemTime;
    }

    /**
     * This method is for setting system time.
     *
     * @param m_systemTime
     *            System time.
     */
    public void setSystemTime(String m_systemTime) {
        this.mSystemTime = m_systemTime;
    }
}
