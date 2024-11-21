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

notes = []
uploadTimes=[]
timeNotesAbsolute=0
timeNotesStart=0
timeStartLeadTime=1000
i=0;
with open(notesFile, 'r') as f:
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

# def sendNotes():


#load first 100 notes
for i in range(min(100, len(notes))):
	ino.write(str(notes[i]).encode())
	ino.write(b'\n')
	del notes[i]

#send upload times
def sendNotes(notes,serial):
	c=0
	for i,note in enumerate(notes):
		if c==50:
			return
		serial.write((str(note)+"\n").encode())
		del notes[i]
		c+=1

#here we start playing music and send the start key to ino
pygame.mixer.init()
pygame.mixer.music.load(songFile)
pygame.mixer.music.play(start=timeStart/1000)
pygame.time.delay(int(duration))  # delay in milliseconds
pygame.mixer.music.fadeout(500)


tindex=0
while len(notes)>0:
	diff=time.time()-timeStart
	if diff>=uploadTimes[tindex]:
		sendNotes(notes,ino)
		del uploadTimes[tindex]
		tindex+=1
	time.sleep(1)


#if all notes loaded then we wait for the song to finish
while pygame.mixer.music.get_busy(): 
    pygame.time.Clock().tick(10)