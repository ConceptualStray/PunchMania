#include <Arduino.h>

//uncomment to keep IDE happy
// fuck that
// #include "config.h"
// #include "ledControl.cpp"
// #include "serial.cpp"


uint8_t lastNotesIndex=0;

void loadNotesIntoMemory() {

	long baseTime=now();
	reinitTime();
	long updatedBaseTime=now();
	int hasSpaceOffset=0;
	for (size_t i = 0; i < MAX_MEM_NOTES; i++) {
		if (notesInMem[i].timestamp == -1) {
			hasSpaceOffset++;
			continue;
		}
		
		long timeDiff = baseTime - updatedBaseTime;
		notesInMem[i].timestamp -= timeDiff;
		notesInMem[i].changeOverTime -= timeDiff;

		// Shift to the left by available space
		if (hasSpaceOffset > 0) {
			int offsetIndex = i - hasSpaceOffset;
			if (offsetIndex >= 0) {
				notesInMem[offsetIndex] = notesInMem[i];
				notesInMem[i].timestamp = -1;
			}
		}
	}

	if(hasSpaceOffset>0){
		long accumulatedTime=now();
		int8_t memoryIndex=MAX_MEM_NOTES-hasSpaceOffset;
		for (size_t i = lastNotesIndex; i < MAX_NOTES and memoryIndex<MAX_MEM_NOTES; i++) {
			lastNotesIndex=i%MAX_NOTES;
			int8_t relativeTime = notes[i][0];

			if (relativeTime == -1) break;

			accumulatedTime += relativeTime;

			Note note;
			note.timestamp=accumulatedTime;
			note.ledId=0;
			note.groupId=getRandomGroupId();
			note.changeOverTime=accumulatedTime-relativeTime+LED_TIMESTEP-lookAhead;
			
			notesInMem[memoryIndex]=note;
			memoryIndex++;
			
		}
	}

	
}

void run(){

	loadNotesIntoMemory();
	
	long timenow=now();
	for (size_t i = 0; i < MAX_MEM_NOTES; i++){
		Note note=notesInMem[i];
		if(note.timestamp==-1)continue;
		if(note.changeOverTime>timenow){
			if(note.ledId>7){
				notesInMem[i].timestamp=-1;
				continue;
			}
			if(note.ledId>0){
				//if we are not at the beginning of the group, turn off the previous led
				toggleLed(ledIds[note.groupId][note.ledId]);
			}
			note.ledId++;
			toggleLed(ledIds[note.groupId][note.ledId]);
			int timestep=LED_TIMESTEP;
			if(note.ledId==7){
				timestep=LED_8_ON_DURATION;
			}
			note.changeOverTime=timenow+timestep;
			notesInMem[i]=note;
		}
	}
	
}


