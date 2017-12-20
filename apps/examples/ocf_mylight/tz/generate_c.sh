#!/bin/bash
SCRIPT_PATH=$1
TARGET=${SCRIPT_PATH}../ocf_mylight_tz.c
TOOL="$(basename $0)"
TIMESTAMP=`date -R`
ZONELIST=""

echo $TOOL $TIMESTAMP

cat ${SCRIPT_PATH}tpl_head.c > ${TARGET}

cd ${SCRIPT_PATH}zoneinfo
for filename in *; do
	echo "Adding "$filename"..."
	xxd -i $filename >> ${TARGET}
	if [ "$ZONELIST" != "" ]; then
		ZONELIST+=",\n"
	fi
	ZONELIST+="\t{ \"${filename}\", ${filename}, \&${filename}_len }"
done
cd -

cd ${SCRIPT_PATH}
echo ${ZONELIST}

sed -i "s/^  /\t/" ${TARGET}
sed -i "s/unsigned/static unsigned/" ${TARGET}

cat tpl_foot.c >> ${TARGET}

sed -i "s/{zone}/{\n${ZONELIST}\n}/" ${TARGET}
sed -i "s/{tool}/${TOOL}/" ${TARGET}
sed -i "s/{timestamp}/${TIMESTAMP}/" ${TARGET}

cd -
