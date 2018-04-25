#!/bin/bash
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
TARGET=$1

if ! which indent >/dev/null; then
    echo "Need to install indent"
    exit 1
fi
if ! which astyle >/dev/null; then
    echo "Need to install astyle"
    exit 1
fi

function USAGE()
{
    echo "usage: $0 TARGET"
    echo "     TARGET: folder path or target files"
    echo "Apply TizenRT C coding rule"
    echo ""
    echo "example:"
    echo "    $0 ../kernel"
    echo "    $0 ../../abc.c"
}

if [ -z ${TARGET} ]; then
    USAGE
    exit 1
fi
if [ ${TARGET} == "--help" ]; then
    USAGE
    exit 1
fi

if [ -d ${TARGET} ]; then
    find ${TARGET} -name "*.[c,h]" | xargs -I{} indent {} -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i4 -ip0 -l1000 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts4 -il1
    astyle --style=linux --add-brackets --indent=force-tab=4 --unpad-paren --pad-header --pad-oper --align-pointer=name --recursive "${TARGET}/*.c" "${TARGET}/*.h"
    find ${TARGET} -name "*.[c,h]" | xargs perl -pi -e 's/ \*\) & / \*\)&/g;s/ \*\) - 1/ \*\)-1/g;s/ \*\) kmm/ \*\)kmm/g;s/ \*\) malloc/ \*\)malloc/g;s/ \*\) realloc/ \*\)realloc/g;s/ \*\) zalloc/ \*\)zalloc/g'
else
    indent ${TARGET} -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i4 -ip0 -l1000 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts4 -il1
    astyle --style=linux --add-brackets --indent=force-tab=4 --unpad-paren --pad-header --pad-oper --align-pointer=name ${TARGET}
    perl -pi -e 's/ \*\) & / \*\)&/g;s/ \*\) - 1/ \*\)-1/g;s/ \*\) kmm/ \*\)kmm/g;s/ \*\) malloc/ \*\)malloc/g;s/ \*\) realloc/ \*\)realloc/g;s/ \*\) zalloc/ \*\)zalloc/g' ${TARGET}
fi
