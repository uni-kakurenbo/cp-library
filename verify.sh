#! /bin/bash

dropbox_token="$(
    curl https://api.dropbox.com/oauth2/token \
        -d grant_type=refresh_token \
        -d "$DROPBOX_REFRESH_TOKEN" \
        -u "$DROPBOX_APP_KEY":"$DROPBOX_APP_SECRET" \
        -s \
    | jq -r ".access_token"
)"

export DROPBOX_TOKEN=$dropbox_token



cd "$CPLUS_INCLUDE_PATH" || exit
ls -al


cd "$GITHUB_WORKSPACE" || exit
oj-verify --config-file "./.verify-helper/remote-config.toml" all
