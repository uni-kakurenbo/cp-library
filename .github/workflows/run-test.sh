#! /bin/bash

TARGET="$1"
DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"$GITHUB_WORKSPACE" "$TARGET")

#shellcheck disable=SC2086
LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- $DEPENDENCIES)"
LAST_VERIFY_DATE="$(
    jq -r --arg target "$TARGET" \
    '.[$target] // "@0"' ./.verify-helper/timestamps.remote.json
)"

LAST_MODIFIED_AT=$(date --date "$LAST_MODIFY_DATE" "+%s")
LAST_VERIFIED_AT=$(date --date "$LAST_VERIFY_DATE" "+%s")

echo "$TARGET"
echo "Last modified: $LAST_MODIFY_DATE ($LAST_MODIFIED_AT)"
echo "Last verified: $LAST_VERIFY_DATE ($LAST_VERIFIED_AT)"

if [ "$LAST_MODIFIED_AT" -le "$LAST_VERIFIED_AT" ]; then
  echo "Already verified."
else
    oj-verify run "$TARGET" --tle 5 || exit 1
fi

cd .verify-helper || exit 1
jq -n --arg target "$TARGET" --arg date "$LAST_MODIFY_DATE" \
'.[$target] = $date' >> ./timestamps-"$NODE_ID".json
