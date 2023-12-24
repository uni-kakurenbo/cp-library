#! /bin/bash
set +e

touch ./prev/hash.txt

diff -q ./hash.txt ./prev/hash.txt
DIFFERED="$?"

STATUS=""
if [ "${DIFFERED}" == '0' ]; then
    STATUS=$(cat ./prev/status.txt)
fi

echo "Differed: ${DIFFERED}"
echo "Status: ${STATUS}"

echo "differred=${DIFFERED}" >> "${GITHUB_OUTPUT}"
echo "status=${STATUS}" >> "${GITHUB_OUTPUT}"
