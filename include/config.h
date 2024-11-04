
bool debug=false;


#define dataPin 8 // Data Pin
#define clockPin 11 // Clock Pin
#define latchPin 12 // Latch Pin

#define maxNotesInMem 20
#define maxLookahead 24
#define startOffset 1500
#define lookAhead 1500
#define LED_8_ON_DURATION 1000  // 0.3 seconds in milliseconds


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


float offset=0;


//[when,prefered led id -1 if none]
int notes[maxNotesInMem][2]={
	{1000,-1},
	{2000,-1},
	{3000,-1},
	{4000,-1},
	{5000,-1},
	{6000,-1},
	{7000,-1},
	{8000,-1},
	{9000,-1},
	};
byte shiftRegisterState[6] = {0}; // Each byte controls 8 outputs
uint8_t totalNotesCnt=0;

//note id, last led id, and changeover time
int notesInMem[maxLookahead][4];