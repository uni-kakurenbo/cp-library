#!/bin/bash

./0000.exe --benchmark_report_aggregates_only=true --benchmark_time_unit=ms --benchmark_out=res.json
pypy3 sum.py >sum.csv
