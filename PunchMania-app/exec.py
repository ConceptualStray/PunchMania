import sys
import serial
import os
import time
import configparser
import pygame

if len(sys.argv) > 1:
	id = sys.argv[1]
else:
	print("false")
	exit()


COMID='COM10'
BAUDRATE=250000
ino = serial.Serial(COMID, BAUDRATE)


dirScript=os.path.dirname(__file__)
songFile=os.path.join(dirScript, './data/'+id, 'song.mp3')
configFile=os.path.join(dirScript, './data/'+id, 'config.ini')
notesFile=os.path.join(dirScript, './data/'+id, 'notes.ini')

with open(configFile, 'r') as f:
	config_string = '[default]\n' + f.read()


config = configparser.ConfigParser()
config.read_string(config_string)
config=config['default']



#we'll send that to the device
timeStart = int(config['startTime'])
duration = int(config['duration'])
timeEnd = timeStart+duration

offset=config['offset']
scanAhead=config['scanAhead']
allowDoubles=config['allowDoubles']

ino.write(('o'+str(offset)+'\n').encode())
#we can add support for rest of config later in development

notes = []
lastNoteTime=0
with open(notesFile, 'r') as f:
	for line in f:
		#cast line to int
		timestamp=int(line.strip());
		if timestamp>=timeStart and timestamp<=timeEnd:
			#calculate relative time between last note and this note
			relativeTime=timestamp-lastNoteTime
			lastNoteTime=timestamp
			notes.append(relativeTime)




#load first 100 notes
for i in range(min(100, len(notes))):
	ino.write(('n' + str(notes[i]) + '\n').encode())
	while True:
		response = ino.readline().strip()
		if response == b'1':
			del notes[i]
			break
		elif response == b'0':
			ino.write(('n' + str(notes[i]) + '\n').encode())
	

#here we start playing music and send the start key to ino
pygame.mixer.init()
pygame.mixer.music.load(songFile)

ino.write(('s\n').encode())

pygame.mixer.music.play(start=timeStart/1000)
pygame.time.delay(int(duration))  # delay in milliseconds
pygame.mixer.music.fadeout(500)


#we'll send the rest of the notes
for note in notes:
    while True:
        if ino.readline().strip() == b'2':
            break
	ino.write(('n' + str(note) + '\n').encode())
    while True:
        response = ino.readline().strip()
        if response == b'1':
            break
        elif response == b'0':
            ino.write(('n' + str(note) + '\n').encode())

#if all notes loaded then we wait for the song to finish
while pygame.mixer.music.get_busy(): 
    pygame.time.Clock().tick(10)

#if song ended delete the {id}.state file from /state dir
os.remove(os.path.join(dirScript, './state', id+'.state'))
#get points from ino and write to id.score file in /state dir

ino.write(('p\n').encode())
time.sleep(1)
points = ino.readline().strip().decode()

with open(os.path.join(dirScript, './points', id+'.score'), 'w') as f:
	f.write(points)
