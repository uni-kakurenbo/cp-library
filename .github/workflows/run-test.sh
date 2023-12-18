#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PID="$$"

DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"${WORKING_DIRECTORY}" "${TARGET}")

#shellcheck disable=SC2086
LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- ${DEPENDENCIES})"
LAST_MODIFIED_AT=$(date --date "${LAST_MODIFY_DATE}" '+%s')

EXIT_STATUS=0

set +e
{
    echo "::group::oj-verify run"

    oj-verify run "${TARGET}" --tle 30
    EXIT_STATUS=$?

    echo "::endgroup::"

    if [ ${EXIT_STATUS} -eq 0 ]; then
        echo "::notice file=${TARGET}::All tests passed successfully."
    fi

    echo
} &>> ".log-${PID}.txt"

set -e

jq -n --arg target "${TARGET}" --arg date "${LAST_MODIFY_DATE}" \
'.[$target] = $date' >> "./.verify-helper/timestamps.json"

cat ".log-${PID}.txt"

exit ${EXIT_STATUS}
