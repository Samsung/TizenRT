#!/bin/sh
TARGET=../ocf_mylight_default_db.c
TOOL="$(basename $0)"
TIMESTAMP=`date -R`

echo $TOOL $TIMESTAMP

cat tpl_head.c > ${TARGET}
xxd -i default_svr_db.json >> ${TARGET}
xxd -i introspection.json >> ${TARGET}
sed -i "s/^  /\t/" ${TARGET}
sed -i "s/unsigned/static unsigned/" ${TARGET}
sed -i "s/{tool}/${TOOL}/" ${TARGET}
sed -i "s/{timestamp}/${TIMESTAMP}/" ${TARGET}
cat tpl_foot.c >> ${TARGET}

