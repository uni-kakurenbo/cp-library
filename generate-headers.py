from collections import defaultdict
from glob import glob
from pathlib import Path

OUTPUT_DIRECTORY = Path("./include")

INDIVIDUAL = {
    "action": "actions",
    "adaptor": "adaptors",
    "algebraic": "algebraic",
    "convolution": "convolutions",
    "data_structure": "data_structures",
    "geometry": "geometries",
    "graph": "graph_theory",
    "hash": "hashes",
    "iterable": "iterable",
    "numeric": "numeric",
    "snippet": "snippets",
    "view": "views"
}

ALL = ["random", "utility", "global", "structure"]


def generate(dir):
    content = defaultdict(list)

    for file in sorted(glob(f"./{ dir }/**/*.hpp", recursive=True)):
        file = Path(file)
        category = file.relative_to(dir).parent.name
        if category == "internal": continue
        if not category: category = "|"
        content[category].append(f"#include \"{ file }\"\n")

    return "\n".join(map(lambda key: "".join(content[key]), sorted(content.keys())))


for dir, header in INDIVIDUAL.items():
    dir = Path(dir)

    output = open(OUTPUT_DIRECTORY / f"{ header }.hpp")
    output_file = open(output.name, "w")

    print(dir)
    output_file.write(f"#pragma once\n\n{ generate(dir) }")

print("all")

output_all = open(OUTPUT_DIRECTORY / "all.hpp")
output_file_all = open(output_all.name, "w")

content_all = "#pragma once\n\n"
for dir in map(Path, INDIVIDUAL.values()):
    content_all += f"#include \"{ OUTPUT_DIRECTORY / dir }.hpp\"\n"

content_all += "\n"

for dir in map(Path, ALL):
    content_all += generate(dir) + "\n"

output_file_all.write(content_all)
