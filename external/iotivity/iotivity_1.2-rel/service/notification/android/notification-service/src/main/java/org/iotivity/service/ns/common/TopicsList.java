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

import android.util.Log;
import java.util.Vector;
import java.util.Iterator;

/**
 *
 * This class provides implementation of Topics List.
 *
 */
public class TopicsList {

    private static final String LOG_TAG     = "NotificationService_TopicList";

    private Vector<Topic>        mTopicsList = new Vector<Topic>();

    public void addTopic(String topicname, Topic.TopicState state) {
        mTopicsList.add(new Topic(topicname, state));
    }

    public void removeTopic(String topicName) {
        Iterator<Topic> it = getTopicsList().iterator();
        while (it.hasNext()) {
            Topic element = it.next();
            if (element.getTopicName().equals(topicName)) {
                mTopicsList.remove(element);
            }
        }
    }

    public Vector<Topic> getTopicsList() {
        return mTopicsList;
    }

}
