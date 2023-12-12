BASE_DIR="$PWD"
TARGET="$1"


echo "$BASE_DIR"
g++-12 -std=gnu++20 -MM -I"$BASE_DIR" "$TARGET"

oj-verify run "$TARGET" --tle 5 || exit 1
cd .verify-helper
echo >> ./timestamps.remote.json
cat ./timestamps.remote.json >> ./timestamps-"$NODE_ID".json
