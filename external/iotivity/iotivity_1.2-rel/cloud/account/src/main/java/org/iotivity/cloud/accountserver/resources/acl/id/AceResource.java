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
package org.iotivity.cloud.accountserver.resources.acl.id;

import java.util.List;

/**
 *
 * This class holds resource of Ace.
 *
 */
public class AceResource {

    String href;
    List<String> rt;
    List<String> oicif;

    public AceResource() {

    }

    public AceResource(String href, List<String> rt, List<String> oicif) {
        this.href = href;
        this.rt = rt;
        this.oicif = oicif;
    }

    public String getHref() {
        return href;
    }

    public void setHref(String href) {
        this.href = href;
    }

    public List<String> getRt() {
        return rt;
    }

    public void setRt(List<String> rt) {
        this.rt = rt;
    }

    public List<String> getOicif() {
        return oicif;
    }

    public void setOicif(List<String> oicif) {
        this.oicif = oicif;
    }

}
