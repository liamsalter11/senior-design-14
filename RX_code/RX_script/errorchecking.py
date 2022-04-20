import os.path
from os import path

def readCheckFiles(master, check):        
    with open(master) as f:
        masterText = f.readlines()
        
    with open(check) as f:
        checkText = f.readlines()
        
    if (len(masterText) != len(checkText)):
        print("Files have different number of lines!")
        
        return [], []

    return masterText, checkText
    
def getLineSymbolErrors(line1, line2):
    if (len(line1) != len(line2)):
        print("Line lengths not equal!!")
        print(line1)
        print(line2)
        return 0
    return sum([a != b for a,b in zip(line1, line2)])


def errorRateCalculation(master="", check=""):
    if (len(master)==0):
        master = input("Enter path of master file: ")
    while os.path.exists(master) == False:
        master = input("Invalid file Path! Enter path of master file: ")
            
    if (len(check)==0):
        check = input("Enter path of master file: ")
    while os.path.exists(check) == False:
        check = input("Invalid file Path! Enter path of master file: ")
            
    txLines, rxLines = readCheckFiles(master, check)
    totalSize = sum([len(a) for a in txLines])
    if totalSize == 0:
        return
    errors = sum([getLineSymbolErrors(a,b) for a,b in zip(txLines, rxLines)])
    print()
    print(errors, "symbol errors found in", totalSize, "symbols.")
    print("{:.3%}".format(errors / totalSize), "error rate")
    
if __name__ == "__main__":
    errorRateCalculation()