import serial

COMID='COM4'
BAUDRATE=250000
ino = serial.Serial(COMID, BAUDRATE)

exit();