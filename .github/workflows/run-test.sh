#! /bin/bash
set -eu

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

LAST_MODIFIED_AT=$(date --date "$LAST_MODIFY_DATE" '+%s')
LAST_VERIFIED_AT=$(date --date "$LAST_VERIFY_DATE" '+%s')

EXIT_STATUS=0

set +e
{
    echo "::group::$TARGET [PID: $PID]"
    echo "Last modify: $LAST_MODIFY_DATE ($LAST_MODIFIED_AT)"
    echo "Last verify: $LAST_VERIFY_DATE ($LAST_VERIFIED_AT)"
    echo "::endgroup::"

    if [ "$LAST_MODIFIED_AT" -le "$LAST_VERIFIED_AT" ]; then
        echo "::notice file=$TARGET::Already verified. (Test was skipped.)"
    else
        echo "::group::oj-verify run"

        oj-verify run "$TARGET" --tle 5
        EXIT_STATUS=$?

        echo "::endgroup::"
        
        if [ $EXIT_STATUS -eq 0 ]; then
            echo "::notice file=$TARGET::All tests passed successfully."
        fi
    fi

    echo
} &>> ".log-$PID.txt"

set -e

jq -n --arg target "$TARGET" --arg date "$LAST_MODIFY_DATE" \
'.[$target] = $date' >> "./.verify-helper/timestamps.json"

cat ".log-$PID.txt"

exit $EXIT_STATUS
