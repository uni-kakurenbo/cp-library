#!/bin/bash

# ./0000.exe --benchmark_time_unit=ms --benchmark_out=res.json --benchmark_repetitions=256 "$@"
# cp res.json res-00.json

items=(
    # "treap-A"
    # "treap-B"
    "rbt-A"
    "rbt-B"
    "per-rbt-A"
    "per-rbt-B"
)

for i in {0..15}; do
    for item in "${items[@]}"; do
        file="./res/${item}.json"
        echo "${item}"

        if [[ -e "${file}" ]]; then
            {
                pypy3 sum.py <"${file}" >/dev/null
            } && continue
        fi

        echo "${file}"

        echo "${i}"

        ./0000.exe --benchmark_time_unit=ms --benchmark_filter="^${item}" --benchmark_out=res.json --benchmark_repetitions=256 "$@"
        cp res.json "${file}"
    done
done
