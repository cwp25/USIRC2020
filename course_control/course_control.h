//Version 1.0
#define RS0 5
#define BS0 3
#define GS0 6
#define RS1 10
#define BS1 9
#define GS1 11
#define B1LED A4
#define B1BUTT A5
#define R1LED 2
#define R1BUTT 4
#define STRLED 7
#define STRBUTT 8
#define R0LED A0
#define R0BUTT A1
#define B0LED A2
#define B0BUTT A3
#define ROUNDTIME 1L*30L*1000
#define RED 0
#define BLUE 1
unsigned long debounceDelay = 50; //buttons must be pressed for 50 ms

bool triggeredZero = false;
bool triggeredOne = false;
bool successZero = false;
bool successOne = false;
unsigned long Oneposttimer;
unsigned long Zeroposttimer;
bool zerostayoff = false;
bool onestayoff = false;

int R, G, B;
unsigned long randNumber;
uint32_t startTime = 0;
int side0color, side1color;
int reddir = 0;
int greendir = 0;
int bluedir = 0;

int r0state, b0state, r1state, b1state;
int r0last = HIGH;
int b0last = HIGH;
int r1last = HIGH;
int b1last = HIGH;

unsigned long r0lastDebounceTime = 0;  // the last time r0 was toggled
unsigned long b0lastDebounceTime = 0;  // the last time b0 was toggled
unsigned long r1lastDebounceTime = 0;  // the last time r1 was toggled
unsigned long b1lastDebounceTime = 0;  // the last time b1 was toggled
bool R0Pressed = false;
bool R1Pressed = false;
bool B0Pressed = false;
bool B1Pressed = false;
