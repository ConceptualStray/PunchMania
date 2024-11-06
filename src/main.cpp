#include <Arduino.h>
#include "config.h"
#include "ledControl.cpp"
#include "serial.cpp"
#include "program.cpp"



void setup(){
	Serial.begin(250000);	  // Read seed from EEPROM
	randomSeed(1354815138);

	randomSeed(analogRead(0));
	//shift registers pins setup
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	// zero all the leds and SRs
	updateShiftRegisters();

	clearMemory();
}



long lastCountDownTime = 0;
int currentGroupId = 0;
int prevGroupId=-1;
void loop() {
	readSerial();
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
			run();
		}
	}
	
}

