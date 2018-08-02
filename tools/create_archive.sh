#!/bin/bash

set -e
set -u

# $1 - archive to create
# $2 ... list of objects (libraries)
function generate_script() {
  archive=$1; shift
  echo "create ${archive}"
  while [ $# -gt 0 ]; do
    if [ -n "$1" -a -f "$1" ]; then
      echo "addlib $1"
    else
      echo "Archive $1 not accessible" >&2
    fi
    shift
  done
  echo -e "save\nend"
}

AR=${CROSSDEV:-}ar
if ! which "${AR}"; then
  echo "${AR} not supplied" >&2
  exit 1
fi

generate_script "$@" | ${AR} -M
