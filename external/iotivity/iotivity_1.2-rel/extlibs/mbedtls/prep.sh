#!/bin/sh

set -e

# Keep mbedtls_revision in sync with extlibs/mbedtls/SConscript's setting. Right now this script
# assumes mbedtls_revision is a tag; comment out the second clause if mbedtls_revision becomes
# a branch or a specific commit.
mbedtls_url="https://github.com/ARMmbed/mbedtls"
mbedtls_dir=`dirname -- "$(readlink -f "$0")"`/mbedtls
mbedtls_reldir="extlibs/mbedtls/mbedtls"

mbedtls_revision="mbedtls-2.4.0"
if [ ! -d ${mbedtls_dir} ]; then
    echo ""
    echo "*********************************** Error: ****************************************"
    echo "* Please download mbedTLS using the following command:                            *"
    echo "*     $ git clone ${mbedtls_url} ${mbedtls_reldir} -b ${mbedtls_revision}"
    echo "***********************************************************************************"
    echo ""
    exit
elif [ ! -z $(git tag -l $mbedtls_revision) ]; then
    echo ""
    echo "*********************************** Error: ****************************************"
    echo "* mbedTLS repo is out of date. Please update mbedtls using the following commands:*"
    echo "*     $ cd (authoritative mbedTLS source repo location)                           *"
    echo "*     $ git fetch                                                                 *"
    echo "***********************************************************************************"
    echo ""
    exit
elif [ -d ${mbedtls_dir}/.git ]; then
    cd ${mbedtls_dir}
    git reset --hard ${mbedtls_revision} ; git apply --whitespace=fix ../ocf.patch
    cd -
    rm -rf ${mbedtls_dir}/.git*
else
    echo "tizen: Checking if ocf.patch is applied in ${mbedtls_dir}"
    grep -r 'TLS_ECDH_ANON_WITH_AES_128_CBC_SHA256' "${mbedtls_dir}"
fi

