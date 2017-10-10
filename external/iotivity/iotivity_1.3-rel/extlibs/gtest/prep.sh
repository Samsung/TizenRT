#!/bin/bash
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

set -e

[ ! -z ${EXEC_MODE} ] || EXEC_MODE=false

package="gtest"
packageRevision='1.7.0'
packageUrl='https://github.com/google/googletest/archive/release-1.7.0.zip'
packageArchive=$(basename -- "${packageUrl}")
packageDir="extlibs/${package}/googletest-release-${packageRevision}"
packageSourceFile="${packageDir}/CMakeLists.txt"


do_()
{
    set +f
    if $EXEC_MODE; then
        echo "warning: fetching online resources may not be reproductible"
        printf "%s \n" "trying: \"$@\""
        eval "$@"
    else
        cat<<EOF
error: Something should be prepared, please manually execute from shell
EOF
        printf "%s \n" "$@"
        exit 1
    fi
}


main_()
{
    echo "# Checking for gtest presence:"
    if [ ! -e "${packageSourceFile}" ] ; then
        whereis -b wget 2>/dev/null
        whereis -b unzip 2>/dev/null
        do_ "cd extlibs/${package} && wget -nc -O ${packageArchive} ${packageUrl} && unzip ${packageArchive}"
    fi
}


main_ "$@"
