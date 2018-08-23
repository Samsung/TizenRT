#!/usr/bin/env bash
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
# dbuild.sh

# Checking docker is installed
nodocker() {
	cat <<EOF

You don't have docker installed in your path.
Please find our quick start to build using docker client.

  https://github.com/samsung/tizenrt#quick-start

EOF
}

if ! which docker > /dev/null; then
    nodocker 1>&2
    exit 1
fi

# Run TizenRT docker container
BUILD_CMD=make
BUILD_OPT=$1
BUILD_OPT_LOWER=$(echo "$BUILD_OPT" | tr '[:upper:]' '[:lower:]')

case "$BUILD_OPT_LOWER" in
download )
	${BUILD_CMD} ${BUILD_OPT_LOWER} $2 $3 $4 $5 $6
	;;
*)
	docker rm -f tizenrt > /dev/null 2>&1
	docker run --rm -it -d -v $(pwd)/..:/root/tizenrt --name tizenrt -w /root/tizenrt/os tizenrt/tizenrt /bin/bash
	docker exec -it tizenrt ${BUILD_CMD} ${BUILD_OPT_LOWER}
	docker stop tizenrt > /dev/null 2>&1
	;;
esac

exit 0
