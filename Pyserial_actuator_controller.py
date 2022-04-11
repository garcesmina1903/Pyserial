# -*- coding: utf-8 -*-
"""
Created on Fri Apr  1 22:45:03 2022

@author: kylec
"""

import serial

signal = serial.Serial("COM5", 9600)
print()
print('Welcome to actuator controller...')
print()

while True:
    userInput = input('What position do you want? up to 6inches, q to quit...')
    if userInput == 'q':
        print('GoodBye.')
        userInput = '0'
        signal.write(userInput.encode())
        break
    else:
        signal.write(userInput.encode())
        
 
signal.close()
    
    