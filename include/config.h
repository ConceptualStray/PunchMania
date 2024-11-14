#define PIN_DATA 8 // Data Pin
#define PIN_CLOCK 11 // Clock Pin
#define PIN_LATCH 12 // Latch Pin
#define OFFSET_SERIAL 0 

#define MAX_NOTES 200
#define MAX_MEM_NOTES 24

#define START_SEQUENCE_DURATION 1500

#define DURATION_LED_MAIN 500  // 0.3 seconds in milliseconds



int TIME_SCAN_AHEAD=1000;
long TIME_START=0;
long TIME_NOTES_AGGREGATED=0;

int DURATION_LED_SUB=TIME_SCAN_AHEAD/8;


const char* validCommandPointers[] = {
	"s",//start playing
	"c",//clear memory
	"e",//end playing
	"o",//offset
	"n",//notes
	"l",//toggle leds
	"d",//debug active leds

};


const int ledIds[6][8] = {
    {22,23,16,42,43,41,17,44},   // LED IDs for group 0
    {40,14,19,20,15,47,18,45}, // Group 1
    {46,21,4,3,6,29,38,5}, // Group 2
    {27,2,26,25,28,1,24,39}, // Group 3
    {8,37,10,36,7,32,0,31}, // Group 4
    {34,33,13,9,12,30,35,11}  // Group 5
};

bool isPlaying=false;
bool isInCountdown=true;

bool allowDoubles=true;
bool flipSides=false;
int disabledIds[6]={0};


unsigned long OFFSET_NOTE=0;


int notes[MAX_NOTES];
byte shiftRegisterState[6] = {0}; // Each byte controls 8 outputs
uint8_t totalNotesCnt=0;

//note id, last led id, and changeover time
struct Note {
    long timestamp=-1;
	int8_t ledId=0;
	int8_t groupId=0;
    long changeOverTime=-1;
} __attribute__((packed)); // Ensure the struct is packed efficiently

// Array of notes in memory
Note notesInMem[MAX_MEM_NOTES];

long activePads[6];