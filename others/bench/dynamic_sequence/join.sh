#!/bin/bash

jq -s '[.[].benchmarks] | flatten' ./res/**.json
