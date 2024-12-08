#include <Arduino.h>


#define MAX_COMMAND_LENGHT 10
void processCommand(char pointer, const char* command);
void cleanSerial();

long now(){
	return millis()-TIME_START-OFFSET_NOTE;
}

// void testpopulate(){
// 	for(int i=0;i<70;i++){
// 		notes[i]=700;
// 	}
// }

void clearMemory(){
	for(int i=0;i<MAX_MEM_NOTES;i++){
		Note note;
		note.timestamp=-1;
		notesInMem[i]=note;
	}
	for(int i=0;i<MAX_NOTES;i++){
		notes[i]=-1;
	}
	
}

void debugFunc() {
    Serial.print("Active LEDs: ");
    bool foundActiveLED = false;

    for (int byteIndex = 0; byteIndex < 6; byteIndex++) {
        for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
            // Check if the specific bit is set
            if (shiftRegisterState[byteIndex] & (1 << bitIndex)) {
                int ledID = byteIndex * 8 + bitIndex;
                Serial.print(ledID);
                Serial.print(" ");
                foundActiveLED = true;
            }
        }
    }

    if (!foundActiveLED) {
        Serial.println("None");
    } else {
        Serial.println();
    }

	for (size_t i = 0; i < 6; i++)
	{
		Serial.println("pad id is "+String(i)+" active: "+activePads[i]);
	}
	
}



void readSerial(){
	static char commandBuffer[MAX_COMMAND_LENGHT];
	static int commandIndex=0;
    while (Serial.available() > 0) {
        char incomingByte = Serial.read();
        if (incomingByte == '\n') {
            commandBuffer[commandIndex] = '\0'; // Null-terminate the command
            processCommand(commandBuffer[0], commandBuffer + 1);
			cleanSerial();
            commandIndex = 0; // Reset command buffer index
			memset(commandBuffer, 0, sizeof(commandBuffer));
        } else {
            if (commandIndex < MAX_COMMAND_LENGHT - 1) {
                commandBuffer[commandIndex++] = incomingByte;
            }
        }
    }
}

void processCommand(char pointer, const char* commandValue) {


	if(pointer=='s'){
		isPlaying=true;
		// Serial.println(TIME_NOTES_AGGREGATED);
		TIME_START=millis()-OFFSET_SERIAL;
		TIME_NOTES_AGGREGATED=now();
		Serial.println(1);
		return;
	}else if(pointer=='o'){
		//set offset
		OFFSET_NOTE=atof(commandValue)*1000;
		Serial.println(1);
		return;
	}else if(pointer=='p'){
		Serial.println(POINTS);
		return;

	}else if(pointer=='r'){
		//reset
		isPlaying=false;
		for(int i=0;i<6;i++){
			toggleWholeGroupOff(i);
		}
		for(int i=0;i<MAX_MEM_NOTES;i++){
			Note note;
			note.timestamp=-1;
			notesInMem[i]=note;
		}
		for(int i=0;i<MAX_NOTES;i++){
			notes[i]=-1;
		}
		Serial.println(1);
		return;
	}else if(pointer=='n'){
		notes[totalNotesCnt]=atoi(commandValue);
		totalNotesCnt=(totalNotesCnt+1)%MAX_NOTES;
		Serial.println(1);
		return;
	}else if(pointer=='l'){
		//toggle led
		toggleLed(atoi(commandValue));
		Serial.println(1);
		return;
	}else if(pointer=='d'){
		//toggle led
		debugFunc();
		Serial.println(1);
		return;
	}
}

void cleanSerial(){
	while (Serial.available() > 0) {
		Serial.read();
	}
}
