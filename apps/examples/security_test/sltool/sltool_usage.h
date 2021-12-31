/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 #define SLTOOL_KEY_USAGE \
	"key mode\n"                  \
	"\tget: get a key index 1\n"    \
	"\tsltool key get 1\n"\
	"\tgen: generate a key to designated index\n"\
	"\tsltool key gen 1\n"\
	"\trem: remove a key in designated index\n"\
	"\tsltool key rem 1\n"\
	"\tset: set a key is not available yet\n\n"

#define SLTOOL_AUTH_USAGE \
	"authenticate mode\n"                  \
	"\tget: get a auth index 1\n"    \
	"\tsltool auth get 1\n"\
	"\trem: remove a auth in designated index\n"\
	"\tsltool auth rem 1\n"\
	"\tset: set a auth is not available yet\n\n"

#define SLTOOL_USAGE              \
	"sltool mode ops [options]\n" \
	SLTOOL_KEY_USAGE\
	SLTOOL_AUTH_USAGE
