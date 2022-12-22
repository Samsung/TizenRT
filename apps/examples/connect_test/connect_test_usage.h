/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#define CT_ADD_HOSTNAME_USAGE                            \
	"\n Add Hostname in file \n"                                   \
	"       wm_test add_hostname [file_path] [hostname]\n\n"

#define CT_EASYSETUP_SERVER_USAGE                           \
	"\n DA Device simulation in Easy Setup process"                \
	"       connect_test server [ap ssid] [ap security mode] [ap password] [softap ssid] [softap password] [softap channel]\n"  \
	"                [repeat count] [udp/dtls] [udp/dtls data size in KB] [tcp/tls] [tcp/tls data size in KB] [hostname filepath]\n\n"
        
#define CT_EASYSETUP_CLIENT_USAGE                          \
	"\n Mobile Smarthings Application simulation in Easysetup Process\n"                 \
        "       connect_test client [ap ssid] [ap security mode] [ap password] [softap ssid] [softap password] [softap channel] \n"            \
	"             [repeat count] [udp/dtls] [udp/dtls data size in KB] [tcp/tls] [tcp/tls data size in KB]\n\n"

#define CT_USAGE									\
	"\n usage: connect_test [options]\n"					\
	"\n run Connect-Test:\n"						\
	CT_ADD_HOSTNAME_USAGE                                                           \
        CT_EASYSETUP_SERVER_USAGE                                                       \
	CT_EASYSETUP_CLIENT_USAGE                                                      
