/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#ifndef CLOUD_AUTH_H
#define CLOUD_AUTH_H

/**
 * Sends Sign UP request to cloud
 *
 * @param[in] endPoint         peer endpoint
 * @param[in] authProvider     authentication provider
 * @param[in] authToken        authentication token
 * @param[in] response         response callback
 * @return  OCStackResult application result
 */
OCStackResult CloudSignUp(const OCDevAddr  *endPoint,
                          const char *authProvider,
                          const char *authToken);

/**
 * Sends Sign IN request to cloud
 *
 * @param[in] endPoint         peer endpoint
 * @param[in] response         response callback
 * @return  OCStackResult application result
 */
OCStackResult CloudSignIn(const OCDevAddr  *endPoint);

/**
 * Sends Sign OUT request to cloud
 *
 * @param[in] endPoint         peer endpoint
 * @param[in] response         response callback
 * @return  OCStackResult application result
 */
OCStackResult CloudSignOut(const OCDevAddr  *endPoint);

#endif // CLOUD_AUTH_H
