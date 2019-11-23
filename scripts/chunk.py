import os, sys

lines = []
n = 2500
with open("store.raw.data") as f:
    x = "".join(f.readlines())
    lines = [x[i:i+n] for i in range(0, len(x), n)]
    for line in lines:
        print("+ std::string(\"" + line + "\")")
