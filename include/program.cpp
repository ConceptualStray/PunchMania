#include <Arduino.h>

//uncomment to keep IDE happy
// fuck that
// #include "config.h"
// #include "ledControl.cpp"
// #include "serial.cpp"


uint8_t lastNotesIndex=0;

void loadNotesIntoMemory() {

	long baseTime=now();
	Serial.println("Base time: "+String(baseTime));
	reinitTime();
	long updatedBaseTime=now();
	Serial.println("Updated base time: "+String(updatedBaseTime));
	int hasSpaceOffset=0;
	int availableSpace=0;

	Serial.println("Adjusting timestamps");
	for (size_t i = 0; i < MAX_MEM_NOTES; i++) {
		if (notesInMem[i].timestamp == -1) {
			// Serial.println("Full, break");
			hasSpaceOffset++;
			availableSpace++;
			continue;
		}
		
		long timeDiff = baseTime - updatedBaseTime;
		Serial.println("Timediff: "+String(timeDiff));
		notesInMem[i].timestamp += timeDiff;
		notesInMem[i].changeOverTime += timeDiff;
		// Shift to the left by available space
		if (hasSpaceOffset > 0) {
			Serial.println(hasSpaceOffset);
			int offsetIndex = i - hasSpaceOffset;
			hasSpaceOffset--;
			if (offsetIndex >= 0) {
				notesInMem[offsetIndex] = notesInMem[i];
				notesInMem[i].timestamp = -1;
			}
		}
	}
	Serial.println("Loading notes into memory");
	Serial.println("Available space: "+String(availableSpace));
	if(availableSpace>0){
		long accumulatedTime=now();
		int8_t memoryIndex=MAX_MEM_NOTES-availableSpace;
		for (size_t i = lastNotesIndex; i < MAX_NOTES and memoryIndex<MAX_MEM_NOTES; i++) {
			lastNotesIndex=i%MAX_NOTES;
			int relativeTime = notes[i];

			if (relativeTime <0) break;
			Serial.println("Relative time: "+String(relativeTime));
			Serial.println("Accumulated time: "+String(accumulatedTime));
			accumulatedTime += relativeTime;

			Note note;
			note.timestamp=accumulatedTime;
			note.ledId=0;
			note.groupId=getRandomGroupId();
			note.changeOverTime=accumulatedTime-relativeTime+LED_TIMESTEP-lookAhead;
			Serial.println("Loading into mem: "+String(i)+": "+String(note.timestamp)+", "+String(note.ledId)+", "+String(note.groupId)+", "+String(note.changeOverTime));
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
		Serial.println(String(i)+": "+String(note.timestamp)+", "+String(note.ledId)+", "+String(note.groupId)+", "+String(note.changeOverTime));
		if(note.timestamp==-1)continue;
		Serial.println("Now: "+String(timenow));
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


