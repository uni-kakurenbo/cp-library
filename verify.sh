#! /bin/bash

cd "$(dirname "$0")" || exit 1

./sync.sh && oj-verify run "$@" -j 10
