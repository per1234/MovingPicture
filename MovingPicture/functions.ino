/*
organize according to main script type functions(standard/allShift) that can only be used one at a time, addon functions(strobe, allShiftRandom, fadeUpBack) that can be used in combination, and other(general functions, debug, etc). split into separate tabe
todo:
  -ministrobe
  -rainbow chase
*/
void setup(){
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot - this needs a more random input
  //Serial.begin(9600);  //for debugging
  SoftPWM.begin(120);  //start the SoftPWM and set the PWM frequency
  for(byte softPWMchannel=0;softPWMchannel<=SoftPWM.size();softPWMchannel++){  //cycle through all the softPWM channels
      SoftPWM.set(softPWMchannel, 1);  //turn on all pins at brightness 1 - there is a big jump from 0 to 1 so I have decided to always have every led slightly on
  }
  //I'm setting these variables up here because the randomSeed(analogRead(0)) doesn't work at the top of the sketch where I'm declaring variables for some reason and I need to do the randomSeed(analogRead(0)) before picking random variables
  fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);  //the maximum bound of the rendomly chosen length of time that the fade will occur over
  fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);  //the minimum bound of the rendomly chosen length of time that the fade will occur over
  valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet)*(SoftPWM.brightnessLevels()-1)*3/100;  //rendomly picks the valueTotalMin from the set range so that there will be a different minimum brightness each power on - converts from a percent to a this should be set up to work with different numbers of brightness levels
  newPos();
}

void newColor(byte positionID){  //set the new color program for the given position
  fadeDelay = random(fadeDelayMin,fadeDelayMax);  //pick the end time for the fade to the new color
  byte newColorArray[3];
  for(byte e=0;e<=2;e++){
    newColorArray[e]=random(1,SoftPWM.brightnessLevels());  //pick random brightness values for R, G, and B
  }
  while(newColorArray[0]+newColorArray[1]+newColorArray[2]<valueTotalMin){  //make sure the brightness is greater than the minimum value
    for(byte e=0;e<=2;e++){
      newColorArray[e]=random(1,SoftPWM.brightnessLevels());  //pick new values
    }
  }
  for(byte e=0;e<=2;e++){
    program[e+(positionID-1)*3][0]=newColorArray[e];  //update the end brightness
    program[e+(positionID-1)*3][1]=millis()+fadeDelay;  //update the end time
  }
}

void standard(){  //shift/crossfade
  if(programStep==1){  //position one color shift
    colorShift();
  }
  if(programStep==2){  //crossfade position 1 to position2
    crossFade();
  }
}

void fadeUpBack(){  //randomly fades all leds to full brightness and then chooses new positions and a new color and fades one position to the new color and the other to 0 and then sends it back to the crossfade
  if(millis()>=fadeUpBackNextTime){
    if(programStep==4){  //crossfade position 1 to position2
      fadeBack();
    }
    else{
      programStep=3;
      fadeUp();
    }
  }
}

void strobe(){  //randomly turns all leds on full and then back to the previous value and program
  if(millis()>=strobeNextTime){  //check if it's time for a strobe
    if(programStep==6){  //is the strobe already on?
      strobeOff();  //if so check if it needs to be turned off
    }
    else{  //the strobe is not on already
      programStep=5;  //take the program control from the other functions
      strobeOn();  //turn on the strobe
    }
  }
}

void allShiftRandom(){  //fades all leds up to a random color and then shifts the color of all leds a random number of times, then picks new positions and new colors for position 1 and fades to the color on position 1 and to 0 on position 2 and then goes to crossfade
}

void newPos(){  //chooses the size of the new position and which pins are in it
//copy old position 2 to position 1 and get the new postition 2
  pos2size=random(0,SoftPWM.size()/3-pos1size); //new position 2
  for(byte i=0;i<=pos2size;i++){
    pos2[i]=random(0,SoftPWM.size()/3);  
    for(byte j=0;j<=pos1size;j++){  //make sure the led isn't already in position 1
      while(pos2[i]==pos1[j]){
        pos2[i]=random(0,SoftPWM.size()/3);
        j=0;  //restart the for loop and recheck to see if the new led number is already in position 1
        for(byte k=0;k<=i-1;k++){  //make sure the led isn't already in pos2
          while(pos2[k]==pos2[i]){
            pos2[i]=random(0,SoftPWM.size()/3);
            j=0;  //restart the for loop and recheck to see if the new led number is already in position 1
            k=0;  //restart the for loop and recheck to see if the new led number is already in position 2
          }
        }
      }
    }
  }
  for(byte i=3;i<=5;i++){
    program[i][2]=1;  //update the position 2 current brightness(1)
    program[i][3]=millis();  //update the position 2 last change time
    

    for(byte k=0;k<=pos2size;k++){  //START position1 = position2
      pos1[k]=pos2[k];  
    }
    pos1size=pos2size;
    for(byte k=0;k<=2;k++){
      program[k][2]=program[k+3][2];  //copy current brightness and last brightness change time from position 1 to position 2
      program[k][3]=program[k+3][3];        
    }  //END position1=position2
    
  }
}

void colorShift(){
  if(millis()>= program[0][1]){  //check if the previous program is complete
    newColor(2);  //set new color for position 1
    programStep=2;  //switch to the crossfade
    //digitalWrite(13, HIGH);  //for debugging
  }
}

