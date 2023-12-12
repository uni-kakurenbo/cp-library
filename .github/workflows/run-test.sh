#! /bin/bash

BASE_DIR="$PWD"
TARGET="$1"


cd "$BASE_DIR" || exit 1


DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"$BASE_DIR" "$TARGET")

#shellcheck disable=SC2086
LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- $DEPENDENCIES)"
LAST_VERIFY_DATE="$(jq -r --arg target "$TARGET" '.[$target] // "@0"' ./.verify-helper/timestamps.remote.json)"
LAST_MODIFIED_AT=$(date --date "$LAST_MODIFY_DATE" "+%s")
LAST_VERIFIED_AT=$(date --date "$LAST_VERIFY_DATE" "+%s")

echo "$LAST_MODIFIED_AT $LAST_VERIFIED_AT"
if [ "$LAST_MODIFIED_AT" -le "$LAST_VERIFIED_AT" ]; then
  echo "already verified."
else
    oj-verify run "$TARGET" --tle 5 || exit 1
fi

cd .verify-helper || exit 1

# jq -n --arg target "$TARGET" --arg date "$LAST_MODIFY_DATE" '.[$target] = $date' >> ./timestamps-"$NODE_ID".json
echo "{}" > ./timestamps-"$NODE_ID".json
# cat ./timestamps-"$NODE_ID".json
