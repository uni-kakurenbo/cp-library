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

set +e
{
    echo "${TARGET} (${PROBLEM})"
    echo "[Hash: ${PROBLEM_HASH}] [PID: ${PID}]"

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

    RICH_TARGET="[\`${TARGET}\`](https://github.com/${GITHUB_REPOSITORY}/blob/${BRANCH_NAME}/${TARGET})"
    RICH_PROBLEM="[$(basename "${PROBLEM}")](${PROBLEM})"

    echo -ne "- ${RICH_TARGET} (${RICH_PROBLEM})\r" >../summary.txt
    echo "  - Testcase hash: \`${PROBLEM_HASH}\`" >>../summary.txt

    if [ ${EXIT_STATUS} -eq 0 ]; then
        jq -n --arg target "${TARGET}" --arg date "$(date -d "@${LAST_MODIFIED_AT}" '+%Y-%m-%d %H:%M:%S %z')" \
            '.[$target] = $date' >>"../timestamps.json"

        cat ../summary.txt >>../passed-tests.txt
    else
        cat ../summary.txt >>../failed-tests.txt
    fi

    echo
} &>".log-${PID}.txt"

set -e

cat ".log-${PID}.txt"

exit ${EXIT_STATUS}
