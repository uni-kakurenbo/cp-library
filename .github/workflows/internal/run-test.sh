#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PROBLEM="$2"
PROBLEM_HASH="$3"
PID="$$"

DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"${WORKING_DIRECTORY}" "${TARGET}")

#shellcheck disable=SC2086
LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- ${DEPENDENCIES})"
LAST_VERIFY_DATE="$(
    jq -r --arg target "${TARGET}" \
    '.[$target] // "@0"' ./.verify-helper/timestamps.remote.json
)"

LAST_MODIFIED_AT=$(date --date "${LAST_MODIFY_DATE}" '+%s')
LAST_VERIFIED_AT=$(date --date "${LAST_VERIFY_DATE}" '+%s')

EXIT_STATUS=0

# shellcheck source=/dev/null
source ./.github/workflows/internal/options.env

set +e
{
    echo "::group::${TARGET} (${PROBLEM}) [PID: ${PID}]"
    echo "Last modify: ${LAST_MODIFY_DATE} (${LAST_MODIFIED_AT})"
    echo "Last verify: ${LAST_VERIFY_DATE} (${LAST_VERIFIED_AT})"
    echo "::endgroup::"

    if [ "${LAST_MODIFIED_AT}" -le "${LAST_VERIFIED_AT}" ]; then
        echo "::notice file=${TARGET}::Already verified. (Test was skipped.)"
    else
        echo "::group::build"
        g++-12 "${OPTIONS[@]}" -I"${WORKING_DIRECTORY}" -o "${TARGET}.exe" "${TARGET}" 2>&1
        echo "::endgroup::"

        TESTER_OPTION=''
        if [ -f "../testcases/${PROBLEM_HASH}/checker" ]; then
            TESTER_OPTION+="--judge-command ../testcases/${PROBLEM_HASH}/checker"
        fi

        echo "::group::run test"

        #shellcheck disable=SC2086
        oj test --command "${TARGET}.exe" --directory "../testcases/${PROBLEM_HASH}/" \
            --tle 30 ${TESTER_OPTION}
        EXIT_STATUS=$?

        echo "::endgroup::"

        if [ ${EXIT_STATUS} -eq 0 ]; then
            echo "::notice file=${TARGET}::All tests passed successfully."
        fi
    fi

    echo
} &>> ".log-${PID}.txt"

set -e

jq -n --arg target "${TARGET}" --arg date "${LAST_MODIFY_DATE}" \
'.[$target] = $date' >> "./.verify-helper/timestamps.json"

cat ".log-${PID}.txt"

exit ${EXIT_STATUS}
