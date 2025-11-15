void setup(){
  randomSeed(analogRead(0));  //makes the pseudorandom number sequence start at a different position each boot - this needs a more random input
  //Serial.begin(9600);  //for debugging
  SoftPWM.begin(100);  //start the SoftPWM and set the PWM frequency - if the frequency is a little too high then there is some brightness fluctuation noticeable at lower brightness levels, too low will be noticeable pwm period, way too high causes unpredictable behavior
  for(byte softPWMchannel=0;softPWMchannel<=SoftPWM.size();softPWMchannel++){  //cycle through all the softPWM channels
    SoftPWM.set(softPWMchannel, offLevel);  //turn on all pins at the offLevel right from the start - there is a big jump from 0 to 1 so I have decided to always have every led slightly on
  }
  for(byte i=0;i<=5;i++){  //step through the program rows
    program[i][2]=offLevel;  //set the current brightness in the program to offLevel
  }
  //I'm setting these variables up here because the randomSeed(analogRead(0)) doesn't work at the top of the sketch where I'm declaring variables for some reason and I need to do the randomSeed(analogRead(0)) before picking random variables
  fadeDelayMax=random(fadeDelayMinSet + fadeDelayDiffMin, fadeDelayMaxSet);  //the maximum bound of the rendomly chosen length of time that the fade will occur over
  fadeDelayMin=random(fadeDelayMinSet, fadeDelayMax - fadeDelayDiffMin);  //the minimum bound of the rendomly chosen length of time that the fade will occur over
  valueTotalMin=random(valueTotalMinMinSet,valueTotalMinMaxSet)*(SoftPWM.brightnessLevels()-1)*3/100;  //rendomly picks the valueTotalMin from the set range so that there will be a different minimum brightness each power on - converts from a percent to brightness levels
  newPos2();  //it needs a position2 for the standard scriptlat to work
  pos1equalsPos2();  //probably not necessary but it seems a good idea to start with both positions full
  newPos2();
}

void newColor(byte positionID){  //set the new color program for the given position
  fadeDelay = random(fadeDelayMin,fadeDelayMax);  //pick the end time for the fade to the new color
  byte newColorArray[3];
  for(byte e=0;e<=2;e++){
    newColorArray[e]=random(offLevel,SoftPWM.brightnessLevels());  //pick random brightness values for R, G, and B
  }
  while(newColorArray[0]+newColorArray[1]+newColorArray[2]<valueTotalMin){  //make sure the brightness is greater than the minimum value
    for(byte e=0;e<=2;e++){
      newColorArray[e]=random(offLevel,SoftPWM.brightnessLevels());  //pick new values
    }
  }
  for(byte e=0;e<=2;e++){
    program[e+(positionID-1)*3][0]=newColorArray[e];  //update the end brightness
    program[e+(positionID-1)*3][1]=millis()+fadeDelay;  //update the end time
    program[e+(positionID-1)*3][3]=millis();  //update the last brightness change time    
  }
}

void setter(){  //sets the RGBLEDs to their current program values - currently used for recovery from the strobe() add-on
  byte setterFlag;
  for(byte i=0;i<=2;i++){
    for(byte j=0;j<=pos1size;j++){
      SoftPWM.set(pos1[j]*3+i, program[i][2]);
    }
  }
  for(byte i=3;i<=5;i++){
    for(byte j=0;j<=pos2size;j++){
      SoftPWM.set(pos2[j]*3+i-3, program[i][2]);
    }
  }
  for(byte i=0;i<SoftPWM.size()/3;i++){  //step through all RGBLEDs to see if any are not in a position
    setterFlag=0;
    for(byte j=0;j<=pos1size;j++){
      if(i==pos1[j]){  
        setterFlag=1;  //already in a position
      }
    }
    for(byte j=0;j<=pos2size;j++){
      if(i==pos2[j]){
        setterFlag=1;  //already in a position
      }
    }
    if(setterFlag==0){
      for(byte j=0;j<=2;j++){
        SoftPWM.set(i*3+j, offLevel);  //not in a position so turn it off
//      digitalWrite(13, HIGH);
      }
    }
  }
}

