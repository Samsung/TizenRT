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

package oic.simulator.serviceprovider.model;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.oic.simulator.server.SimulatorResource.AutoUpdateType;

import oic.simulator.serviceprovider.utils.Constants;

/**
 * This is a helper class for providing the automation settings information to
 * UI.
 */
public class AutomationSettingHelper {
    private String       settingID;
    private String       settingValue;
    private List<String> allowedValues;

    public String getSettingID() {
        return settingID;
    }

    public void setSettingID(String settingID) {
        this.settingID = settingID;
    }

    public String getSettingValue() {
        return settingValue;
    }

    public void setSettingValue(String settingValue) {
        this.settingValue = settingValue;
    }

    public List<String> getAllowedValues() {
        return allowedValues;
    }

    public void setAllowedValues(List<String> allowedValues) {
        this.allowedValues = allowedValues;
    }

    public void addAllowedValue(String newText) {
        if (null != allowedValues) {
            allowedValues.add(newText);
        }
    }

    public static List<AutomationSettingHelper> getAutomationSettings(
            AttributeElement attribute) {
        boolean invalidSetting;
        List<AutomationSettingHelper> settingList = new ArrayList<AutomationSettingHelper>();
        for (int count = 0; count < Constants.AUTOMATION_SETTINGS_COUNT; count++) {
            invalidSetting = false;
            AutomationSettingHelper setting = new AutomationSettingHelper();
            if (Constants.AUTOMATION_SETTINGS[count]
                    .equals(Constants.AUTOMATION_TYPE)) {
                setting.setSettingID(Constants.AUTOMATION_TYPE);
                if (null != attribute) {
                    setting.setSettingValue(attribute.getAutoUpdateType()
                            .toString());
                } else {
                    setting.setSettingValue(AutoUpdateType.ONE_TIME.toString());
                }
                List<String> valueList = new ArrayList<String>();
                valueList.add(AutoUpdateType.ONE_TIME.toString());
                valueList.add(AutoUpdateType.REPEAT.toString());
                setting.setAllowedValues(valueList);
            } else if (Constants.AUTOMATION_SETTINGS[count]
                    .equals(Constants.UPDATE_INTERVAL_IN_MS)) {
                setting.setSettingID(Constants.UPDATE_INTERVAL_IN_MS);
                if (null != attribute) {
                    setting.setSettingValue(String.valueOf(attribute
                            .getAutoUpdateInterval()));
                } else {
                    setting.setSettingValue(String
                            .valueOf(Constants.DEFAULT_AUTOMATION_INTERVAL));
                }
                List<String> valueList = new ArrayList<String>();
                for (int index = 1; index <= 10; index++) {
                    valueList.add(String.valueOf(index * 500));
                }
                setting.setAllowedValues(valueList);
            } else {
                invalidSetting = true;
            }
            if (!invalidSetting) {
                settingList.add(setting);
            }
        }
        return settingList;
    }

    public static void updateAutomationStatus(
            List<AutomationSettingHelper> localSettingList, String status) {
        if (null != localSettingList && null != status) {
            Iterator<AutomationSettingHelper> settingItr = localSettingList
                    .iterator();
            AutomationSettingHelper setting;
            while (settingItr.hasNext()) {
                setting = settingItr.next();
                if (null != setting) {
                    if (setting.getSettingID().equals(Constants.AUTOMATION)) {
                        setting.setSettingValue(status);
                        break;
                    }
                }
            }
        }
    }
}