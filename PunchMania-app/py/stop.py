import serial

COMID='COM7'
BAUDRATE=250000
ino = serial.Serial(COMID, BAUDRATE)

exit();