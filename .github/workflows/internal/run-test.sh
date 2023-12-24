#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PROBLEM="$2"
PROBLEM_HASH="$3"
LAST_MODIFY_DATE="$4"
PID="$$"

EXIT_STATUS=0

# shellcheck source=/dev/null
source ./.github/workflows/internal/options.env

set +e
{
    echo "${TARGET} (${PROBLEM} : ${PROBLEM_HASH}) [PID: ${PID}]"

    echo "::group::build"
    time g++-12 "${OPTIONS[@]}" -I"${WORKING_DIRECTORY}" -o "${TARGET}.exe" "${TARGET}"
    echo "::endgroup::"

    TESTER_OPTION=''
    if [ -f "../testcases/${PROBLEM_HASH}/checker" ]; then
        TESTER_OPTION+="--judge-command ../testcases/${PROBLEM_HASH}/checker"
    fi

    echo "::group::run test"

    #shellcheck disable=SC2086
    time oj test --command "${TARGET}.exe" --directory "../testcases/${PROBLEM_HASH}/" \
        --tle 600 ${TESTER_OPTION}
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
