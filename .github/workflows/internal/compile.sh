#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PID="$$"

# shellcheck source=/dev/null
source ./.github/workflows/internal/options.env

DEFAULT_DATE="$(date -d '@0' '+%Y-%m-%d %H:%M:%S %z')"

set +e
{
    echo "::group::${TARGET}"

    DEPENDENCIES="$(time g++-12 -std=gnu++20 -MM -I"${WORKING_DIRECTORY}" "${TARGET}" | sed -E s/^.*\.o:\ //)"

    # shellcheck disable=SC2086
    LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- ${DEPENDENCIES})"
    LAST_VERIFY_DATE="$(
        jq -r --arg target "${TARGET}" \
            '.[$target] // "@0"' ./.verify-helper/timestamps.compile.json
    )"

    LAST_MODIFIED_AT=$(date --date "${LAST_MODIFY_DATE}" '+%s')
    LAST_VERIFIED_AT=$(date --date "${LAST_VERIFY_DATE}" '+%s')

    echo "Last modify: ${LAST_MODIFY_DATE} (${LAST_MODIFIED_AT})"
    echo "Last verify: ${LAST_VERIFY_DATE} (${LAST_VERIFIED_AT})"

    if [ "${LAST_MODIFIED_AT}" -gt "${LAST_VERIFIED_AT}" ]; then
        time g++-12 "${OPTIONS[@]}" -O0 -I"${WORKING_DIRECTORY}" -I"${WORKING_DIRECTORY}/../ac-library" "${TARGET}" 2>".warn-${PID}.txt"

        LINES=$(wc -l ".warn-${PID}.txt" | cut -d' ' -f1)

        DATE=""

        if [[ ${LINES} -gt 3 ]]; then
            cat ".warn-${PID}.txt"

            {
                RICH_TARGET="<a href=https://github.com/${GITHUB_REPOSITORY}/blob/${BRANCH_NAME}/${TARGET}><code>${TARGET}</code></a>"

                echo -ne "<details>\r"
                echo -ne "<summary>${RICH_TARGET}</summary>\r"
                echo -ne "<pre>\r"
                head ".warn-${PID}.txt" -n 25 | tail -n 22 | tr '\n' '\r' | sed -e 's/</\&lt;/g'
                echo -ne "</pre>\r"
                echo -e "</details>\r"
            } >>".summary-${PID}.txt"

            DATE="$(jq -r --arg target "${TARGET}" --arg default "${DEFAULT_DATE}" '.[$target] // $default' './.verify-helper/timestamps.compile.json')"
        else
            DATE="$(date -d "@${LAST_MODIFIED_AT}" '+%Y-%m-%d %H:%M:%S %z')"
        fi

        jq -n --arg target "${TARGET}" --arg date "${DATE}" \
            '.[$target] = $date' >>'../timestamps.json'
    fi

    echo "::endgroup::"

} &>".log-${PID}.txt"
set -e

cat ".log-${PID}.txt"
