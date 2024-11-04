#include <Arduino.h>
// #include "digitalWriteFast.h"
#include "config.h"
#include "ledControl.cpp"
#include "control.cpp"
#include "program.cpp"
// #include "notes.cpp"



void setup(){
	Serial.begin(9600);	  // Read seed from EEPROM
	randomSeed(1354815138);

	randomSeed(analogRead(0));
	//shift registers pins setup
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	// zero all the leds and SRs
	updateShiftRegisters();

    for (int i = 7; i < maxNotesInMem; i++) {
        notes[i][0] = -1; // Set timestamp to -1 to indicate unused
        notes[i][1] = -1;   // Or any default value you need for the second column
    }

	for (int j = 0; j < maxLookahead; j++) {
        notesInMem[j][0] = -1;  // Set an unused marker
        notesInMem[j][1] = -1;  // Set an unused marker
        notesInMem[j][2] = -1;  // Set an unused marker
        notesInMem[j][3] = -1;  // Set an unused marker
    }
}



long lastCountDownTime = 0;
int currentGroupId = 0;
int prevGroupId=-1;
void loop() {
	processCommands();
	if(isPlaying){
		if(isInCountdown){
			float groupTime=(startOffset-offset)/6;
            if (millis() - lastCountDownTime >= groupTime) {
				if(currentGroupId < 6)toggleWholeGroup(currentGroupId);  
				if(prevGroupId != -1)toggleWholeGroup(prevGroupId);        
                lastCountDownTime = millis();
				prevGroupId=currentGroupId;
                currentGroupId = (currentGroupId + 1);
                if (currentGroupId == 7){
					isInCountdown = false;
				}
			}
		}else{
			if(zeroTime<millis()){
				// processNotes();
				//what
				for (size_t i = 0; i < 6; i++){
					
					for (size_t j = 0; j < 8; j++){
						toggleLed(ledIds[i][j]);
						int delayT=20;
						if(j==7){
							delayT=100;
						}
						delay(delayT);
						toggleLed(ledIds[i][j]);
					}
					// toggleWholeGroup(i);
				}
				
			}
		}
		// updateShiftRegisters();
	}
	
}

