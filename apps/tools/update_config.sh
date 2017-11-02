#!/bin/bash
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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

OSDIR="../os"
METAFILE=".appSpec"
DOTCONFIG=".config"

function write_config(){
	file_path=`find $OSDIR -name "$METAFILE"`
	config_path=`find $OSDIR -name "$DOTCONFIG"`
	# update .config's user entry point from metafile;

	UPDATED_ENTRYPOINT=`sed -n '/USER_ENTRYPOINT/p' $file_path | sed -n 's/.*=//p'`
	sed -i "s/CONFIG_USER_ENTRYPOINT.*/CONFIG_USER_ENTRYPOINT=$UPDATED_ENTRYPOINT/" $config_path

	CONFIG_ENTRY_LIST=`sed -n '/^CONFIG_ENTRY_/p' $config_path`
	if [ "$CONFIG_ENTRY_LIST" != "CONFIG_ENTRY_MANUAL" ]; then
		sed -i '/^CONFIG_ENTRY_/{ s/CONFIG/# CONFIG/g; s/=y/ is not set/g }' $config_path
		sed -i '/CONFIG_ENTRY_MANUAL/{ s/# //g ; s/ is not set/=y/g }' $config_path
	fi

	# update .config with reference to metafile.
	# .config will be updated only if config is newly created or config value is changed.

	UPDATED_APP_LIST=`sed -n '/{.*}/p' $file_path | sed -n 's/ //pg'`
	for UPDATED_APP in $UPDATED_APP_LIST
	do
		CONFIG_NAME=`echo "$UPDATED_APP" | awk 'BEGIN {FS=","} {print$3}'`
		NEW_CONFIG_VALUE=`echo "$UPDATED_APP" | awk 'BEGIN {FS=","} {print$4}'`

		if [ "`sed -n "/^$CONFIG_NAME=y/p" $config_path`" ]
		then
			OLD_CONFIG_VALUE="y"
		elif [ "`sed -n "/^\# $CONFIG_NAME is not set/p" $config_path`" ]
		then
			OLD_CONFIG_VALUE="n"
		else
			OLD_CONFIG_VALUE="x"
		fi

		if [ "$OLD_CONFIG_VALUE" == "y" ] && [ "$NEW_CONFIG_VALUE" == "n" ]
		then
			sed -i "s/^$CONFIG_NAME=y/# $CONFIG_NAME is not set/" $config_path
		elif [ "$OLD_CONFIG_VALUE" == "n" ] && [ "$NEW_CONFIG_VALUE" == "y" ]
		then
			sed -i "s/^# $CONFIG_NAME is not set/$CONFIG_NAME=y/" $config_path
		elif [ "$OLD_CONFIG_VALUE" == "x" ] && [ "$NEW_CONFIG_VALUE" == "y" ]
		then
			echo "$CONFIG_NAME=y" >> $config_path
		fi
	done
}

function set_entrypoint(){
	# if appname for updating is omitted => set_entrypoint() is not excuted;
	if [ "$1" = "" ]
		then return
	fi
	file_path=`find $OSDIR -name "$METAFILE"`
	SELECTED_APP=`sed -n "/$1,/p" $file_path`
	SELECTED_APP_ENTRY=`echo "$SELECTED_APP" | awk 'BEGIN {FS=","} {print$2}' | sed -n 's/ //p'`
	sed -i "s/USER_ENTRYPOINT=.*/USER_ENTRYPOINT=$SELECTED_APP_ENTRY/" $file_path
}

while getopts ws opt
do
	case $opt in
		w) write_config;
			;;
		s) set_entrypoint $2;
			;;
	esac
done
