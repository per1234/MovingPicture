/*
Matrix Alternative Code
it will follow the same basic theme of crossfade to new position, color shift at that position, then repeat but each position will consist of a random number of consecutive leds(up to the number that are not lit in the current position) on at one time.
*/

#include <SoftPWM.h> //the SoftPWM library that contains all commands starting with SoftPWM
int ledNum=2;  //the number of leds connected
int pin[][4]={  //the pins each led is connected to. These must be in consecutive order. {r,g,b,power manage}. Use 1 for the 4th parameter for LEDs that are connected to the power limiting pin group
  {0,1,A4},
  {2,3,4},
  {6,8,9},
  {11,12,A0},
  {A1,A2,A3}
};
int fadeDelayMaxSet = 3000;  //the longest time that it will color shift.
int fadeDelayMinSet = 700;  //shortest shift time
int fadeDelayDiffMin = 100;  //the minimum difference in the range of fade speeds
int valueTotalMinMinSet = 250; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors. I will eventually set this up to change randomly with each power on
int valueTotalMinMaxSet = 250; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors. I will eventually set this up to change randomly with each power on
//int valueDiffMin=15;  //the minimum change in value of each color. not yet implemented
int valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet);
int pos1[]={0};  //the 0 is there just to make it work until I have the multiple led/position system working
int pos2[]={1};  //the 1 is there just to make it work until I have the multiple led/position system working
int valueRa=0;
int valueGa=0;
int valueBa=0;
int valueRb;
int valueGb;
int valueBb;
int firstRun;
int fadeDelay;
int fadeDelayMax;
int fadeDelayMin;

void perFade(int perFadePos[],int perFadePosQty, float perFadeDelay, float perFadeValueRa, float perFadeValueRb, float perFadeValueGa, float perFadeValueGb, float perFadeValueBa, float perFadeValueBb){  //this function takes an array of position numbers, the length of the position number array, the length of time that the fade will occur over, and the initial(a) and final value(b) of each color
  int perFadeValueDiffArray[]={abs(perFadeValueRa-perFadeValueRb),abs(perFadeValueGa-perFadeValueGb),abs(perFadeValueBa-perFadeValueBb)};
  int perFadeValueBArray[]={perFadeValueRb,perFadeValueGb,perFadeValueBb};
  for(int row=0;row<=perFadePosQty-1;row++){  //cycle through the position rows to be perFaded
    for(int column=0;column<=2;column++){  //cycle through the color columns of the 2 dimensional pin array
      int perVar=255*perFadeDelay/perFadeValueDiffArray[column];
      if(perVar>4000){
        perVar=4000;
      }
      SoftPWMSetFadeTime(pin[perFadePos[row]][column],perVar, perVar);  //must make sure that the fadeValueDiff will never be zero. there needs to be some way to make sure that the FadeTime never exceeds 4000
   Serial.println("fadetime=");
   Serial.println(255*perFadeDelay/perFadeValueDiffArray[column]);
      SoftPWMSet(pin[perFadePos[row]][column], perFadeValueBArray[column]);
         Serial.println("value=");
         Serial.println(perFadeValueBArray[column]);
    }
  }
}

void setup(){
  Serial.begin(9600); //REMOVE for debugging via serial monitor
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each 
  SoftPWMBegin();
  for(int row=0;row<=ledNum-1;row++){  //cycle through the position rows of the 2 dimensional pin array
    for(int column=0;column<=2;column++){  //cycle through the color columns of the 2 dimensional pin array
      SoftPWMSet(pin[row][column], 1); //create and turn off all Red pins
    }
  }
  SoftPWMSetPolarity(ALL,SOFTPWM_INVERTED);
}

