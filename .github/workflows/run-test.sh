BASE_DIR="$PWD"
TARGET="$1"

echo "$BASE_DIR"
DEPENDENCIES=$(g++-12 -std=gnu++20 -MM -I"$BASE_DIR" "$TARGET")
LAST_COMMIT="$(git log -1 --date=iso --pretty=%ad -- $DEPENDENCIES)"
date --date "LAST_COMMIT" "+%s"

oj-verify run "$TARGET" --tle 5 || exit 1
cd .verify-helper
echo >> ./timestamps.remote.json
cat ./timestamps.remote.json >> ./timestamps-"$NODE_ID".json
