#!/bin/sh
SCRIPT_PATH=$1
TARGET=${SCRIPT_PATH}../ocf_mylight_default_db.c
TOOL="$(basename $0)"
TIMESTAMP=`date -R`

if [ $# -ne 0 ]; then
	cd ${SCRIPT_PATH}
fi

echo $TOOL $TIMESTAMP

cat tpl_head.c > ${TARGET}
xxd -i default_svr_db.json >> ${TARGET}
echo "#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER" >> ${TARGET}
xxd -i introspection.dimmer.json >> ${TARGET}
echo "#else" >> ${TARGET}
xxd -i introspection.json >> ${TARGET}
echo "#endif" >> ${TARGET}

sed -i "s/^  /\t/" ${TARGET}
sed -i "s/unsigned/static unsigned/" ${TARGET}
sed -i "s/{tool}/${TOOL}/" ${TARGET}
sed -i "s/{timestamp}/${TIMESTAMP}/" ${TARGET}
cat tpl_foot.c >> ${TARGET}

if [ $# -ne 0 ]; then
	cd -
fi
