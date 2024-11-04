#include <Arduino.h>

// Helper function to check if a note index is already in look-ahead array
bool isInLookAhead(int index) {
    for (int j = 0; j < maxLookahead; j++) {
		if (lookAheadNotes[j].noteIndex == -1)continue;
        if (lookAheadNotes[j].noteIndex == index) {
            return true;
        }
    }
    return false;
}

int lookAheadIndex = 0; 
int lastIndexInRange = 0;     


void findNotesInRange(Note *outputArray) {
    long currentTime = millis()-zeroTime;
    long rangeEnd = currentTime + lookAhead;

    // Start from the last index processed within the lookahead range
    for (int i = lastIndexInRange; i < maxNotesInMem; i++) {
        if (notes[i][0] > rangeEnd or notes[i][0]==-1) {
			
            break; // Stop if the note timestamp exceeds the lookahead range
        }

        // Only add notes within the range that aren’t duplicates
        if (notes[i][0] > currentTime && !isInLookAhead(i)) {
            Note note;
            note.noteIndex = i;
            note.groupId = (notes[i][1] == -1) ? getRandomGroupId() : notes[i][1];
            // Place the new note into the output array in a circular manner
            outputArray[lookAheadIndex] = note;
            lookAheadIndex = (lookAheadIndex + 1) % maxLookahead; // Wrap if it reaches max size
        }

        lastIndexInRange = i%maxNotesInMem; // Update the last processed index
    }
}


void processNoteTiming(Note *lookAheadNotes) {
    static int previousLedIndex[maxLookahead] = { -1 };
    static long lastToggleTime[maxLookahead] = { 0 };
    static long led8OnStartTime[6] = { 0 };

    long currentTime = millis() - zeroTime;

    for (int i = 0; i < maxLookahead; i++) {
        int groupId = lookAheadNotes[i].groupId;
        // Skip if groupId is invalid
        if (groupId == -1) {
            return;
        }

        long timeToNote = notes[lookAheadNotes[i].noteIndex][0] - currentTime;

        // Calculate toggle interval for proximity LEDs
		// long toggleInterval = ;  // Slightly reduce interval to allow room for LED 8
        int ledIndexToLight = -1;

        // Determine which LED to light based on timeToNote
		if (timeToNote > 0 && timeToNote <= lookAhead) {
			ledIndexToLight = map(timeToNote, lookAhead, 0, 1, 8) - 1; // For proximity LEDs
		} else if (timeToNote <= 0) {
			// Check if LED 8 was not already activated
			if (previousLedIndex[i] != 7) { // Only turn on LED 8 if it was not already on
				ledIndexToLight = 7; // LED 8 (main LED)
				// Serial.println("Turning on LED 8 on group " + String(groupId));
				led8OnStartTime[i] = currentTime;  // Start LED 8's "on" timer
			}
		}

        // Toggle LEDs only if ledIndexToLight is valid and different from previous
        if (ledIndexToLight != -1 && ledIndexToLight != previousLedIndex[i] &&
            (currentTime - lastToggleTime[i] >= ((timeToNote / 8) - 300))) {

            if (previousLedIndex[i] != -1) {
                // int prevLedId = ledIds[groupId][previousLedIndex[i]];
                toggleLed(ledIds[groupId][previousLedIndex[i]]); // Turn off the previous LED
            }

            // int currentLedId = ledIds[groupId][ledIndexToLight];
            toggleLed(ledIds[groupId][ledIndexToLight]);  // Turn on the current LED

            previousLedIndex[i] = ledIndexToLight;
            lastToggleTime[i] = currentTime;
        }

        // Keep LED 8 lit for LED_8_ON_DURATION without retoggling
		if (previousLedIndex[i] == 7) {  // If the last lit LED was LED 8
			if (currentTime - led8OnStartTime[i] >= LED_8_ON_DURATION) {
				// Serial.println("Turning off LED 8 on group " + String(groupId));
				toggleLed(ledIds[groupId][7]); // Turn off LED 8
				previousLedIndex[i] = -1; // Reset the index as LED 8 is turned off
			} else {
				continue; // Skip further actions if LED 8 is still within its on-duration
			}
		}
    }
}
