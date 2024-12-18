#define PIN_DATA 8 // Data Pin
#define PIN_CLOCK 11 // Clock Pin
#define PIN_LATCH 12 // Latch Pin
#define OFFSET_SERIAL 0 

#define MAX_NOTES 50
#define MAX_MEM_NOTES 24

#define DURATION_LED_MAIN 500  // 0.3 seconds in milliseconds
#define PAD_THRESHOLD 200
const int PAD_TO_PIN[6]={
	A2,A1,A3,A0,A4,A5
};


int TIME_SCAN_AHEAD=1000;
long POINTS=0;
long TIME_START=0;
long TIME_NOTES_AGGREGATED=0;

int DURATION_LED_SUB=TIME_SCAN_AHEAD/8;

const int ledIds[6][8] = {
    {33,34,11,35,37,10,9,36},   // LED IDs for group 0
    {31,32,7,30,8,0,13,12}, // Group 1
    {24,39,38,5,4,1,2,3}, // Group 2
    {25,46,21,6,26,28,27,29}, // Group 3
    {47,40,42,41,17,20,18,19}, // Group 4
    {43,44,16,45,14,15,22,23}  // Group 5
};

bool isPlaying=false;

bool allowDoubles=false;
bool flipSides=true;
int disabledIds[6]={0};

unsigned long OFFSET_NOTE=0;

uint32_t notes[MAX_NOTES];
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