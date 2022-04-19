import serial

import os
if os.path.exists("Received.txt"):
    os.remove("Received.txt")

com = input("Enter COM port number:\n")

mcu = serial.Serial("COM" + com, 19200, timeout = 0.1)

while(True):
	with open("Received.txt", 'a') as f:
		line = mcu.read()
		try:
			character = line.decode("utf-8")
			if(character==''):
				continue
			f.write(character)
			print(character,end = '')
			'''if character == '\n':
				print('\n')
				f.write('\n')'''
		except:
			continue