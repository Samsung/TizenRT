#!/bin/bash
#

WD=$(cd `dirname $0` && pwd)

# clean
make -C $WD distclean

# set config
$WD/tools/configure.sh $1
