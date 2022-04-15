from email import message
import serial
import time
import os.path
from os import path

com = input("Enter COM port number:\n")

mcu = serial.Serial("COM" + com, 19200, timeout = 0.1)

while(True):
    choice = input("\nPlease select:\n\t(1) Text Input\n\t(2) File Input\n\t(3) Exit\n")
    if choice == '2':
        path = input("\nEnter file path of text document to transmit:\n")
        if os.path.exists(path):
            f = open(path, 'r').readlines()
        else:
            print("Invalid File Path")
            continue

        for line in f:
            mcu.write(line.encode())
            time.sleep(0.1)
    elif choice == '1':
        textInput = input("Please enter text:\n") + '\n'
        mcu.write(textInput.encode())
        time.sleep(0.1)
    else:
        break
    print('\n')
