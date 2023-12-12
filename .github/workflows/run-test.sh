#! /bin/bash

WORKING_DIRECTORY="$PWD"
TARGET="$1"
PID="$$"

DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"$WORKING_DIRECTORY" "$TARGET")

#shellcheck disable=SC2086
LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- $DEPENDENCIES)"
LAST_VERIFY_DATE="$(
    jq -r --arg target "$TARGET" \
    '.[$target] // "@0"' ./.verify-helper/timestamps.remote.json
)"

LAST_MODIFIED_AT=$(date --date "$LAST_MODIFY_DATE" "+%s")
LAST_VERIFIED_AT=$(date --date "$LAST_VERIFY_DATE" "+%s")

{
    echo "::group::$TARGET [PID: $PID]"
    echo "Last modified: $LAST_MODIFY_DATE ($LAST_MODIFIED_AT)"
    echo "Last verified: $LAST_VERIFY_DATE ($LAST_VERIFIED_AT)"

    if [ "$LAST_MODIFIED_AT" -le "$LAST_VERIFIED_AT" ]; then
        echo "Already verified."
    else
        oj-verify run "$TARGET" --tle 5 || exit 1
    fi

    echo "::endgroup::"
} >> ".log-$PID.txt"

jq -n --arg target "$TARGET" --arg date "$LAST_MODIFY_DATE" \
'.[$target] = $date' >> "./.verify-helper/timestamps-$NODE_ID.json"

cat ".log-$PID.txt"
