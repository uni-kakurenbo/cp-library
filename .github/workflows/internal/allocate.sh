#! /bin/bash

WORKING_DIRECTORY="${PWD}"
TARGET="$1"
PID="$$"

{
  PROBLEM="$(grep -Po '(?<=\#define\ PROBLEM\ ")[^",]+(?=")' "${TARGET}")"
  DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"${WORKING_DIRECTORY}" "${TARGET}")

  # shellcheck disable=SC2086
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
    RICH_TARGET="[\`${TARGET}\`](https://github.com/${GITHUB_REPOSITORY}/blob/${GITHUB_REF_NAME}/${TARGET})"
    RICH_PROBLEM="[$(basename "${PROBLEM}")](${PROBLEM})"
    echo "- ${RICH_TARGET} (${RICH_PROBLEM})" >> ../skipped-tests.txt
  else
    echo "${PROBLEM}" >> ./.verify-helper/problems.txt
    echo "${TARGET} ${PROBLEM} ${LAST_MODIFIED_AT}" >> ./.verify-helper/tests.txt
  fi

  echo
} &>> ".log-${PID}.txt"

cat ".log-${PID}.txt"
