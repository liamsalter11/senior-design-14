import os.path
from os import path

path = input("Enter file path of text document to strip:\n")

if os.path.exists(path):
    f = open(path, 'r').readlines()
else:
    print("Invalid File Path")

split = [line for line in f if line.strip()]
split.append('')

path = path.split(".")
f = open(path[0] + "_stripped." + path[1], 'w').writelines(split)
