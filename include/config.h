#define dataPin 8 // Data Pin
#define clockPin 11 // Clock Pin
#define latchPin 12 // Latch Pin
#define serialTimeOffset 0 

#define maxNotesInMem 100
#define maxLookahead 24
#define startOffset 1500
#define lookAhead 1500
#define LED_8_ON_DURATION 1000  // 0.3 seconds in milliseconds
#define LED_TIMESTEP lookAhead/7
const char* validCommandPointers[] = {
	"s",//start playing
	"c",//clear memory
	"e",//end playing
	"o",//offset
	"n",//notes
	"l",//leds

};


const int ledIds[6][8] = {
    {22,23,16,42,43,41,17,44},   // LED IDs for group 0
    {40,14,19,20,15,47,18,45}, // Group 1
    {46,21,4,3,6,29,38,5}, // Group 2
    {27,2,26,25,28,1,24,39}, // Group 3
    {8,37,10,36,7,32,0,31}, // Group 4
    {34,33,13,9,12,30,35,11}  // Group 5
};

unsigned long zeroTime;
bool isPlaying=false;
bool isInCountdown=true;

bool allowDoubles=true;
bool flipSides=false;
int disabledIds[6]={0};


unsigned long offset=0;


int notes[maxNotesInMem][1]={
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	{1000},
	};
byte shiftRegisterState[6] = {0}; // Each byte controls 8 outputs
uint8_t totalNotesCnt=0;

//note id, last led id, and changeover time
struct Note {
    long timestamp;
	int8_t ledId;
	int8_t groupId;
    long changeOverTime;
} __attribute__((packed)); // Ensure the struct is packed efficiently

// Array of notes in memory
Note notesInMem[maxLookahead];