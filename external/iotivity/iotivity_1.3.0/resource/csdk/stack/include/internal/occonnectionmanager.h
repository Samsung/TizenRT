/******************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/**
 * Initialize the Connection Manager and set IP ConnectionStatus callback
 *
 * @return ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult OCCMInitialize();

/**
 * Terminate the Connection Manager
 *
 * @return ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult OCCMTerminate();

/**
 *  Discover Resources by Connection Manager
 *
 * @param[in]  clientResponse   OCClientResponse struct pointer.
 * @return ::OC_STACK_OK or Appropriate error code.
 */
OCStackResult OCCMDiscoveryResource(OCClientResponse *clientResponse);