void crossFade(){
  if(millis()>=program[0][1]){  //check if the previous program is complete
    newPos();  //new position 2
    newColor(2);  //new color on position 2
    for(byte t=0;t<=2;t++){
      program[t][0]=1;  //update the position 1 end brightness(0)
      program[t][1]=millis()+fadeDelay;  //update the position 1 end time
    }
    programStep=1;  //switch back to the shift
//      digitalWrite(13, LOW);  //for debugging
  }
}
void fadeUp(){  //set the target brightness to 255 for all leds and the fade end time to a random length of time
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
    pos1size=0;
    pos2size=SoftPWM.size()/3-1;  //position 2 is large enough to fit all leds - it might be better to use both positions?
    for(byte w=0;w<SoftPWM.size()/3;w++){  //step through the leds
      pos2[w]=w;  //put all the leds in position 2
    }
    fadeDelay=random(fadeDelayMin,fadeDelayMax);  //pick a length of time for the fadeUp
    for(byte b=3;b<=5;b++){  //step through position 2 program rows
      program[b][0] = 255;  //set target brightness
      program[b][1] = millis()+fadeDelay;  //set fade end time
    }
   programStep=4;
  } 
}
void fadeBack(){  //this picks new positions and a new color and fades one position to the new color and the other to 0
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
//    newPos(2);
//    newPos(1);
    newColor(1);  //new color for position 1
    for(byte b=3;b<=5;b++){  //step through the position 2 rows
      program[b][0] = 1;  //set target brightness 
      program[b][1] = millis()+fadeDelay;  //set fade end time
    }
  fadeUpBackNextTime=millis()+random(fadeUpBackDelayMin,fadeUpBackDelayMax);
  programStep=2;  //the next step will be the crossfade - it might be best to make it go back to the previous(the value before this function was called) programStep to make it compatible with more combos
  } 
}
void fader(){
  for(byte i=0;i<=5;i++){  //cycle through the program rows
    if(program[i][0] != program[i][2]){  //no change needed if the start and end brightness are equal
      if(millis()-program[i][3]>=(program[i][1]-program[i][3])/(abs(program[i][0]-program[i][2])+1)){  //check if it's time to change the brightness
        if(program[i][0]-program[i][2]>0){  //check if it is a positive change
          program[i][2]=program[i][2]+1;  //update the current brightness
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
          program[i][2]=program[i][2]-1; //update the current brightness
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
void strobeOn(){  //this needs to save the program array in a different array and then set the program to full on and then after the strobeTime set it back to the original program array with strobeTime added to all end and last times in the array
    if(millis()>=strobeNextTime){  //turn on the strobe
      for(byte row=0;row<=5;row++){    //step through the RGB for position 1 and 2
        for(byte col=0;col<=3;col++){  //and the progam columns
          lastProgram[row][col]=program[row][col];  //save the current program
        }
        program[row][0]=255;  //target brightness
        program[row][2]=254;  //current brightness
        program[row][1]=millis()+10;  //fade end time
      }
      digitalWrite(13, HIGH);   //
      oldProgramStep=programStep;  //save the current programStep so it can resume where it left off
      programStep=6;  //next step is strobeOff
      strobeOffTime=millis()+random(strobeTimeMin,strobeTimeMax);  //randomly choose how long the strobe will be on
      strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax)+strobeOffTime;  //randomly choose the next time it will strobe
    }
}
void strobeOff(){
  if(millis()>=strobeOffTime){  //this will just hit the min every time but it will make it still work until I do the real strobe with the program array updates
    for(byte row=0;row<=5;row++){    //step through the RGB for position 1 and 2
      for(byte col=0;col<=3;col++){
         program[row][col]=lastProgram[row][col];  //reset the program to the pre-strobe values - this would be better if it added the strobetime to the numbers
      }
    }
    programStep=oldProgramStep;  //resume where it was before the strobe
    //digitalWrite(13, LOW);   //
  }
}

void allShift(){  //color shifts all leds simultaneously to random colors this will most likely not work with standard()
  if(millis()>= program[3][1]){  //check if the position 2 color shift is complete
    pos2size=4;  //position one is set to contain all the RGBs this is not optimum to set the variable over and over again
    for(byte u=0;u<SoftPWM.size()/3;u++){  //step through the RGB leds;
      pos2[u]=u;  //put ell RGB leds in position 1
     }
    newColor(2);  //set new color for position 1
  }
}

void loop(){
script(scriptID);  //the script function runs the collection of functions in the scriptID that is set in the current profile
fader();  //the fader function changes brightness values to reach the target brightness in the program by the end time for the fade
//interruptLoadCheck()  //for debugging
//sramCheck();  //for debugging
//debugBlink();  //for debugging
//millisBlink();  //for debugging
}

/*
void interruptLoadCheck(){  //prints the interrupt load
  if(millis()>lastCheckTime+4000){  //4000 is the time between checks
    SoftPWM.printInterruptLoad();
    lastCheckTime=millis();
  }
}*/

/*
void sramCheck(){  //turns on led if there is less than the set value of free sram
  if(millis()>lastCheckTime+200){  //200 is the time between checks
    //Serial.print("sramFree:");  //uses sram I think so best just to use the led
    //Serial.println(freeMemory());  //uses sram I think so best just to use the led
    digitalWrite(13, LOW);
    if(freeMemory()<1000){  //checks to make sure there is 1000 bytes of sram free, that is plenty enough to spare
      digitalWrite(13, HIGH);  //turns on the onboard led if there is less than 1000 bytes free
    }
    lastCheckTime=millis();
  } 
}
*/

/*
void debugBlink(){  //blinks pin 13
  if(millis()>=debugBlinkNextTime){
    if (debugBlinkState==0){
      digitalWrite(13, HIGH);
      debugBlinkNextTime=millis()+50;
      debugBlinkState=1;
    }
    else{
      digitalWrite(13, LOW);
      debugBlinkNextTime=millis()+500;
      debugBlinkState=0;
    }
  }
}
*/

/*
void millisBlink(){
  if(millis()>=32000){
    digitalWrite(13, HIGH);
  }
}
*/
