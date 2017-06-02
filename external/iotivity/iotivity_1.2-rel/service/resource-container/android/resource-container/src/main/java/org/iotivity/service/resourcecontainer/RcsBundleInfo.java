//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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


/**
 * @file
 * This file contains RCSBundleInfo class, which provides APIs related to Bundle information.
 */
package org.iotivity.service.resourcecontainer;

/**
 * This class provides APIs for getting and setting the Bundle Information
 */
public class RcsBundleInfo {
    private final String mId;
    private final String mPath;
    private final String mActivatorName;
    private final String mLibraryPath;
    private final String mVersion;
    private boolean mActivated; // not final since it might be modified for Android-specific bundles

    private RcsBundleInfo(String id, String path, String activatorName,
            String libraryPath, String version) {
        mId = id;
        mPath = path;
        mActivatorName = activatorName;
        mLibraryPath = libraryPath;
        mVersion = version;
        mActivated = false;
    }

    private RcsBundleInfo(String id, String path, String activatorName,
            String libraryPath, String version, boolean activated) {
        mId = id;
        mPath = path;
        mActivatorName = activatorName;
        mLibraryPath = libraryPath;
        mVersion = version;
        mActivated = activated;
    }

    /**
     * API for getting the Id of the bundle
     *
     * @return string - Id of the bundle
     *
     */
    public String getID() {
        return mId;
    }

    /**
     * API for getting the path of the bundle
     *
     * @return path - path of the bundle
     *
     */
    public String getPath() {
        return mPath;
    }

    /**
     * API for setting the Activator name for the bundle
     *
     * @return string - Name of the activator
     *
     */
    public String getActivatorName() {
        return mActivatorName;
    }

    /**
     * API for getting the version of the bundle
     *
     * @return string - version of the bundle
     *
     */
    public String getVersion() {
        return mVersion;
    }
    
    /**
     * Returns the current activation status of the bundle
     *
     * @return boolean - bundle has been successfully loaded and started
     *
     */
    public boolean isActivated() {
        return mActivated;
    }

    /**
     * Set the current activation status of the bundle
     *
     * @return boolean - bundle has been successfully loaded and started
     *
     */
    protected void setActivated(boolean activated) {
        mActivated = activated;
    }
}
