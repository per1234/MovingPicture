/*
Matrix Alternative Code
it will follow the same basic theme of crossfade to new position, color shift at that position, then repeat but each position will consist of a random number of consecutive leds(up to the number that are not lit in the current position) on at one time.
*/

#include <SoftPWM.h> //the SoftPWM library that contains all commands starting with SoftPWM
const byte ledNum=5;  //the number of leds connected
int pin[][3]={  //the pins each led is connected to. These must be in consecutive order. {r,g,b}.
  {0,1,A4},
  {2,3,4},
  {6,8,9},
  {A0,12,11},
  {A3,A2,A1}
};
int fadeDelayMaxSet = 3000;  //the longest time that it will color shift.
const int fadeDelayMinSet = 700;  //shortest shift time
const int fadeDelayDiffMin = 100;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 120; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors. I will eventually set this up to change randomly with each power on
const int valueTotalMinMaxSet = 200; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors. I will eventually set this up to change randomly with each power on
//int valueDiffMin=15;  //the minimum change in value of each color. not yet implemented
const int valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet);
int pos1[ledNum]={0};  //The postition 1 array - this contains the pin[] row numbers of the LEDs that make up the current pos1.
int pos1size=0; //the current number of items in the pos1 array(zero indexed)
int pos2[ledNum]; 
int pos2size;
byte valueRa=0;
byte valueGa=0;
byte valueBa=0;
byte valueRb;
byte valueGb;
byte valueBb;
int fadeDelay;
int fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);
int fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);

void perFade(int perFadePos[],byte perFadePosQty, float perFadeDelay, float perFadeValueRa, float perFadeValueRb, float perFadeValueGa, float perFadeValueGb, float perFadeValueBa, float perFadeValueBb){  //this function takes an array of position numbers, the length of the position number array, the length of time that the fade will occur over, and the initial(a) and final value(b) of each color
  int perFadeValueDiffArray[]={abs(perFadeValueRa-perFadeValueRb),abs(perFadeValueGa-perFadeValueGb),abs(perFadeValueBa-perFadeValueBb)};
  int perFadeValueBArray[]={perFadeValueRb,perFadeValueGb,perFadeValueBb};
  for(int row=0;row<=perFadePosQty;row++){  //cycle through the position rows to be perFaded
    for(int column=0;column<=2;column++){  //cycle through the color columns of the 2 dimensional pin array
      int perVar=255*perFadeDelay/perFadeValueDiffArray[column];
      if(perVar>4000){
        perVar=4000;
      }
      SoftPWMSetFadeTime(pin[perFadePos[row]][column],perVar, perVar);  //must make sure that the fadeValueDiff will never be zero. there needs to be some way to make sure that the FadeTime never exceeds 4000
//   Serial.println("fadetime=");
//   Serial.println(255*perFadeDelay/perFadeValueDiffArray[column]);
      SoftPWMSet(pin[perFadePos[row]][column], perFadeValueBArray[column]);
//         Serial.println("value=");
//         Serial.println(perFadeValueBArray[column]);
    }
  }
}
  
void setup(){
//  Serial.begin(9600); //REMOVE for debugging via serial monitor
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot
  SoftPWMBegin();
  for(byte row=0;row<=ledNum-1;row++){  //cycle through the position rows of the 2 dimensional pin array
    for(byte column=0;column<=2;column++){  //cycle through the 3 color columns of the 2 dimensional pin array
      SoftPWMSet(pin[row][column], 0); //create and turn off all pins
      SoftPWMSetPolarity(pin[row][column],SOFTPWM_INVERTED);
    }
  }
}

void loop()
{
  //begin color shift
  fadeDelay = random(fadeDelayMin,fadeDelayMax);
  valueRb=random(0,256);  
  valueGb=random(0,256);
  valueBb=random(0,256);
  while(valueRb+valueGb+valueBb<valueTotalMin){
    valueRb=random(0,256);
    valueGb=random(0,256);
    valueBb=random(0,256);
  }
  perFade(pos1, pos1size, fadeDelay, valueRa, valueRb, valueGa, valueGb, valueBa, valueBb);   //perFadePos[],perFadePosQty,perFadeDelay,perFadeValueRa,perFadeValueRb,perFadeValueGa,perFadeValueGb,perFadeValueBa,perFadeValueBb.
  delay(fadeDelay);
  //begin crossfade
  fadeDelay = random(fadeDelayMin,fadeDelayMax);
  valueRa=valueRb;
  valueGa=valueGb;
  valueBa=valueBb;
  perFade(pos1, pos1size, fadeDelay, valueRa, 0, valueGa, 0, valueBa, 0);  //pos1 fade out
  pos2size=random(0,ledNum-pos1size); 
  for(byte i=0;i<=pos2size;i++){
    pos2[i]=random(0,ledNum);  //new position
    for(byte j=0;j<=pos1size;j++){  //make sure the led isn't already in pos1 or pos2
      while(pos2[i]==pos1[j]){
        pos2[i]=random(0,ledNum);
        for(byte k=0;k<=i-1;k++){ 
          while(pos2[i]==pos2[k]){
            pos2[i]=random(0,ledNum);
          }
        }
      }
    }
  }
  valueRb=random(0,256);  //color shift
  valueGb=random(0,256);
  valueBb=random(0,256);
  while(valueRb+valueGb+valueBb<valueTotalMin){
    valueRb=random(0,256);
    valueGb=random(0,256);
    valueBb=random(0,256);
  }
  perFade(pos2, pos2size, fadeDelay, 0, valueRb, 0, valueGb, 0, valueBb); 
  delay(fadeDelay);
  valueRa=valueRb;
  valueGa=valueGb;
  valueBa=valueBb;
  for(byte i=0;i<=pos2size;i++){
    pos1[i]=pos2[i];  
  }
  pos1size=pos2size;
}

/*
to do:
-new pwm system
-fade system(most likely will not be included in the pwm system
-spectrum limitation - 3(?) different manually set ranges(Ra,Ga,Ba,Rb,Gb,Bb array) that allows me to give a custom palette to the piece and a random range within that range that changes on every powerup. the range can be inclusive as normal or exclusive if the max and min values are reversed.
  -the values will be used as a ratio of r:g:b and don't denote brightness
-consecutive leds only in positions - it must choose a start led and number of leds and then create the position from the available leds consecutive to that start led
  -positions as a range rather than an array of leds. if the max value is less than the min value then that means it crosses the zero point of the circle
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
  -push toggles between standard, fixed color(rotary selects color), no movement all leds synched color shift, randomly triggered events/bonus tracks with rotary selector cycling through the options
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
  -PROGMEM - this stores variables? in flash

*/
