import os.path
from os import path

goodFile = input("Enter path of master file:\n")
while os.path.exists(goodFile) == False:
    goodFile = input("Invalid file Path! Enter path of master file:\n")

receivedFile = input("Enter path of received file:\n")
while os.path.exists(receivedFile) == False:
    receivedFile = input("Invalid file Path! Enter path of received file:\n")

with open(goodFile) as f:
    masterLines = f.readlines()
with open(receivedFile) as f:
    receivedLines = f.readlines()

masterLines = masterLines[:len(receivedLines)]

errors = 0
totalSize = 0
for checkLine, receivedLine in zip(masterLines, receivedLines):
    errors = errors + sum([a != b for a,b in zip(checkLine, receivedLine)])
    totalSize = len(checkLine)

print()
print(errors, " symbol errors found.")
print("{:.3%}".format(errors / totalSize), "error rate")