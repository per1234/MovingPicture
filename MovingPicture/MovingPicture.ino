//#include <MemoryFree.h>  //for debugging this library checks available sram - uncomment the lastCheckTime line, the function call in loop(), and the function in the function tab also
//#define __DEBUG_SOFTPWM__ 1  //for debugging - define this before including SoftPWM.h for printInterruptLoad() to work. - uncomment the lastCheckTime line, the function call in loop(), and the function in the function tab also
//long lastCheckTime;  //for debugging the last time the interrupt load or sram was printed
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
const byte ledNum=5;  //still needed to size the pos1 and pos2 arrays
//these are all setup in the profile() function now and this just needs to initialize them, unfortunately can't use const anymore I don't think
int fadeDelayMaxSet;  //the longest time that it will color shift.
int fadeDelayMinSet;  //shortest shift time
int fadeDelayDiffMin;  //the minimum difference in the range of fade speeds
int valueTotalMinMinSet; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
int valueTotalMinMaxSet; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//int valueDiffMinMinSet;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//int valueDiffMinMaxSet;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
byte strobeTime;  //the length of time the strobe is on
int strobeDelayMin;  //the minimum time between strobes
byte strobeLikelyhood;  //the likelyhood of a strobe happening(1 in n, higher number=less likely to happen)
byte scriptID;
//initialize variables, none of these should need setup
byte pos1[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos1size; //the current number of items in the pos1 array(zero indexed) - get rid of the zero index thing too confusing
byte pos2[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos2size;  //the current number of items in the pos2 array(zero indexed) - get rid of the zero index thing too confusing
//byte valueRb;  //the new color brightness values, this could be contained within the new color function
//byte valueGb;
//byte valueBb;
int fadeDelay;  //the length of time that the fade will occur over, should be moved to the various functions that need it(crossfade, shift, etc.)
long program[6][4];  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
byte programStep=1;  //used to step through various different script actions, must start at 0 to run the setup function
int fadeDelayMax;  //this is randomly set in the initialize function
int fadeDelayMin;  //this is randomly set in the initialize function
int valueTotalMin;  //this is randomly set in the initialize function
long lastStrobeTime;  //the last time the strobe was turned on - can this be put in the function or will it be lost with every function call?
byte strobeFlag;  //is the strobe on or off

void setup(){
//  Serial.begin(9600);  //for debugging
  profile(1);
  SoftPWM.begin(120);  //start the SoftPWM and set the PWM frequency
  for(byte softPWMchannel=0;softPWMchannel<=SoftPWM.size();softPWMchannel++){  //cycle through all the softPWM channels
      SoftPWM.set(softPWMchannel, 1);  //turn on all pins at brightness 1 - there is a big jump from 0 to 1 so I have decided to always have every led slightly on
  }
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot
  //I'm setting these variables up here because the randomSeed(analogRead(0)); doesn't work at the top of the sketch where I'm declaring variables for some reason and I need to do the randomSeed(analogRead(0)); before picking random variables
  fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);  //the maximum bound of the rendomly chosen length of time that the fade will occur over
  fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);  //the minimum bound of the rendomly chosen length of time that the fade will occur over
  valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet)*(SoftPWM.brightnessLevels()-1)*3/100;  //rendomly picks the valueTotalMin from the set range so that there will be a different minimum brightness each power on - converts from a percent to a this should be set up to work with different numbers of brightness levels
  //new position 2
  pos2size=random(0,SoftPWM.size()/3-2);  //chose how many RGBs will be in the new postition
  for(byte n=0;n<=pos2size;n++){  //step through to fill the new position
    pos2[n]=random(0,SoftPWM.size()/3);  //chose the RGB to add to the new postition
    for(byte m=0;m<=pos1size;m++){  //make sure the led isn't already in position 1
      while(pos2[n]==pos1[m]){  //make sure the led isn't already in position 1
        pos2[n]=random(0,SoftPWM.size()/3);  //it was already in position 1, try again
        m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
        for(byte o=0;o<=n-1;o++){  //make sure the led isn't already in pos2
          while(pos2[o]==pos2[n]){  //make sure the led isn't already in pos2
            pos2[n]=random(0,SoftPWM.size()/3);  //it was already in position 2, try again
            m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
            o=0;  //restart the for loop and recheck to see if the new led number is already in position 2
          }
        }
      }
    }
  }
}

void loop(){
script(scriptID);
fader();
//interruptLoadCheck()
//sramCheck();
}
