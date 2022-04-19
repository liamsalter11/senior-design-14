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
    masterLine = masterLines[i].strip()
    receivedLine = receivedLines[i].strip()

    last = False
    if receivedLine == receivedLines[-1].strip():
        last = True
    
    sizeDif = len(masterLine) - len(receivedLine)
    lineSize = 0
    if sizeDif < 0:
        sizeDif = abs(sizeDif)
        lineSize = len(masterLine)
    else:
        lineSize = len(receivedLine)
    
    if not last:
        errors += sizeDif
    totalSize += len(masterLine)
    
    for j in range(lineSize):
        if masterLine[j] != receivedLine[j]:
            shifted = False
            for k in range(sizeDif):
                if len(masterLine) > (j + k):
                    break
                if masterLine[j + k] == receivedLine[j]:
                    shifted = True
                    break
            
            if shifted or last:
                errors += 1

print()
print(errors, " errors found.")
print("{:.3%}".format(errors / totalSize), "error rate")
input()