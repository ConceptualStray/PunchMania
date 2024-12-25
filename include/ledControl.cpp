#include <Arduino.h>

int lastGroup=-1;
int leftSide[3] = {5,0,1};
int rightSide[3] = {2,3,4};
int biases[6] = {10, 10, 2, 10, 10, 2}; // Bias weights for each ID

//take into consideration options like:
//allowDoubles - same id twice in a row
//disabled ids
//flip sides - if true allow only opposite sides

int getRandomGroupId(){
    int availableGroups[6];
    int cumulativeWeights[6];
    int count = 0;
    int weightSum = 0;
    bool lastWasLeft=false;
    bool lastWasRight=false;

    for (size_t i = 0; i < 6; i++){
        if(leftSide[i]==lastGroup){
            lastWasLeft=true;
            break;
        }
        if(rightSide[i]==lastGroup){
            lastWasRight=true;
            break;
        }
    }

    for(int i=0;i<6;i++){
        if (disabledIds[i] != 0) continue;  // Skip disabled IDs
        if (!allowDoubles and i == lastGroup) continue;  // Skip if doubles aren’t allowed
        // Skip if flipSides restricts
        if(flipSides and lastGroup != -1){
            if (lastWasLeft && i < 3) continue;  // Skip left side if last was left
            if (lastWasRight && i >= 3) continue;  // Skip right side if last was right
        }

        availableGroups[count] = i;
        weightSum += biases[i];
        cumulativeWeights[count] = weightSum;
        count++;
    }

    if(count==0) return 0;

    // Generate random number within the range [0, total weight)
    int randVal = random(0, weightSum);

    // Find the corresponding index using cumulative weights
    for (int i = 0; i < count; i++){
        if (randVal < cumulativeWeights[i]){
            lastGroup = availableGroups[i];
            return lastGroup;
        }
    }

    return -1; // Should never reach here
}



void updateShiftRegisters() {
	// Latch low to start sending data
	digitalWrite(PIN_LATCH, LOW);

	// Send data to all shift registers (from last to first)
	for (int i = 5; i >= 0; i--) {
	  shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, shiftRegisterState[i]);
	}

	// Latch high to output the data
	digitalWrite(PIN_LATCH, HIGH);
}

int getLedId(int groupId, int ledIndex) {
  return ledIds[groupId][ledIndex];
}

void toggleLedOn(int ledIndex){
	if(ledIndex<0 or ledIndex>47) return;
	int registerIndex = ledIndex / 8;   // Determines which shift register
	int bitIndex = ledIndex % 8;   
	shiftRegisterState[registerIndex] |= (1 << bitIndex);
}

void toggleLedOff(int ledIndex){
	if(ledIndex<0 or ledIndex>47) return;
	int registerIndex = ledIndex / 8;   // Determines which shift register
	int bitIndex = ledIndex % 8;   
	shiftRegisterState[registerIndex] &= ~(1 << bitIndex);
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

void toggleWholeGroupOff(int groupId){
	for (int j = 0; j < 8; j++) {
		toggleLedOff(ledIds[groupId][j]); // Use ledIds[i][j] to get the LED ID
	}
	// if(debug)Serial.println("Toggling group "+String(groupId));
}

void toggleWholeGroup(int groupId){
	for (int j = 0; j < 8; j++) {
		toggleLed(ledIds[groupId][j]); // Use ledIds[i][j] to get the LED ID
	}
	// if(debug)Serial.println("Toggling group "+String(groupId));
}

void lastNoteDance() {
	//we toggle each sequential led on each group
	for (size_t i = 0; i < 8; i++) {
		for(size_t j=0;j<6;j++){
			toggleLed(ledIds[j][i]);
		}
		updateShiftRegisters();
		delay(100);
	}
	//do a while group blink blink 3 times
	for (size_t i = 0; i < 6; i++) {
		for(size_t j=0;j<6;j++){
			toggleWholeGroup(j);
		}
		updateShiftRegisters();
		delay(200);
		
	}
	//the same but other way around
	for (size_t i = 0; i < 8; i++) {
		for(size_t j=0;j<6;j++){
			toggleLedOff(ledIds[j][i]);
		}
		delay(100);
		updateShiftRegisters();
		
	}
}

void toggleLedLeads(){
	for (size_t i = 0; i < 7; i++){
		toggleLed(ledIds[i][0]);
	}
}