#! /bin/bash

set -eu

# shellcheck source=/dev/null
source ./.github/workflows/internal/functions/get-problem-tag.sh

cd ./verify/

function sort_out() {
    PROBLEM="$(grep -I -Po "(?<=\#define\ PROBLEM\ \")[^\",]+(?=\")" "$1")"
    ID="$(echo "${PROBLEM}" | get_problem_tag)"

    TAG="$(basename "$(dirname "$1")")"
    FILE="${ID}/${TAG}_$(basename "$1")"

    mkdir -p "./${ID}/"
    echo "${PROBLEM}" >"${ID}/problem.info"

    mv "$1" "$FILE"

    dirname "${FILE}"
}

export -f sort_out

function rename() {
    cd "$1"
    find ./**.test.cpp -type f | awk '{ printf "mv %s %04d.test.cpp\n", $0, NR - 1 }' | bash
}

export -f rename

rm -f ./**/**/problem.info

find ./**/**/**.test.cpp -type f -exec bash -c 'sort_out "$1"' shell {} \; | sort -u |
    xargs -I {} bash -c 'rename "{}"'

find . -type d -empty -delete
