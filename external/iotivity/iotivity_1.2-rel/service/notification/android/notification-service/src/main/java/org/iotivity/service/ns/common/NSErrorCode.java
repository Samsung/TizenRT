//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

package org.iotivity.service.ns.common;

/**
 *
 * This enum provides details of error code messages thrown in NSException.
 *
 */
public enum NSErrorCode {
    OK("OK", ""),
    ERROR("ERROR", ""),
    SUCCESS("SUCCESS", ""),
    FAIL("FAIL", ""),
    ALLOW("ALLOW", ""),
    DENY("DENY", ""),
    JNI_EXCEPTION("JNI_EXCEPTION", "Generic Java binder error"),
    JNI_NO_NATIVE_OBJECT("JNI_NO_NATIVE_POINTER", ""),
    JNI_INVALID_VALUE("JNI_INVALID_VALUE", ""),
    NATIVE_EXCEPTION("NATIVE_EXCEPTION", "");

    private String error;
    private String description;

    private NSErrorCode(String error, String description) {
        this.error = error;
        this.description = description;
    }

    public String getError() {
        return error;
    }

    public String getDescription() {
        return description;
    }

    public static NSErrorCode get(String errorCode) {
        for (NSErrorCode eCode : NSErrorCode.values()) {
            if (eCode.getError().equals(errorCode)) {
                return eCode;
            }
        }
        throw new IllegalArgumentException("Unexpected NSErrorCode value");
    }

    @Override
    public String toString() {
        return error + (description.isEmpty() ? "" : " : " + description);
    }
}
