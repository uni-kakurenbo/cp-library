import json
import sys


def debug(*args, **opts):
    print(*args, **opts, file=sys.stderr)


Res = {}
Sizes = set()

data = json.load(sys.stdin)

for item in data:
    name: str = item["name"]
    if not name.endswith("mean"): continue

    info = name.split("/")

    tag = info[0]
    sz = int(info[1][:info[1].find("_")])

    if tag not in Res:
        Res[tag] = {}

    Res[tag][sz] = item["cpu_time"]
    Sizes.add(sz)

Sizes = [*sorted([*Sizes])]

Data = [[
    "size",
    *map(lambda x: 2**x, Sizes),
]]

for tag, vals in Res.items():
    Data.append([tag])
    for sz in Sizes:
        if sz not in vals: break
        Data[-1].append(vals[sz])

l = max(map(len, Data))
Data = [*map(lambda x: x + [""] * (l - len(x)), Data)]

for line in zip(*Data):
    print(",".join(map(str, line)))
