import sys
import serial
import os
import time
import configparser
import pygame
import random

if len(sys.argv) > 1:
    id = sys.argv[1]
else:
    print("false")
    exit()

# id=1.l

COMID='COM7'
BAUDRATE=250000
ino = serial.Serial(COMID, BAUDRATE)


dirScript=os.path.dirname(__file__)
songFile=os.path.join(dirScript, './data/'+str(id), 'song.mp3')
configFile=os.path.join(dirScript, './data/'+str(id), 'config.ini')
notesFile=os.path.join(dirScript, './data/'+str(id), 'notes.ini')

with open(configFile, 'r') as f:
    config_string = '[default]\n' + f.read()


config = configparser.ConfigParser()
config.read_string(config_string)
config=config['default']

timeStart = int(config['startTime'])
duration = int(config['duration'])
timeEnd = timeStart+duration

offset=int(config['offset'])
scanAhead=int(config['scanAhead'])
allowDoubles=int(config['allowDoubles'])

ino.write(('o'+str(offset)+'\n').encode())
#we can add support for rest of config later in development

notes = []
lastNoteTime=0
firstCumulative=0;
with open(notesFile, 'r') as f:
    for line in f:
        #cast line to int
        timestamp=int(line.strip());
        if timestamp>=timeStart and timestamp<=timeEnd:
            #calculate relative time between last note and this note
            relativeTime=timestamp-lastNoteTime
            lastNoteTime=timestamp
            # print(f"Converting {timestamp} to int: {relativeTime}")
            notes.append(relativeTime)

if(lastNoteTime<timeEnd):
    timeEnd=lastNoteTime

time.sleep(3)
initLoops=49;

#gen random int
random.seed(time.time())
seed=random.random();
#cast that float to int
seed=int(seed*1000000);

ino.write(('c'+str(seed)+'\n').encode())



if(len(notes)<initLoops):
	initLoops=len(notes)
#load first x notes

for i in range(min(initLoops, len(notes))):
    timestamp=notes[i]
    ino.write(('n' + str(timestamp) + '\n').encode())
    time.sleep(0.1)
    while True:
        response = ino.readline().strip()
        response=int(response.decode('utf-8'))
        print(f"Response {response} timestamp {timestamp}")
        if response == 1:
            break
        else:
            time.sleep(0.1)
    
del notes[:initLoops]


#here we start playing music and send the start key to ino
pygame.mixer.init()
pygame.mixer.music.load(songFile)

ino.write(('s\n').encode())
# time.sleep(1.5)
pygame.mixer.music.play(start=timeStart/1000)
#get now in ms
startTime=time.time()*1000


# pygame.time.delay(int(duration))  # delay in milliseconds



#we'll send the rest of the notes
print("Sending remaining notes")
for note in notes:
    while True:
        if ino.readline().strip() == b'2':
            break
    ino.write(('n' + str(note) + '\n').encode())
    while True:
        response = ino.readline().strip()
        print(f"Response {response} timestamp {note}")
        if response == b'1':
            break
        elif response == b'0':
            time.sleep(0.1)
            ino.write(('n' + str(note) + '\n').encode())

#if all notes loaded then we wait for the song to finish.
print("All notes loaded")
while True:
    time.sleep(0.5)
    print("Duration: " + str(duration)+"now "+str(time.time()*1000 - startTime))
    if (time.time()*1000 - startTime) >= timeEnd:
        print("Song finished")
        break

print("Song ended")
pygame.mixer.music.fadeout(7000)

time.sleep(3)
ino.write(('d\n').encode())
time.sleep(4)
#if song ended delete the {id}.state file from /state dir
os.remove(os.path.join(dirScript, './state', id+'.state'))
#get points from ino and write to id.score file in /state dir

ino.write(('p\n').encode())
time.sleep(1)
points = ino.readline().strip().decode()

with open(os.path.join(dirScript, './points', id+'.score'), 'w') as f:
    f.write(points)
