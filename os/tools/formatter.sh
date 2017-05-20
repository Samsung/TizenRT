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

function USAGE()
{
    echo "usage: $0 <TARGET>"
    echo "     <TARGET> : folder path or target files"
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

read -d '' INDENT_RULES << __EOF__
	--no-blank-lines-after-declarations
	--blank-lines-after-procedures
	--no-blank-lines-after-commas
	--break-before-boolean-operator
	--honour-newlines
	--braces-on-if-line
	--braces-on-struct-decl-line
	--comment-indentation33
	--declaration-comment-column33
	--no-comment-delimiters-on-blank-lines
	--cuddle-else
	--continuation-indentation4
	--case-indentation0
	--line-comments-indentation0
	--declaration-indentation1
	--dont-format-first-column-comments
	--indent-level4
	--parameter-indentation0
	--line-length1000
	--continue-at-parentheses
	--no-space-after-function-call-names
	--no-space-after-parentheses
	--dont-break-procedure-type
	--space-after-if
	--space-after-for
	--space-after-while
	--no-space-after-casts
	--dont-star-comments
	--swallow-optional-blank-lines
	--dont-format-comments
	--else-endif-column33
	--space-special-semicolon
	--tab-size4
	--indent-label1
__EOF__

read -d '' ASTYLE_RULES << __EOF__
	--style=linux
	--add-brackets
	--indent=force-tab=4
	--unpad-paren
	--pad-header
	--pad-oper
	--align-pointer=name
__EOF__

if [ -d ${TARGET} ]; then
    find ${TARGET} -name "*.[c,h]" | xargs -I{} indent {} ${INDENT_RULES}
    astyle ${ASTYLE_RULES} --recursive "${TARGET}/*.c" "${TARGET}/*.h"
    find ${TARGET} -name "*.[c,h]" | xargs perl -pi -e 's/ \*\) & / \*\)&/g;s/ \*\) - 1/ \*\)-1/g;s/ \*\) kmm/ \*\)kmm/g;s/ \*\) malloc/ \*\)malloc/g;s/ \*\) realloc/ \*\)realloc/g;s/ \*\) zalloc/ \*\)zalloc/g'
else
    indent ${TARGET} ${INDENT_RULES}
    astyle ${ASTYLE_RULES} ${TARGET}
    perl -pi -e 's/ \*\) & / \*\)&/g;s/ \*\) - 1/ \*\)-1/g;s/ \*\) kmm/ \*\)kmm/g;s/ \*\) malloc/ \*\)malloc/g;s/ \*\) realloc/ \*\)realloc/g;s/ \*\) zalloc/ \*\)zalloc/g' ${TARGET}
fi
