#!/bin/sh
###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

# File   : mkcertData.sh
# Description : make cert data file templete

WD=$(cd `dirname $0` && pwd)

CERTDATA_FILE=certData.c

#1 rootCA

tee ${CERTDATA_FILE} << __EOF__

/* root CA certificate file */

const unsigned char root_ca_pem[] =
__EOF__

while IFS='' read -r line
do
	echo '\t"'"$line"'\\r\\n"' >> ${CERTDATA_FILE}
done < "$1"

echo ';' >> ${CERTDATA_FILE}

tee -a ${CERTDATA_FILE} << __EOF__
const int rootCaLen = sizeof(root_ca_pem);
__EOF__

#2 cert.pem

tee -a ${CERTDATA_FILE} << __EOF__

/* cert.pem certificate file */

const unsigned char client_cert_pem[] =
__EOF__

while IFS='' read -r line
do
	echo '\t"'"$line"'\\r\\n"' >> ${CERTDATA_FILE}
done < "$2"

echo ';' >> ${CERTDATA_FILE}

tee -a ${CERTDATA_FILE} << __EOF__
const int clientCertLen = sizeof(client_cert_pem);
__EOF__

#3 privateKey.pem

tee -a ${CERTDATA_FILE} << __EOF__

/* privateKey.pem certificate file */

const unsigned char client_private_key_pem[] =
__EOF__

while IFS='' read -r line
do
	echo '\t"'"$line"'\\r\\n"' >> ${CERTDATA_FILE}
done < "$3"

echo ';' >> ${CERTDATA_FILE}

tee -a ${CERTDATA_FILE} << __EOF__
const int clientPrivateKeyLen = sizeof(client_private_key_pem);
__EOF__
