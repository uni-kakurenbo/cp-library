import json

Res = {}
Sizes = set()

with open('./res.json') as f:
    data = json.load(f)

    for item in data["benchmarks"]:
        name: str = item["name"]
        if not name.endswith("mean"): continue

        info = name.split("/")

        tag = info[0]
        size = info[1]

        if tag not in Res:
            Res[tag] = []

        Res[tag].append(item["cpu_time"])
        Sizes.add(2**int(size))

Data = [["size", *Sizes]]

for tag, vals in Res.items():
    Data.append([tag, *vals])

l = max(map(len, Data))
Data = [*map(lambda x: x + [""] * (l - len(x)), Data)]

for line in zip(*Data):
    print(",".join(map(str, line)))