void pos1equalsPos2(){  //copy old position 2 to position 1
  for(byte k=0;k<=pos2size;k++){
    pos1[k]=pos2[k];  
  }
  pos1size=pos2size;
  for(byte k=0;k<=2;k++){
    for(byte l=0;l<=3;l++){
      program[k][l]=program[k+3][l];  //copy the program over from position 1 to position 2, I used to have only the current brightness and last brightness change time and that might be all I need
    }
  }
}

void newPos2(){  //New Position 2 - chooses the size of the new position and which pins are in it this just does postition 2 right now but it could have a position parameter easily
  pos2size=random(0,SoftPWM.size()/3-1-pos1size); //(zero indexed, hence the -1)new position 2 - this will have problems if position 1 or 2 have all the RGBLEDs call fillAllPos(); newPos2();pos1equalsPos2();newPos2(); to recover from all pins being put in one position
  for(byte i=0;i<=pos2size;i++){
    pos2[i]=random(0,SoftPWM.size()/3);  
    for(byte j=0;j<=pos1size;j++){  //make sure the led isn't already in position 1
      while(pos2[i]==pos1[j]){
        pos2[i]=random(0,SoftPWM.size()/3);
        j=0;  //restart the for loop and recheck to see if the new led number is already in position 1
        for(byte k=0;k<=i-1;k++){  //make sure the led isn't already in pos2, won't run if this is the first RGBLED in the position
          while(pos2[k]==pos2[i]){
            pos2[i]=random(0,SoftPWM.size()/3);
            j=0;  //restart the for loop and recheck to see if the new led number is already in position 1
            k=0;  //restart the for loop and recheck to see if the new led number is already in position 2
          }
        }
      }
    }
  }
}

void fillAllPos2(){  //puts all the RGBLEDs not already in position 1 in postition 2
  pos2size=0;
  fillAllPos2count=0;
  for(byte i=0;i<=SoftPWM.size()/3-1;i++){  //step through all the RGBLEDs
    fillAllPos2flag=0;
    for(byte j=0;j<=pos1size;j++){
      if(pos1[j]==i){  //the RGBLED is in position 1
        fillAllPos2flag=1;
      }
    }
    if(fillAllPos2flag==0){  //the RGBLED wasn't in position 1
      pos2size=fillAllPos2count++;  //this has to be set up weird like this because of the zero indexed pos2size
      pos2[pos2size]=i;
    }
  }
  for(byte b=3;b<=5;b++){  //step through the position 2 program rows
      program[b][2] = offLevel;  //set current brightness
      //program[b][3]=millis();  //set the last brightness change time
  }
}

void fader(){  //the current code only allows the fader to increment by 1 even if the program calls for faster, this seems like it enforces smooth fades over reaching the goal but will not work well for instant brightness change
  for(byte i=0;i<=5;i++){  //cycle through the program rows
    if(program[i][0] != program[i][2] && millis()-program[i][3]>=(program[i][1]-program[i][3])/(abs(program[i][0]-program[i][2])+1)){  //no change needed if the start and end brightness are equal
      if(program[i][0]>program[i][2]){  //check if it is a positive change
        program[i][2]++;  //update the current brightness
      }
      if(program[i][0]<program[i][2]){  //check if it is a negative change
        program[i][2]--; //update the current brightness
      }
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
      program[i][3]=millis();  //update the last brightness change time
    }
  }
}

void loop(){
script();  //runs the functions currently uncommented in the profiles tab
fader();  //the fader function changes brightness values to reach the target brightness in the program by the end time for the fade
fader();  //I think more calls to the fader might make for smoother fades, the script() function doesn't need to update anywhere near as frequently
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


void debugBlink(){  //blinks pin 13
  if(millis()>=debugBlinkNextTime){
    if (debugBlinkState==0){
      digitalWrite(10, HIGH);
      debugBlinkNextTime=millis()+50;
      debugBlinkState=1;
    }
    else{
      digitalWrite(10, LOW);      debugBlinkNextTime=millis()+500;
      debugBlinkState=0;
    }
  }
}


/*
void millisBlink(){
  if(millis()>=32000){
    digitalWrite(13, HIGH);
  }
}
*/
