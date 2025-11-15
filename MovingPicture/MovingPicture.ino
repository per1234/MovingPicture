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

//these are the setup variables that can be modified to alter the character of the light
const int fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
const int fadeDelayMinSet = 5000;  //shortest shift time
const int fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//int valueDiffMinMaxSet=15;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented

//initialize variables, non of these should need setup
byte pos1[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos1size; //the current number of items in the pos1 array(zero indexed) - get rid of the zero index thing too confusing
byte pos2[ledNum];  //The postition 1 array - this contains the numbers of the LEDs that make up the current pos1.
byte pos2size;  //the current number of items in the pos2 array(zero indexed) - get rid of the zero index thing too confusing
byte valueRb;  //the new color brightness values, this could be contained within the new color function
byte valueGb;
byte valueBb;
int fadeDelay;  //the length of time that the fade will occur over, should be moved to the various functions that need it(crossfade, shift, etc.)
long program[6][4];  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
byte programStep;  //used to step through various different script actions, must start at 0 to run the setup function
int fadeDelayMax;  //this is randomly set in the initialize function
int fadeDelayMin;  //this is randomly set in the initialize function
int valueTotalMin;  //this is randomly set in the initialize function

void setup(){
//  Serial.begin(9600);  //for debugging
  SoftPWM.begin(120);  //start the SoftPWM and set the PWM frequency
  for(byte softPWMchannel=0;softPWMchannel<=SoftPWM.size();softPWMchannel++){  //cycle through all the softPWM channels
      SoftPWM.set(softPWMchannel, 1);  //turn on all pins at brightness 1 - there is a big jump from 0 to 1 so I have decided to always have every led slightly on
  }
}

void loop(){
  if(programStep==0){  //setup random variables - I put it here instead of at the top of the sketch because I want to get the random numbers after the randomSeed() command
    randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot
    fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);
    fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);
    valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet)*(SoftPWM.brightnessLevels()-1)*3/100;  //rendomly picks the valueTotalMin from the set range so that there will be a different minimum brightness each power on - converts from a percent to a this should be set up to work with different numbers of brightness levels
    pos2size=random(0,SoftPWM.size()/3-2); //new position 2
    for(byte n=0;n<=pos2size;n++){
      pos2[n]=random(0,SoftPWM.size()/3);  
      for(byte m=0;m<=pos1size;m++){  //make sure the led isn't already in position 1
        while(pos2[n]==pos1[m]){
          pos2[n]=random(0,SoftPWM.size()/3);
          m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
          for(byte o=0;o<=n-1;o++){  //make sure the led isn't already in pos2
            while(pos2[o]==pos2[n]){
              pos2[n]=random(0,SoftPWM.size()/3);
              m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
              o=0;  //restart the for loop and recheck to see if the new led number is already in position 2
            }
          }
        }
      }
    }
    programStep=1;
  }

  if(programStep==1){  //position one color shift
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
      valueRb=random(1,SoftPWM.brightnessLevels());  
      valueGb=random(1,SoftPWM.brightnessLevels());
      valueBb=random(1,SoftPWM.brightnessLevels());
      while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
        valueRb=random(1,SoftPWM.brightnessLevels());
        valueGb=random(1,SoftPWM.brightnessLevels());
        valueBb=random(1,SoftPWM.brightnessLevels());
      }
      program[0][0]=valueRb;  //update the end brightness
      program[1][0]=valueGb;    
      program[2][0]=valueBb;   
      program[0][1]=millis()+fadeDelay;  //update the end time
      program[1][1]=millis()+fadeDelay;      
      program[2][1]=millis()+fadeDelay;
      programStep=2;
//      digitalWrite(13, HIGH);
    }
  }
  if(programStep==2){  //crossfade position 1 to position2
    if(millis()>=program[0][1]){  //check if the color shift for position 1 is complete  
      pos2size=random(0,SoftPWM.size()/3-pos1size); //new position 2
      for(byte n=0;n<=pos2size;n++){
        pos2[n]=random(0,SoftPWM.size()/3);  
        for(byte m=0;m<=pos1size;m++){  //make sure the led isn't already in position 1
          while(pos2[n]==pos1[m]){
            pos2[n]=random(0,SoftPWM.size()/3);
            m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
            for(byte o=0;o<=n-1;o++){  //make sure the led isn't already in pos2
              while(pos2[o]==pos2[n]){
                pos2[n]=random(0,SoftPWM.size()/3);
                m=0;  //restart the for loop and recheck to see if the new led number is already in position 1
                o=0;  //restart the for loop and recheck to see if the new led number is already in position 2
              }
            }
          }
        }
      }
      fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new values
      valueRb=random(1,SoftPWM.brightnessLevels());  
      valueGb=random(1,SoftPWM.brightnessLevels());
      valueBb=random(1,SoftPWM.brightnessLevels());
      while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
        valueRb=random(1,SoftPWM.brightnessLevels());
        valueGb=random(1,SoftPWM.brightnessLevels());
        valueBb=random(1,SoftPWM.brightnessLevels());
      }
      program[0][0]=1;  //update the position 1 end brightness(0)
      program[1][0]=1;    
      program[2][0]=1;
      program[3][2]=1;  //update the position 2 current brightness(0)
      program[4][2]=1;    
      program[5][2]=1;
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
      programStep=1;
//      digitalWrite(13, LOW);
    }
  }
  if(programStep==3){  //full fade up
    if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
      pos1size=0;
      pos2size=3;
      for(byte w=1;w<SoftPWM.size()/3;w++){
        pos2[w-1]=w;
      }
      for(byte b=0;b<=5;b++){
        program[b][0] = 255;
        program[b][1] = millis()+10000;  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
      }
     programStep=4;
    } 
  }
  if(programStep==4){  //full fade down
    if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
      for(byte b=0;b<=5;b++){
        program[b][0] = 1;
        program[b][1] = millis()+10000;  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
      }
     programStep=3;
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
              SoftPWM.set(pos1[j]*3+i, program[i][2]);  //change the brightness 
            }
          }
          if(i>=3){  //change brightness of position 2
            for(byte j=0;j<=pos2size;j++){  //cycle through the position rows to be perFaded
             SoftPWM.set(pos2[j]*3+i-3, program[i][2]);  //change the brightness
            }
          }
        }
        if(program[i][0]-program[i][2]<0){  //check if it is a negative change
          program[i][2]=program[i][2]-1; //update the last brightness
          if(i<3){  //change brightness of position 1
            for(byte j=0;j<=pos1size;j++){  //cycle through the position rows to be perFaded
              SoftPWM.set(pos1[j]*3+i, program[i][2]);  //change the brightness
            }
          }
          if(i>=3){  //change brightness of position 2
            for(byte j=0;j<=pos2size;j++){  //cycle through the position rows to be perFaded
              SoftPWM.set(pos2[j]*3+i-3, program[i][2]);  //change the brightness       
            }
          }
        }
        program[i][3]=millis();  //update the last brightness change time
      }
    }
  }
}
