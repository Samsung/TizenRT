#!/bin/bash

set -e

TOOLS_DIR=$(dirname $0)

CHANGES=$2
OUTPUT="$(git diff --name-only --diff-filter=AM ${CHANGES})"

${TOOLS_DIR}/lint.py $1 $OUTPUT