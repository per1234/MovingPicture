//#include <MemoryFree.h>  //for debugging this library checks available sram - uncomment the lastCheckTime line, the function call in loop(), and the function in the function tab also
//#define __DEBUG_SOFTPWM__ 1  //for debugging - define this before including SoftPWM.h for printInterruptLoad() to work. - uncomment the lastCheckTime line, the function call in loop(), and the function in the function tab also
//long lastCheckTime;  //for debugging the last time the interrupt load or sram was printed
//long debugBlinkNextTime;  //the last time the debug blink happened
//byte debugBlinkState;  //is the debug led on or off
#include <arduino_softpwm_master.h>  //the softPWM library

//map the softPWM channels to the port and bit of the pins the leds are connected to, these must be in rgb consecutive order for each RGB LED and if the number of channels are changed the SOFTPWM_DEFINE_OBJECT() and ledNum parameters must be updated also
SOFTPWM_DEFINE_CHANNEL_INVERT( 0, DDRD, PORTD, PORTD0 );  //D0
SOFTPWM_DEFINE_CHANNEL_INVERT( 1, DDRD, PORTD, PORTD1 );  //D1
SOFTPWM_DEFINE_CHANNEL_INVERT( 2, DDRC, PORTC, PORTC4 );  //A4
SOFTPWM_DEFINE_CHANNEL_INVERT( 3, DDRD, PORTD, PORTD2 );  //D2
SOFTPWM_DEFINE_CHANNEL_INVERT( 4, DDRD, PORTD, PORTD3 );  //D3
SOFTPWM_DEFINE_CHANNEL_INVERT( 5, DDRD, PORTD, PORTD4 );  //D4
SOFTPWM_DEFINE_CHANNEL_INVERT( 6, DDRD, PORTD, PORTD5 );  //D5
SOFTPWM_DEFINE_CHANNEL_INVERT( 7, DDRD, PORTD, PORTD6 );  //D6
SOFTPWM_DEFINE_CHANNEL_INVERT( 8, DDRB, PORTB, PORTB1 );  //D9
SOFTPWM_DEFINE_CHANNEL_INVERT( 9, DDRC, PORTC, PORTC0 );  //A0
SOFTPWM_DEFINE_CHANNEL_INVERT( 10, DDRB, PORTB, PORTB3 );  //D11
SOFTPWM_DEFINE_CHANNEL_INVERT( 11, DDRB, PORTB, PORTB2 );  //D10
SOFTPWM_DEFINE_CHANNEL_INVERT( 12, DDRC, PORTC, PORTC3 );  //A3
SOFTPWM_DEFINE_CHANNEL_INVERT( 13, DDRC, PORTC, PORTC2 );  //A2
SOFTPWM_DEFINE_CHANNEL_INVERT( 14, DDRC, PORTC, PORTC1 );  //A1
SOFTPWM_DEFINE_OBJECT(15);  //15 softPWM channels
const byte ledNum=5;  //still needed to size the pos1 and pos2 arrays but use SoftPWM.size() everywhere else

//initialize global variables, none of these should need setup
byte pos1[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos1size; //the current number of items in the pos1 array(zero indexed) - get rid of the zero index thing too confusing
byte pos2[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos2size;  //the current number of items in the pos2 array(zero indexed) - get rid of the zero index thing too confusing
int fadeDelay;  //the length of time that the fade will occur over, should be moved to the various functions that need it(crossfade, shift, etc.)
long program[6][4];  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time] - can't be unsigned because the fader equation needs negative numbers
byte lastTargetBrightness[6];  //used to store the previous target brightness values from the program[] array in the strobe function
int fadeDelayMax;  //this is randomly set in the initialize function
int fadeDelayMin;  //this is randomly set in the initialize function
int valueTotalMin;  //this is randomly set in the initialize function
byte programControl;  //flag to disable the main script while an addon script is in control of the program
byte standardStep=1;  //the step the standard() scriptlet is currently in
byte strobeStep;  //is the strobe on or off flag
long strobeNextTime;  //the time until the next strobe;
byte fadeUpBackStep;  //the step the fadeUpBack() function is currently in
long fadeUpBackNextTime;  //the next time to do a fadeUpBack
byte allShiftRandomStep;  //the current step of the allShiftRandom add-on scriptlet
long allShiftRandomNextTime;  //the next time the allShiftRandom add-on scriptlet will run
byte allShiftRandomIterations;  //the number of times the current allShiftRandom will color shift
byte setterFlag;  //try to get rid of this
byte fillAllPosCount;
