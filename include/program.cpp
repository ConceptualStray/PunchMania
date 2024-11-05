#include <Arduino.h>

//reverse logic here,
//main loop should go thru notes in mem and helper func that populates that mem

void purgeNoteFromMemory(uint8_t i){
	Note note;
	note.timestamp=-1;
	notesInMem[i]=note;
}

uint8_t lastNotesIndex=0;

void loadNotesIntoMemory() {
    // Clear memory
    clearMemory();
	reinitTime();
    // Load notes into memory
    long accumulatedTime = now();
    for (size_t i = lastNotesIndex; i < maxNotesInMem; i++) {
		lastNotesIndex=i%maxNotesInMem;
        int8_t relativeTime = notes[i][0];

        if (relativeTime == -1) break;

        accumulatedTime += relativeTime;

		Note note;
		note.timestamp=accumulatedTime;
		note.ledId=0;
		note.groupId=getRandomGroupId();
		note.changeOverTime=accumulatedTime-relativeTime+LED_TIMESTEP-lookAhead;
		
		notesInMem[i]=note;
		
    }
	
}

void run(){
	//figure out if we need to populate notes in memory
	if(lastNotesIndex==0){
		loadNotesIntoMemory();
	}
	long timenow=now();
	for (size_t i = 0; i < maxLookahead; i++){
		Note note=notesInMem[i];
		if(note.timestamp==-1)continue;
		if(note.changeOverTime>timenow){
			if(note.ledId>7){
				purgeNoteFromMemory(i);
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


