#include <Arduino.h>

//uncomment to keep IDE happy
// fuck that
// #include "config.h"
// #include "ledControl.cpp"
// #include "serial.cpp"


uint8_t lastNotesIndex=0;

void loadNotesIntoMemory() {

	int hasSpaceOffset=0;
	int availableSpace=0;


	for (size_t i = 0; i < MAX_MEM_NOTES; i++) {
		if (notesInMem[i].timestamp == -1) {
			// Serial.println("Full, break");
			hasSpaceOffset++;
			availableSpace++;
			continue;
		}
		if (hasSpaceOffset > 0) {
			// Serial.println(hasSpaceOffset);
			int offsetIndex = i - hasSpaceOffset;
			hasSpaceOffset--;
			if (offsetIndex >= 0) {
				notesInMem[offsetIndex] = notesInMem[i];
				notesInMem[i].timestamp = -1;
			}
		}
	}
	// Serial.println("Loading notes into memory");
	// Serial.println("Available space: "+String(availableSpace));
	if(availableSpace>0){
		int8_t memoryIndex=MAX_MEM_NOTES-availableSpace;
		for (size_t i = lastNotesIndex; i < MAX_NOTES and memoryIndex<MAX_MEM_NOTES; i++) {
			lastNotesIndex=i%MAX_NOTES;
			int relativeTime = notes[i];

			if (relativeTime <0) break;
			// Serial.println("Relative time: "+String(relativeTime));
			// Serial.println("Accumulated time: "+String(accumulatedTime));
			lastNotesTime += relativeTime;

			Note note;
			note.timestamp=lastNotesTime;
			note.ledId=0;
			note.groupId=getRandomGroupId();
			note.changeOverTime=lastNotesTime-lookAhead-relativeTime+LED_TIMESTEP;
			Serial.println("Loading into mem: "+String(i)+": "+String(note.timestamp)+", "+String(note.ledId)+", "+String(note.groupId)+", "+String(note.changeOverTime));
			notesInMem[memoryIndex]=note;

			memoryIndex++;
			
		}
	}

	
}

void run(){

	loadNotesIntoMemory();
	
	for (size_t i = 0; i < MAX_MEM_NOTES; i++){
		Note note=notesInMem[i];
		
		if(note.timestamp<0)continue;
		// if(i==0)Serial.println("Now: "+String(now())+", "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
		// Serial.println("Changeover: "+String(note.changeOverTime));
		if(note.changeOverTime<=now()){
			if(note.ledId>7){
				// Serial.println("End of group "+String(i));
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
			note.changeOverTime=now()+timestep;
			notesInMem[i]=note;
		}
	}
	
}


