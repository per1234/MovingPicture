//#include <SoftPWM.h> //the SoftPWM library that contains all commands starting with SoftPWM
#include <arduino_softpwm_master.h>
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
SOFTPWM_DEFINE_OBJECT( 15 );
const byte ledNum=5;  //the number of leds connected - I can get rid of this with ARRAY_SIZE or something like that
const byte pin[ledNum][3]={  //the pins each led is connected to. These must be in consecutive order. {r,g,b}. A0-A5=14-19,
  {0,1,2},
  {3,4,5},
  {6,7,8},
  {9,10,11},
  {12,13,14}
};
const int fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
const int fadeDelayMinSet = 5000;  //shortest shift time
const int fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//int valueDiffMin=15;  //the minimum change in value of each color. not yet implemented

const int valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet);  //rendomly picks the valueTotalMin from the set range so that there will be a different minimum brightness each power on
byte pos1[ledNum]={4,5};  //The postition 1 array - this contains the pin[] row numbers of the LEDs that make up the current pos1.
byte pos1size=1; //the current number of items in the pos1 array(zero indexed) I can get rid of this with ARRAY_SIZE or something like that
byte pos2[ledNum]={1,2,3}; 
byte pos2size=2;  //I can get rid of this with ARRAY_SIZE or something like that
byte valueRb=0;
byte valueGb=0;
byte valueBb=0;
int fadeDelay;
const int fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);
const int fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);
long program[6][4]={  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};
byte programStep=0;

void setup(){
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot
//  SoftPWMBegin();
  SoftPWM.begin( 90 );
/*  for(byte row=0;row<=ledNum-1;row++){  //cycle through the position rows of the 2 dimensional pin array
    for(byte column=0;column<=2;column++){  //cycle through the 3 color columns of the 2 dimensional pin array
      SoftPWMSet(pin[row][column], 0); //create and turn off all pins
      SoftPWMSetPolarity(pin[row][column],SOFTPWM_INVERTED);  //configure SoftPWM in inverted mode to work with common anode RGB LEDs
    }
  }*/
    pinMode(13, OUTPUT);     
}

void loop(){
  //the program
  if(programStep==3){  //position one color shift
    if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
      for(int f=0;f<=5;f++){
        program[f][0] = 255;
        long permilles=millis()+4000;
        program[f][1] = permilles;  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
      }
     programStep=4;
       digitalWrite(13, HIGH);
    } 
  }
  if(programStep==4){  //position one color shift
    if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
      for(int f=0;f<=5;f++){
        program[f][0] = 0;
        long permilles=millis()+4000;
        program[f][1] = permilles;  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
      }
       programStep=3;
         digitalWrite(13, LOW);
    }
  }
  if(programStep==0){  //position one color shift
    if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
      for(byte k=0;k<=pos2size;k++){  //START position1 = position2
        pos1[k]=pos2[k];  
      }
      pos1size=pos2size;
      for(byte k=2;k<=3;k++){
        program[0][k]=program[3][k];  //copy current brightness and last brightness change time from position 1 to position 2
        program[1][k]=program[4][k];        
        program[2][k]=program[5][k];
      }  //END position1=position2
      fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new brightness values
      valueRb=random(0,256);  
      valueGb=random(0,256);
      valueBb=random(0,256);
      while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
        valueRb=random(0,256);
        valueGb=random(0,256);
        valueBb=random(0,256);
      }
      program[0][0]=valueRb;  //update the end brightness
      program[1][0]=valueGb;    
      program[2][0]=valueBb;   
      program[0][1]=millis()+fadeDelay;  //update the end time
      program[1][1]=millis()+fadeDelay;      
      program[2][1]=millis()+fadeDelay;
      programStep=1;
    }
  }
  if(programStep==1){  //crossfade position 1 to position2
    if(millis()>=program[0][1]){  //check if the color shift for position 1 is complete  
      pos2size=random(0,ledNum-1-pos1size); //new position 2
      for(byte n=0;n<=pos2size;n++){
        pos2[n]=random(0,ledNum);  
        for(byte m=0;m<=pos1size;m++){  //make sure the led isn't already in position 1
          while(pos2[n]==pos1[m]){
            pos2[n]=random(0,ledNum);
            m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
            for(byte o=0;o<=n-1;o++){  //make sure the led isn't already in pos2
              while(pos2[o]==pos2[n]){
                pos2[n]=random(0,ledNum);
                m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
                o=0;  //restart the for loop and recheck to see if the new led number is already in position 2
              }
            }
          }
        }
      }
      fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new values
      valueRb=random(0,256);  
      valueGb=random(0,256);
      valueBb=random(0,256);
      while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
        valueRb=random(0,256);
        valueGb=random(0,256);
        valueBb=random(0,256);
      }
      program[0][0]=0;  //update the position 1 end brightness(0)
      program[1][0]=0;    
      program[2][0]=0;
      program[3][2]=0;  //update the position 2 current brightness(0)
      program[4][2]=0;    
      program[5][2]=0;
      program[3][0]=valueRb;  //update the posiiton 2 end brightness
      program[4][0]=valueGb;    
      program[5][0]=valueBb;   
      program[0][1]=millis()+fadeDelay;  //update the end time
      program[1][1]=millis()+fadeDelay;      
      program[2][1]=millis()+fadeDelay;
      program[3][1]=millis()+fadeDelay;  //update the end time
      program[4][1]=millis()+fadeDelay;      
      program[5][1]=millis()+fadeDelay;
      program[3][3]=millis();  //update the position 2 last change time
      program[4][3]=millis();      
      program[5][3]=millis();
      programStep=0;
    }
  }

  //the fade system
  for(byte i=0;i<=5;i++){  //cycle through the program rows
    if(program[i][0] != program[i][2]){  //no change needed if the start and end brightness are equal
      if(millis()-program[i][3]>=(program[i][1]-program[i][3])/(abs(program[i][0]-program[i][2])+1)){  //check if it's time to change the brightness
        if(program[i][0]-program[i][2]>0){  //check if it is a positive change
          program[i][2]=program[i][2]+1;  //update the last brightness
          if(i<3){  //change brightness of position 1
            for(byte j=0;j<=pos1size;j++){  //cycle through the position rows to be perFaded
              SoftPWM.set(pin[pos1[j]][i], program[i][2]);  //change the brightness 
              //SoftPWMSet(pin[pos1[j]][i], program[i][2]);  //change the brightness 
            }
          }
          if(i>=3){  //change brightness of position 2
            for(byte j=0;j<=pos2size;j++){  //cycle through the position rows to be perFaded
              SoftPWM.set(pin[pos2[j]][i-3], program[i][2]);  //change the brightness
              //SoftPWMSet(pin[pos2[j]][i-3], program[i][2]);  //change the brightness              
            }
          }
        }
        if(program[i][0]-program[i][2]<0){  //check if it is a negative change
          program[i][2]=program[i][2]-1; //update the last brightness
          if(i<3){  //change brightness of position 1
            for(byte j=0;j<=pos1size;j++){  //cycle through the position rows to be perFaded
              SoftPWM.set(pin[pos1[j]][i], program[i][2]);  //change the brightness
              //SoftPWMSet(pin[pos1[j]][i], program[i][2]);  //change the brightness               
            }
          }
          if(i>=3){  //change brightness of position 2
            for(byte j=0;j<=pos2size;j++){  //cycle through the position rows to be perFaded
              //SoftPWMSet(pin[pos2[j]][i-3], program[i][2]);  //change the brightness
              SoftPWM.set(pin[pos2[j]][i-3], program[i][2]);  //change the brightness              
            }
          }
        }
        program[i][3]=millis();  //update the last brightness change time
      }
    }
  }
}

