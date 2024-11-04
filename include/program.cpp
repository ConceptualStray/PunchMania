#include <Arduino.h>

//reverse logic here,
//main loop should go thru notes in mem and helper func that populates that mem

void purgeNoteFromMemory(uint8_t i){
	notesInMem[i][0]=-1;
	notesInMem[i][1]=-1;
	notesInMem[i][2]=-1;
	notesInMem[i][3]=-1;
}

void updateNoteInMemory(int8_t nextLedIndex, int16_t i, long timeNow){
	for (size_t j = 0; j < maxLookahead; j++){
		if(notesInMem[j][0]==i){
			notesInMem[j][1]=nextLedIndex;
			notesInMem[j][2]=timeNow+notesInMem[j][3];
			break;
		}
	}
	
}

void getNoteInfoFromMemory(int8_t &ledIndexToLight, long &changeOverTime, int16_t i, long timeNow){
	uint8_t freeIndex=0;
	bool found=false;
	for (size_t j = 0; j < maxLookahead; j++){
		if(notesInMem[j][0]==-1){
			freeIndex=j;
			continue;
		}
		//if found
		if(notesInMem[j][0]==i){
			found=true;
			//get last led id and changeover time
			ledIndexToLight=notesInMem[j][1];
			changeOverTime=notesInMem[j][2];
			// long timeStep=notesInMem[j][3];
			break;
		}
	}
		//note not found, add to first free index
	if(!found){
		unsigned long timeStep=static_cast<long>(lookAhead / 7);
		ledIndexToLight=1;
		changeOverTime = timeNow + timeStep;
		notesInMem[freeIndex][0]=i;
		notesInMem[freeIndex][1]=ledIndexToLight;
		notesInMem[freeIndex][2]=changeOverTime;
		notesInMem[freeIndex][3]=timeStep;
	}


}

long now(){
	return (millis() - zeroTime)-offset;
}

uint8_t lastNotesIndex=0;
uint8_t lastMemIndex=0;

void processNotes(){
	long timeNow=now();
	long lookAheadEnd = now() + lookAhead;
	//fist of all we need to get last
	//index offset so we don't start processing from the start
	for (size_t i = lastNotesIndex; i < maxNotesInMem; i++){
		long noteTimestamp=notes[i][0];
		uint8_t noteGroupId=notes[i][1];
		if(noteGroupId==-1 or noteGroupId==255){
			int randGroupId=getRandomGroupId();
			notes[i][1]=randGroupId;
			noteGroupId=randGroupId;
		}
		Serial.println("");
		Serial.println("=========================");

		//quick check for disabling led 8 on old notes
		if((noteTimestamp+LED_8_ON_DURATION)<timeNow and noteTimestamp>=0){
			toggleLed(ledIds[noteGroupId][7]);
			notes[i][0]=-1;
			notes[i][1]=-1;
			lastNotesIndex=i+1;
			purgeNoteFromMemory(i);
			Serial.println("Zeroing note  "+String(i)+" with group "+String(noteGroupId)+" @ "+String(timeNow));
			continue;
		}
		
		//break out if no more valid notes
		// if(noteTimestamp==-1 or noteTimestamp>lookAheadEnd)break;
		//what we're left with is either notes that are incoming in the lookahead window

		Serial.println("Processing note "+String(i)+" with timestamp "+String(noteTimestamp)+" and group id "+String(noteGroupId));
			
		//assign group id to the note if it has none already

		
		//seach for note in notesInMem array

		int8_t ledIndexToLight=-1;
		long changeOverTime=0;
		getNoteInfoFromMemory(ledIndexToLight,changeOverTime,i,timeNow);
		if(changeOverTime>timeNow){
			if(ledIndexToLight>0){
				Serial.println("Disabling led "+String(ledIndexToLight)+" on group "+String(noteGroupId));
				toggleLed(ledIds[noteGroupId][ledIndexToLight]);
			}
			ledIndexToLight++;
			Serial.println("Led "+String(ledIndexToLight)+" on group "+String(noteGroupId));
			toggleLed(ledIds[noteGroupId][ledIndexToLight]);
			updateNoteInMemory(ledIndexToLight,i,timeNow);
		}
	}
	
}



