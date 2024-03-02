#! /bin/bash

cd ./.verify-helper/ || exit 1

./sync.sh && oj-verify run "$1"
