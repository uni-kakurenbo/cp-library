#! /bin/bash

function sort_out() {
    PROBLEM="$(grep -I -Po "(?<=\#define\ PROBLEM\ \")[^\",]+(?=\")" "$1")"
    HASH="$(echo -n "$PROBLEM" | md5sum | cut -d" " -f1)"

    TAG="$(basename "$(dirname "$1")")"
    FILE="${HASH}/${TAG}_$(basename "$1")"

    mkdir -p "${HASH}"
    echo "${PROBLEM}" >"${HASH}/problem.info"

    mv "$1" "$FILE"

    dirname "$FILE"
}

export -f sort_out

function rename() {
    cd "$1" || exit 1
    find ./**.cpp -type f | awk '{ printf "mv %s %04d.test.cpp\n", $0, NR }' | bash
}

export -f rename

find ./**/**.cpp -type f -exec bash -c 'sort_out "$1"' shell {} \; | sort -u |
    xargs -I {} bash -c 'rename "{}"'

find . -type d -empty -delete
