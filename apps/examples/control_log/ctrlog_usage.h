/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#pragma once

#define CTRLOG_USAGE                                                                                        \
	"NAME\n"                                                                                                  \
	"\tctrlog option [...]\n"                                                                                 \
	"\n"                                                                                                      \
	"OPTIONS\n"                                                                                               \
	"\n"                                                                                                      \
	"LEVEL SPECIFIC OPTIONS\n"                                                                                \
	"TASH> ctrlog level=value mod=value\n"                                                                    \
	"level \n"                                                                                                \
	"\terror : print error log\n"                                                                             \
	"\tinfo : print information log\n"                                                                        \
	"\tverb : print verbose log\n"                                                                            \
	"mod : select module to set\n"                                                                            \
	"\tlwip[:submodule] : select lwip submodule. for example lwip:tcp will set color for TCP_DEBUG in lwIP\n" \
	"\twifi : select wifi_manager module\n"                                                                   \
	"\tnetmgr : select network manager module\n"                                                              \
	"\tnetlib : select network library module like dhcp dns\n"                                                \
	"\n"                                                                                                      \
	"TIME SPECIFIC OPTIONS\n"                                                                                 \
	"TASH> time=mode\n"                                                                                       \
	"time\n"                                                                                                  \
	"\ton : turn on time log\n"                                                                               \
	"\toff : turn off time log\n"                                                                             \
	"\n"
