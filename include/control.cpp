#include <Arduino.h>
#include "serial.cpp"



const char* validCommandPointers[] = {"c", "n", "m", "d"};
const int cPointersC=4;

void processCommands(){
	if (Serial.available() > 0){
		String commandPointer = Serial.readStringUntil('.');
		bool validCommandPointer=false;
		for(int i=0;i<cPointersC;i++){
			if(commandPointer.equals(validCommandPointers[i])){
				validCommandPointer=true;
				break;
			}
		}
		if(validCommandPointer==true){
			commandParse(commandPointer,Serial.readStringUntil('\n'));
		}else{
			Serial.println("eUcp: "+commandPointer);
			//clear the buffer
			while (Serial.available() > 0) {
				Serial.read();
			}
		}

	}
}


