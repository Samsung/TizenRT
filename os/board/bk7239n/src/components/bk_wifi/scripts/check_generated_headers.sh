#!/bin/sh

current_user=$(whoami)
cd $1
if [ -d .git ]; then
	git diff --exit-code include/generated/ > /dev/null
	if [ $? -ne 0 ]; then
		# include/generated has uncommit changes, if current user is jenkins, fail the compile
		if [ "$current_user" = "jenkins" ]; then
			echo "include/generated directory has uncommited changes"
			exit 1
		else
			echo -e "\e[91mRemember to submit components/bk_wifi/include/generated directory\e[0m"
			exit 0
		fi
	fi
fi
