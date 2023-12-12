#! /bin/bash

BASE_DIR="$PWD"
TARGET="$1"

DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"$BASE_DIR" "$TARGET")

#shellcheck disable=SC2086
LAST_COMMIT_DATE="$(git log -1 --date=iso --pretty=%ad -- $DEPENDENCIES)"
LAST_VERIFY_DATE="$(jq -r --arg target "$TARGET" '.[$target]' ./.verify-helper/timestamps.remote.json)"
LAST_COMMITTED_AT=$(date --date "$LAST_COMMIT_DATE" "+%s")
LAST_VERIFIED_AT=$(date --date "$LAST_VERIFY_DATE" "+%s")

if [ "$LAST_COMMITTED_AT" -le "$LAST_VERIFIED_AT" ]; then
  echo "already verified."
  exit 0
fi

oj-verify run "$TARGET" --tle 5 || exit 1

cd .verify-helper || exit 1

jq -n --arg target "$TARGET" date "$LAST_COMMIT_DATE" '.[$target] = $date' >> ./timestamps-"$NODE_ID".json
cat ./timestamps-"$NODE_ID".json