/*
to do:
-solve flicker!!!
  -try to make it repeatable - all leds on?
  -look into a higher pwm frequency - this could effect millis() and delay() but I could probably adjust the variables to account for that
  -check for bug 
  -test other pwm options
-check sram usage at various points in the sketch
-if I'm sticking with the palatis pwm then I can get rid of the pins array, use the channelsize and brightness levels size functions
-check the millis() overflow(I'm using long instead of unsigned long in the program) and make it overflow friendly
-check the interrupt load and max out the pwm frequency
-increase the brightness levels number and try to slow down the initial part of the fade that's too sudden - this will increase the interrupt load and require a lower pwm frequency
-check the frequency with video and if it has issues then try to change the frequency  
-spectrum limitation - 3(?) different manually set ranges(Ra,Ga,Ba,Rb,Gb,Bb array) that allows me to give a custom palette to the piece and a random range within that range that changes on every powerup. the range can be inclusive as normal or exclusive if the max and min values are reversed.
  -the values will be used as a ratio of r:g:b and don't denote brightness
-consecutive leds only in positions - it must choose a start led and number of leds and then create the position from the available leds consecutive to that start led
  -positions as a range rather than an array of leds. if the max value is less than the min value then that means it crosses the zero point of the circle
-randomly triggered events - I can fill in extra memory with these and put different ones on each piece
  -rainbow chase - the leds crossfade in a circle while color shifting through the spectrum
  -strobe - it could get annoying if not done right - fade in and out but fairly fast to max power
  -R/G/B fast fade in and out of all leds of one color, then returns to the original color
  -white - fade in and out of all leds simultaneously
  -catscradle - the light jumps back and forth across the board to the opposite LED while rotating around the circle
  -interlaced crossfade - every other led around the circle lights and crossfades to the others
  -all shift - all possible leds color shift simultaneously.
  -SOS morse code + 420, 710, per, not fade away
  -heartbeat
  -most on and randomly turn off leds(random5Ledonoff with fade instead of on/off)
-bonus tracks - using the rotary encoder you can access these events by rotating way past the fastest setting(or just have it as an option in the push button menu and then turn to control speed) and then you can select the randomly triggered event to run continuously
-rotary selector input
  -interrupt system?
  -write value to the EPROM using the library so that it will be the same even when turned off - it only is rated for 100000 cycles so it should only write every certain amount of time and only if it has changed - research what the behavior will be after it is worn out and make the program still work
  -push toggles between standard, fixed color(rotary selects color), no movement all leds synched color shift, randomly triggered events/bonus tracks with rotary selector cycling through the options
-optimization - there is flash(code) and sram(variables?) and the sketch must fit in the limits of both
  -ARRAY_SIZE instead of ledNum, pos1size, pos2size - the position1=position2 section needs to resize the array so that if the new one is smaller then the old overflow will be removed
  -let array sizes be automatically determined? - this will make the code a bit more flexible but I don't know if declaring the size is better for memory?
  -use analogWrite() on all hardware pwm enabled pins?
  -remove softPWMsetPercent and fade code from SoftPWM library
  -use const for all constant variables
  -int>byte
  -functions
  -check for unused variables
  -i could split the program array into 2 different arrays, one is target and current brightness(byte), the other is end and last brightness change time(long)
maybe
  -burn sketch with external programmer with no bootloader on the chip for more memory and faster boot
  -PROGMEM - this stores variables? in flash - this seems too complicated
*/
