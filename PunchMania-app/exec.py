import sys
import serial
import configparser

if len(sys.argv) > 1:
	id = sys.argv[1]
else:
	print("false")
	exit()

with open('config.ini', 'r') as f:
    config_string = '[default]\n' + f.read()



config = configparser.ConfigParser()
config.read_string(config_string)
config=config['default']

#we'll send that to the device
timeStart = config['startTime']
duration = config['duration']
timeEnd = timeStart+duration

offset=config['offset']
scanAhead=config['scanAhead']
allowDoubles=config['allowDoubles']


notes = []
uploadTimes=[]
timeNotesAbsolute=0
timeNotesStart=0
timeStartLeadTime=1000
i=0;
with open('notes.ini', 'r') as f:
    for line in f:
		#cast line to int
		timestamp=int(line.strip());
		timeNotesAbsolute+=timestamp
		if timeNotesAbsolute>=timeStart and timeNotesAbsolute<=timeEnd and timeNotesStart>=timeStartLeadTime:
			timeNotesStart+=timestamp
			notes.append(timestamp)
			i+=1
			if i==50:
				uploadTimes.append(timeNotesStart)
				i=0
		

ino = serial.Serial('COM10', 250000)

def sendNotes():


#load first 100 notes
for i in range(min(100, len(notes))):
    ino.write(str(notes[i]).encode())
    ino.write(b'\n')
	
