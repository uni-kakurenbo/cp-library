#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PROBLEM="$2"
PROBLEM_HASH="$3"
LAST_MODIFIED_AT="$4"
PID="$$"

EXIT_STATUS=0

# shellcheck source=/dev/null
source ./.github/workflows/internal/options.env

DEFAULT_DATE="$(date -d '@0' '+%Y-%m-%d %H:%M:%S %z')"

set +e
{
    echo "${TARGET} (${PROBLEM})"
    echo "[Hash: ${PROBLEM_HASH}] [PID: ${PID}]"

    echo "::group::build"
    time g++-12 "${OPTIONS[@]}" -I"${WORKING_DIRECTORY}" -o "${TARGET}.exe" "${TARGET}"
    echo "::endgroup::"

    PRECISION="$(grep -Po '(?<=\#define\ ERROR\ ).+' "${TARGET}")"

    TESTER_OPTION=''
    if [ -f "../testcases/${PROBLEM_HASH}/checker" ]; then
        TESTER_OPTION+="--judge-command ../testcases/${PROBLEM_HASH}/checker"
    fi

    if [ -n "${PRECISION}" ]; then
        TESTER_OPTION+="--error ${PRECISION}"
    fi

    echo "::group::run test"

    #shellcheck disable=SC2086
    time oj test --command "${TARGET}.exe" --directory "../testcases/${PROBLEM_HASH}/" \
        --tle 180 ${TESTER_OPTION}
    EXIT_STATUS=$?

    echo "::endgroup::"

    RICH_TARGET="[\`${TARGET}\`](https://github.com/${GITHUB_REPOSITORY}/blob/${BRANCH_NAME}/${TARGET})"
    RICH_PROBLEM="[$(basename "${PROBLEM}")](${PROBLEM})"

    echo -ne "- ${RICH_TARGET} (${RICH_PROBLEM})\r" >../summary.txt
    echo "  - Testcase hash: \`${PROBLEM_HASH}\`" >>../summary.txt

    DATE=""

    if [ ${EXIT_STATUS} -eq 0 ]; then
        DATE="$(date -d "@${LAST_MODIFIED_AT}" '+%Y-%m-%d %H:%M:%S %z')"

        cat ../summary.txt >>../passed-tests.txt
    else
        DATE="$(jq -r --arg target "${TARGET}" --arg default "${DEFAULT_DATE}" '.[$target] // $default' './.verify-helper/timestamps.remote.json')"

        cat ../summary.txt >>../failed-tests.txt
    fi

    jq -n --arg target "${TARGET}" --arg date "${DATE}" \
        '.[$target] = $date' >>'../timestamps.json'

    echo
} &>".log-${PID}.txt"

set -e

cat ".log-${PID}.txt"

exit ${EXIT_STATUS}
