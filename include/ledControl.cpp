#include <Arduino.h>

int lastGroup=-1;

//take into consideration options like:
//allowDoubles - same id twice in a row
//disabled ids
//flip sides - if true allow only opposite sides
int getRandomGroupId(){
	int availableGroups[6];
	int count = 0;
	for(int i=0;i<6;i++){
		if (disabledIds[i] != 0) continue;  // Skip disabled IDs
		if (!allowDoubles and i == lastGroup) continue;  // Skip if doubles aren’t allowed
		if (flipSides and lastGroup != -1 and abs(i - lastGroup) % 3 != 0) continue;  // Skip if flipSides restricts

		availableGroups[count++] = i; 
	}
	int randomIndex = random(0, count);
	lastGroup = availableGroups[randomIndex];
	return lastGroup;
}

void updateShiftRegisters() {
	// Latch low to start sending data
	digitalWrite(latchPin, LOW);
	
	// Send data to all shift registers (from last to first)
	for (int i = 5; i >= 0; i--) {
	  shiftOut(dataPin, clockPin, MSBFIRST, shiftRegisterState[i]);
	}
	// Latch high to output the data
	digitalWrite(latchPin, HIGH);
}


void toggleLed(int ledIndex) {
	// Find which shift register (0-5) and which bit (0-7) to toggle
	if(ledIndex<0 or ledIndex>47) return;
	int registerIndex = ledIndex / 8;   // Determines which shift register
	int bitIndex = ledIndex % 8;        // Determines which bit within that register

	// Toggle the bit using XOR
	shiftRegisterState[registerIndex] ^= (1 << bitIndex);
	// Serial.println("Led "+String(ledIndex));
	
	// Update the shift registers with the new states
	// updateShiftRegisters();
}

void toggleWholeGroup(int groupId){
	for (int j = 0; j < 8; j++) {
		toggleLed(ledIds[groupId][j]); // Use ledIds[i][j] to get the LED ID
	}
	// if(debug)Serial.println("Toggling group "+String(groupId));
}
