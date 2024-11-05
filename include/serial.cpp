#include <Arduino.h>


const int maxCommandLength = 10;
void processCommand(char pointer, const char* command);
void cleanSerial();

void reinitTime(){
	zeroTime=millis();
}

long now(){
	return (millis() - zeroTime)-offset;
}

void clearMemory(){
	for(int i=0;i<maxNotesInMem;i++){
		// notes[i][0]=-1;
	}
	for(int i=0;i<maxLookahead;i++){
		// notes[i][0]=-1;
		// notes[i][1]=-1;
		// notes[i][2]=-1;
		// notes[i][3]=-1;
	}
	
}

void readSerial(){
	static char commandBuffer[maxCommandLength];
	static int commandIndex=0;
    while (Serial.available() > 0) {
        char incomingByte = Serial.read();

        if (incomingByte == '\n') {
            commandBuffer[commandIndex] = '\0'; // Null-terminate the command
            bool validCommandPointer = false;

            for (int i = 0; i < sizeof(validCommandPointers); i++) {
                if (commandBuffer[0] == validCommandPointers[i][0]) {
                    validCommandPointer = true;
                    break;
                }
            }

            if (validCommandPointer) {
                processCommand(commandBuffer[0], commandBuffer + 1);
				cleanSerial();
            } else {
                Serial.print("e:");
                Serial.println(commandBuffer);
            }

            commandIndex = 0; // Reset command buffer index
			memset(commandBuffer, 0, sizeof(commandBuffer));
        } else {
            if (commandIndex < maxCommandLength - 1) {
                commandBuffer[commandIndex++] = incomingByte;
            }
        }
    }
}

void processCommand(char pointer, const char* commandValue) {


	if(pointer=='s'){
		zeroTime=millis()+(startOffset-serialTimeOffset);
		isPlaying=true;
		return;
	}else if(pointer=='o'){
		//set offset
		offset=atof(commandValue)*1000;
		return;
	}else if(pointer=='e'){
		isPlaying=false;
		return;

	}else if(pointer=='c'){
		clearMemory();
		return;
	}else if(pointer=='n'){
		notes[totalNotesCnt][0]=atof(commandValue);
		totalNotesCnt=(totalNotesCnt+1)%maxNotesInMem;
		return;
	}else if(pointer=='l'){
		//toggle led
		toggleLed(atoi(commandValue));
		return;
	}


    Serial.println("OK");


}

void cleanSerial(){
	while (Serial.available() > 0) {
		Serial.read();
	}
}
