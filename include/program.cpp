#include <Arduino.h>

//uncomment to keep IDE happy
// fuck that
// #include "config.h"
// #include "ledControl.cpp"
// #include "serial.cpp"


uint8_t lastNotesIndex=0;

void purgeNoteFromMemory(int index) {
	notesInMem[index].timestamp = -1;
	notesInMem[index].groupId = -1;
	notesInMem[index].ledId = -1;
	notesInMem[index].changeOverTime = -1;
}

void loadNotesIntoMemory() {

	int hasSpaceOffset=0;
	int availableSpace=0;


	for (size_t i = 0; i < MAX_MEM_NOTES; i++) {
		if (notesInMem[i].timestamp == -1) {
			hasSpaceOffset++;
			availableSpace++;
			continue;
		}
		if (hasSpaceOffset > 0) {
			int offsetIndex = i - hasSpaceOffset;
			// hasSpaceOffset--;
			if (offsetIndex >= 0) {
				notesInMem[offsetIndex] = notesInMem[i];
				purgeNoteFromMemory(i);
			}
		}
	}

	if(availableSpace>0){
		int8_t memoryIndex=MAX_MEM_NOTES-availableSpace;
		for (size_t i = lastNotesIndex; i < MAX_NOTES and memoryIndex<MAX_MEM_NOTES; i++) {
			int relativeTime = notes[i];
			
			if (relativeTime<=0) break;

			
			notes[i]=0;

			TIME_NOTES_AGGREGATED += relativeTime;

			Note note;
			note.timestamp=TIME_NOTES_AGGREGATED;
			note.ledId=-1;
			note.groupId=getRandomGroupId();
			// note.changeOverTime=TIME_NOTES_AGGREGATED-TIME_SCAN_AHEAD;
			note.changeOverTime=TIME_NOTES_AGGREGATED-TIME_SCAN_AHEAD+DURATION_LED_SUB;
			// Serial.println("Loading into mem: "+String(i)+": "+String(note.timestamp)+", "+String(note.ledId)+", "+String(note.groupId)+", "+String(note.changeOverTime));
			// Serial.println();
			notesInMem[memoryIndex]=note;

			memoryIndex++;
			lastNotesIndex=(i+1)%MAX_NOTES;
			
			
		}
	}

}

void registerActivePad(int8_t groupId){
	activePads[groupId]=now()+DURATION_LED_MAIN;
}

void deregisterActivePad(int8_t groupId){
	activePads[groupId]=-1;
}

void run(){
	//6 is the number of groups/max active pads
	for (size_t i = 0; i < 6; i++){
		//int this loop we can also add later check for btn press
		// and if it's pressed we can toggle the whole group off
		if(activePads[i]!=-1 and activePads[i]<=now()){
			// Serial.println("Toggling group "+String(i));
			toggleLedOff(ledIds[i][7]);
			deregisterActivePad(i);
		}
	}
	
	loadNotesIntoMemory();
	
	//dump mempory to serial for testing
	// Serial.println("Memory:");
	// for (size_t i = 0; i < MAX_MEM_NOTES; i++){
	// 	Note note=notesInMem[i];
	// 	Serial.println("Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
	// }
	// Serial.println("================\n\n");

	for (size_t i = 0; i < MAX_MEM_NOTES; i++){
		Note note=notesInMem[i];
		// Serial.println("Now: "+String(now())+", "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
		if(note.timestamp<0)continue;
		// Serial.println("Now: "+String(now())+", "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
		// Serial.println("Changeover: "+String(note.changeOverTime));
		if(note.timestamp<=now() and note.timestamp!=-1){
			toggleLedOff(getLedId(note.groupId, note.ledId));
			//toggle on the last led
			toggleLedOn(getLedId(note.groupId, 7));
			// Serial.println("Done: "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
			registerActivePad(note.groupId);
			purgeNoteFromMemory(i);
			continue;
		}else if(note.changeOverTime<=now()){
			if(note.ledId!=-1){
				//if we are not at the beginning of the group, turn off the previous led
				toggleLedOff(getLedId(note.groupId,note.ledId));
			}
			note.ledId++;
			toggleLedOn(getLedId(note.groupId,note.ledId));
			// Serial.println("Toggling: "+String(note.ledId)+", "+String(note.groupId)+" global index of: "+String(getLedId(note.groupId,note.ledId)));
			note.changeOverTime=now()+DURATION_LED_SUB;
			notesInMem[i]=note;
		}
	}
	// Serial.println("=======================\n");
}


