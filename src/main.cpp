#include <Arduino.h>
#include "config.h"
#include "ledControl.cpp"
#include "serial.cpp"
#include "program.cpp"

void selfTest(){
	for(int i=0;i<6;i++){
		toggleWholeGroup(i);
		updateShiftRegisters();
		delay(100);
	}
	delay(300);
	for(int i=0;i<6;i++){
		toggleWholeGroupOff(i);
	}
	updateShiftRegisters();
}

void testpopulate(){
	for(int i=0;i<40;i++){
		notes[i]=700;
	}
}

void setup(){
	Serial.begin(250000);
	pinMode(PIN_DATA, OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	
	//set all btn pins to inuput
	for(int i=0;i<6;i++){
		pinMode(PAD_TO_PIN[i], INPUT);
	}

	clearMemory();
	selfTest();
}

void loop() {
	readSerial();
	if(isPlaying){
		run();
		updateShiftRegisters();
	}
	
}

