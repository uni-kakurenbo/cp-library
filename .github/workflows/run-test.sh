TARGET="$1"

oj-verify run "$TARGET" --tle 5 || exit 1
echo "$TARGET"
cd .verify-helper
echo >> ./timestamps.remote.json
cat ./timestamps.remote.json >> ./timestamps-"$NODE_ID".json
