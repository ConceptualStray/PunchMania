import sys
import serial
import os
import time
import configparser
import pygame

# if len(sys.argv) > 1:
#     id = sys.argv[1]
# else:
#     print("false")
#     exit()

id=1

COMID='COM7'
BAUDRATE=250000
ino = serial.Serial(COMID, BAUDRATE)

exit();