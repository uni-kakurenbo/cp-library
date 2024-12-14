#!/bin/bash

./join.sh >./sum.json

pypy3 sum.py <./sum.json >./sum.csv
