#! /bin/bash

cd "$(dirname "$0")" || exit 1
cd ./.verify-helper/ || exit 1

jq --slurp --sort-keys \
    '
        reduce .[] as $item ([]; . + ($item | to_entries))
        | group_by(.key) | map(max_by(.value)) |
        from_entries
    ' \
    ./timestamps.remote.json \
    ./timestamps.local.json \
    >./timestamps.temp.json

cat ./timestamps.temp.json >./timestamps.remote.json
cat ./timestamps.temp.json >./timestamps.local.json

rm ./timestamps.temp.json
