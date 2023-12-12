TARGET="$1"

oj-verify run "$TARGET" --tle 5 || exit 1
cd .verify-helper
g++-12 -std=gnu++20 -MM "$TARGET"
echo >> ./timestamps.remote.json
cat ./timestamps.remote.json >> ./timestamps-"$NODE_ID".json
