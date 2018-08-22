/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __CERTDATA_H__
#define __CERTDATA_H__

extern const unsigned char root_ca_pem[];
extern const int rootCaLen;

extern const unsigned char client_cert_pem[];
extern const int clientCertLen;

extern const unsigned char client_private_key_pem[];
extern const int clientPrivateKeyLen;

#endif /* __CERTDATA_H__ */
