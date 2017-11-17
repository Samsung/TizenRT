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

KCONFIG="Kconfig"
METAFILE=".appSpec"
MAKEFILE="Makefile"
OSDIR="../os/"
APPDIR="../apps/"
KCONFIG_ENTRY="Kconfig_ENTRY"
DOTCONFIG=`find ../os/ -name '.config'`
TARGET_DIR_LIST=`find . -name Kconfig_ENTRY | sed -n 's/Kconfig_ENTRY//p'`

# Remove an old meta file before making a new meta
if [ -f $OSDIR$METAFILE ]; then
	rm $OSDIR$METAFILE
fi

USER_ENTRYPOINT=`sed -n '/CONFIG_USER_ENTRYPOINT/p' $DOTCONFIG | sed -n 's/.*=//p'`
echo -e "USER_ENTRYPOINT=$USER_ENTRYPOINT\n" >> $OSDIR$METAFILE

# get app's info from Kconfig & Kconfig_ENTRY;
# find CONFIG_NAME from Kconfig_ENTRY and search SELECTABLE(find select & depends on);

for FILE in $TARGET_DIR_LIST
do
	APPNAME=`sed -n '/^APPNAME/p' $APPDIR$FILE$MAKEFILE | sed -n 's/APPNAME = //p'`
	FUNCTION=`grep -A 3 'config USER_ENTRYPOINT' $APPDIR$FILE$KCONFIG | sed -n 's/\tdefault "\(\w*\)".*/\1/p'`
	CONFIG_NAME=`sed -n '/depends/p' $APPDIR$FILE$KCONFIG_ENTRY | sed -n 's/.*depends on //p'`
	if [ "$CONFIG_NAME" = "" ]; then
		continue
	fi
	CHECK=`sed -n "/CONFIG_$CONFIG_NAME=y/p" $DOTCONFIG`
	if [ "$CHECK" = "" ]
		then CONFIG_USE="n"
	else
		CONFIG_USE="y"
	fi
	DESCRIPTION=`sed -n '/bool/p' $APPDIR$FILE$KCONFIG_ENTRY | sed -n 's/.*bool //p'`
	KCONFIG_INFO=`sed -n "/config $CONFIG_NAME$/,/^$/p" $APPDIR$FILE$KCONFIG`
	DEPENDENCY=`echo "$KCONFIG_INFO" | sed -n '/depends on/p'`
	SELECTFLAG=`echo "$KCONFIG_INFO" | sed -n '/select/p'`
	if [ "$DEPENDENCY" != "" ] || [ "$SELECTFLAG" != "" ]
		then SELECTABLE="n"
	else
		SELECTABLE="y"
	fi
	if [ "$APPNAME" != "" ]
		then echo "{ $APPNAME, $FUNCTION, CONFIG_$CONFIG_NAME, $CONFIG_USE, $SELECTABLE, $DESCRIPTION }" >> $OSDIR$METAFILE
	fi
done

echo "$METAFILE was made at os directory."
