#! /bin/bash

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PID="$$"

{
  DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"${{ github.workspace }}" "${TARGET}")
  
  LAST_MODIFY_DATE="$(git log -1 --date=iso --pretty=%ad -- ${DEPENDENCIES})"
  LAST_VERIFY_DATE="$(
      jq -r --arg target "${TARGET}" \
      '.[$target] // "@0"' ./.verify-helper/timestamps.remote.json
  )"
  
  LAST_MODIFIED_AT=$(date --date "${LAST_MODIFY_DATE}" '+%s')
  LAST_VERIFIED_AT=$(date --date "${LAST_VERIFY_DATE}" '+%s')

  echo "::group::${TARGET} [PID:${PID}]"
  echo "Last modify: ${LAST_MODIFY_DATE} (${LAST_MODIFIED_AT})"
  echo "Last verify: ${LAST_VERIFY_DATE} (${LAST_VERIFIED_AT})"
  echo '::endgroup::'
  echo '::group::dependencies'
  echo -e "Dependencies:\n${DEPENDENCIES}"
  echo '::endgroup::'

  if [ "${LAST_MODIFIED_AT}" -le "${LAST_VERIFIED_AT}" ]; then
    echo "::notice file=${TARGET}::Already verified. (Test was skipped.)"
  else
    jq --arg TARGET "${TARGET}" --arg date "${LAST_MODIFY_DATE}" '.[$TARGET] = $date' \
    ./.verify-helper/allocation.json > ./.verify-helper/allocation.json
  fi
} &>> ".log-${PID}.txt"

cat ".log-${PID}.txt"
