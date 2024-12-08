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


	int availableSpace=0;


	for (size_t i = 0; i < MAX_MEM_NOTES; i++) {
		if (notesInMem[i].timestamp == -1) {

			availableSpace++;
			continue;
		}
		if (availableSpace > 0) {
			int offsetIndex = i - availableSpace;

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

			//notify controller app about "played" note and free global mem slot
			//return 2 to indicate that the note has been played, bc 1 is "understood" and 0 "not understood"
			Serial.println(2);
			
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

		// and if it's pressed we can toggle the whole group off
		if(activePads[i]!=-1){
			// Serial.println("Toggling group "+String(i));
			long localnow=now();
			if(activePads[i]<=localnow or (digitalRead(PAD_TO_PIN[i])==HIGH)){
				//get ms time diff between now and the time the led should be turned off
				//if greater than 0 we should award points based on time left, less time more points
				int pointsToAward=localnow-activePads[i];
				POINTS+=pointsToAward;
				// Serial.println("Points: "+String(pointsToAward));
				toggleLedOff(ledIds[i][7]);
				deregisterActivePad(i);
			}
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
		long localnow=now();
		// Serial.println("Now: "+String(now())+", "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
		if(note.timestamp<0)continue;
		// Serial.println("Now: "+String(now())+", "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
		// Serial.println("Changeover: "+String(note.changeOverTime));
		if(note.timestamp<=localnow and note.timestamp!=-1){
			toggleLedOff(getLedId(note.groupId, note.ledId));
			//toggle on the last led
			toggleLedOn(getLedId(note.groupId, 7));
			// Serial.println("Done: "+String(i)+": Timestamp: "+String(note.timestamp)+", LedId: "+String(note.ledId)+",GroupId:  "+String(note.groupId)+", ChangeoverTime: "+String(note.changeOverTime));
			registerActivePad(note.groupId);
			
			purgeNoteFromMemory(i);
			continue;
		}else if(note.changeOverTime<=localnow){
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


