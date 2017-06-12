#!/bin/sh

set -e

# Keep cborRevision in sync with extlibs/tinycbor/SConscript's setting. Right now this script
# assumes cborRevision is a tag; comment out the second clause if cborRevision becomes
# a branch or a specific commit.
cborUrl="https://github.com/01org/tinycbor"
cborDir="./extlibs/tinycbor/tinycbor"
cborRevision="v0.4"
if [ ! -d ${cborDir} ]; then
    echo ""
    echo "*********************************** Error: ****************************************"
    echo "* Please download TinyCBOR using the following command:                           *"
    echo "*     $ git clone ${cborUrl} ${cborDir} -b ${cborRevision} "
    echo "***********************************************************************************"
    echo ""
    exit
elif [ ! -z $(git tag -l $cborRevision) ]; then
    echo ""
    echo "*********************************** Error: *******************************************"
    echo "* TinyCBOR repo is out of date. Please update TinyCBOR using the following commands: *"
    echo "*     $ cd (authoritative TinyCBOR source repo location)                             *"
    echo "*     $ git fetch                                                                    *"
    echo "**************************************************************************************"
    echo ""
    exit
elif [ -d ${cborDir}/.git ]; then
    cd ${cborDir}
    git reset --hard ${cborRevision}
    cd -
    rm -rf ${cborDir}/.git*
else
    echo "Assuming TinyCBOR is already on correct revision ${cborRevision}"
fi
