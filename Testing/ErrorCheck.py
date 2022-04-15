import os.path
from os import path

goodFile = input("Enter path of master file:\n")
while os.path.exists(goodFile) == False:
    goodFile = input("Invalid file Path! Enter path of master file:\n")

if os.path.exists("Received.txt"):
    receivedFile = "Received.txt"
else:
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
for i in range(len(masterLines)):
    masterLine = masterLines[i]
    receivedLine = receivedLines[i]
    
    sizeDif = len(masterLine) - len(receivedLine)
    lineSize = 0
    if sizeDif < 0:
        sizeDif = abs(sizeDif)
        lineSize = len(masterLine)
    else:
        lineSize = len(receivedLine)
    errors += sizeDif
    totalSize += lineSize
    
    for j in range(lineSize):
        if masterLine[j] != receivedLine[j]:
            errors += 1

print()
print(errors, " errors found.")
print("{:.3f}".format(100 * errors / totalSize), "% error rate")
