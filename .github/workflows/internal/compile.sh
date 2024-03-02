#! /bin/bash
set -eu

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PID="$$"

# shellcheck source=/dev/null
source ./.github/workflows/internal/options.env

set +e
{
    echo "::group::compile: ${TARGET}"

    time g++-12 "${OPTIONS[@]}" -O0 -I"${WORKING_DIRECTORY}" -I"${WORKING_DIRECTORY}/../ac-library" "${TARGET}" 2>".warn-${PID}.txt"

    LINES=$(wc -l ".warn-${PID}.txt" | cut -d' ' -f1)

    if [[ ${LINES} -gt 3 ]]; then
        cat ".warn-${PID}.txt"

        {
            RICH_TARGET="<a href=https://github.com/${GITHUB_REPOSITORY}/blob/${GITHUB_REF_NAME}/${TARGET}><code>${TARGET}</code></a>"

            echo -ne "<details>\r"
            echo -ne "<summary>${RICH_TARGET}</summary>\r"
            echo -ne "<pre><xmp>\r"
            head ".warn-${PID}.txt" -n 25 | tail -n 22 | tr '\n' '\r'
            echo -ne "</xmp></pre>\r"
            echo -e "</details>\r"
        } >>".summary-${PID}.txt"
    fi

    echo "::endgroup::"
} &>".log-${PID}.txt"
set -e

cat ".log-${PID}.txt"
