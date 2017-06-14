/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
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
 *
 ******************************************************************/

#ifndef JNI_SHARED_OBJECT_HOLDER_H_
#define JNI_SHARED_OBJECT_HOLDER_H_

#include <memory>

template <typename T>
class JniSharedObjectHolder
{
    public:
        std::shared_ptr<T> get()
        {
            return m_sharedObject;
        }

        static JniSharedObjectHolder *create(std::shared_ptr<T> &sharedObject)
        {
            return new JniSharedObjectHolder(sharedObject);
        }

    private:
        JniSharedObjectHolder(std::shared_ptr<T> &sharedObject)
        {
            m_sharedObject = sharedObject;
        }

        std::shared_ptr<T> m_sharedObject;
};

#endif
