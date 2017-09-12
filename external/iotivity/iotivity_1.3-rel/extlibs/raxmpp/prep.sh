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

topdir="${PWD}"

# Keep packageRevision in sync with SConscript's setting
# Right now this script assumes packageRevision is a tag;
# comment out the second clause
# if packageRevision becomes a branch or a specific commit.
package="iotivity-xmpp"
packageUrl="https://gerrit.iotivity.org/gerrit/iotivity-xmpp"
packageDir="./extlibs/raxmpp/raxmpp"
packageVersion="master"
# TODO: pin
packageRevision="$packageVersion"


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
    if [ ! -d "${packageDir}" ]; then
        do_ "git clone ${packageUrl} ${packageDir} -b ${packageVersion}"
    elif [ ! -z $(git tag -l "${packageRevision}") ]; then
        cat<<EOF
error: $packageDir is unaligned with supported release of ${package}
Please update ${package} using "cd ${packageDir} && git fetch"
For more support please refer to:
https://wiki.iotivity.org/build
EOF
        exit 2
    fi
    if [ -d "${packageDir}/.git" ]; then
        cd "${packageDir}" || exit 1
        git clean -f -d -x
        git reset --hard "${packageRevision}"
        cd -
        rm -rf -- "${packageDir}/.git"
    else
        echo "log: Assuming ${package} is already on correct revision: \"${packageRevision}\""
    fi
}


main_ "$@"