void loop()
{
 
  if(firstRun==0){
    firstRun=1;
    while(fadeDelayMaxSet - fadeDelayMinSet < fadeDelayDiffMin){  //foolproofing in case the setup variables are less than the minimum range
      fadeDelayMaxSet = fadeDelayMinSet + fadeDelayDiffMin;
    }
    fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin + 5, fadeDelayMaxSet);
    fadeDelayMin=random(fadeDelayMinSet,fadeDelayMax - fadeDelayDiffMin - 5);
    while(fadeDelayMax - fadeDelayMin < fadeDelayDiffMin){  //check that the variation in the range is higher than the minimum
      fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin + 5, fadeDelayMaxSet);
      fadeDelayMin=random(fadeDelayMinSet,fadeDelayMax - fadeDelayDiffMin - 5);
    }
  }
  fadeDelay = random(fadeDelayMin,fadeDelayMax);
  valueRb=random(0,256);  //color shift
  valueGb=random(0,256);
  valueBb=random(0,256);
  while(valueRb+valueGb+valueBb<valueTotalMin){
    valueRb=random(0,256);
    valueGb=random(0,256);
    valueBb=random(0,256);
  }
  //begin color shift
  perFade(pos1, 1, fadeDelay, valueRa, valueRb, valueGa, valueGb, valueBa, valueBb);   //perFadePos[],perFadePosQty,perFadeDelay,perFadeValueRa,perFadeValueRb,perFadeValueGa,perFadeValueGb,perFadeValueBa,perFadeValueBb. the 1 as second parameter is just to make it work before the multi cell position system goes in, eventually that will have to reflect the size of an array that changes every time
  delay(fadeDelay);
  //begin crossfade
  valueRa=valueRb;
  valueGa=valueGb;
  valueBa=valueBb;
  fadeDelay = random(fadeDelayMin,fadeDelayMax);
  perFade(pos1, 1, fadeDelay, valueRa, 0, valueGa, 0, valueBa, 0); 
  pos2[0]=random(0,ledNum);  //new position - this will need to be changed to work with multiple led positions, right now the 0 is just to make it work
  while(pos2[0]==pos1[0]){
    pos2[0]=random(0,ledNum);
  }
  valueRb=random(0,256);  //color shift
  valueGb=random(0,256);
  valueBb=random(0,256);
  while(valueRb+valueGb+valueBb<valueTotalMin){
    valueRb=random(0,256);
    valueGb=random(0,256);
    valueBb=random(0,256);
  }
  perFade(pos2, 1, fadeDelay, 0, valueRb, 0, valueGb, 0, valueBb); 
  delay(fadeDelay);
  valueRa=valueRb;
  valueGa=valueGb;
  valueBa=valueBb;
  for(int i=0;i<=0;i++){
    pos1[i]=pos2[i];  
  }
  pos2[0]=random(0,ledNum);  //new position - this will need to be changed to work with multiple led positions, right now the 0 is just to make it work
}

/*
to do:
-check if the fadeTime number is really milliseconds - maybe an issue with 16vs8mhz?
-spectrum limitation - 3(?) different manually set ranges(Ra,Ga,Ba,Rb,Gb,Bb array) that allows me to give a custom palette to the piece and a random range within that range that changes on every powerup. the range can be inclusive as normal or exclusive if the max and min values are reversed.
  -the values will be used as a ratio of r:g:b and don't denote brightness
-multi-led/position system
-randomly triggered events - I can fill in extra memory with these and put different ones on each piece
  -rainbow chase - the leds crossfade in a circle while color shifting through the spectrum
  -strobe - it could get annoying if not done right - fade in and out but fairly fast to max power
  -R/G/B fast fade in and out of all leds of one color, then returns to the original color
  -white - fast fade in and out of all possible leds within power restrictions - space out the dark LEDs
  -min color variation - percent setup limit and random on power on
  -catscradle - the light jumps back and forth across the board to the opposite LED while rotating around the circle
  -interlaced crossfade - every other led around the circle lights and crossfades to the others
  -all shift - all possible leds color shift. Space out the dark leds that cannot be lit due to current restrictions
  -SOS morse code + 420, 710, per, not fade away
  -heartbeat
  -most on and randomly turn off leds(random5Ledonoff with fade instead of on/off)
-bonus tracks - using the rotary encoder you can access these events by rotating way past the fastest setting(or just have it as an option in the push button menu and then turn to control speed) and then you can select the randomly triggered event to run continuously
-rotary selector input
  -interrupt system?
  -write value to the EPROM using the library so that it will be the same even when turned off - it only is rated for 100000 cycles so it should only write every certain amount of time and only if it has changed - research what the behavior will be after it is worn out and make the program still work
  -push toggles between standard, fixed color(rotary selects color), and no movement all leds synched color sdhift
-optimization - there is flash(code) and sram(variables?) and the sketch must fit in the limits of both
  -delay>millis
  -put perFade in the softPWM library
  -remove softPWMsetPercent code from SoftPWM library
  -remove fade up and down time from softPWM library - just need one fade time
  -use const for all constant variables
  -int>byte
  -functions
  -check for unused variables
  -declare array sizes?
  -burn sketch with external programmer with no bootloader on the chip for more memory and faster boot
  -PROGMEM - this stores in flash

*/
