/*
new position - copy old position 2 to postiinot 1 and get the new postition 2
all fade
strobe
ministrobe
rainbow chase
*/
void setup(){
//  Serial.begin(9600);  //for debugging
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

void newColor(byte positionID){  //
    fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new brightness values
    byte valueRb=random(1,SoftPWM.brightnessLevels());  //change
    byte valueGb=random(1,SoftPWM.brightnessLevels());
    byte valueBb=random(1,SoftPWM.brightnessLevels());
    while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
      valueRb=random(1,SoftPWM.brightnessLevels());
      valueGb=random(1,SoftPWM.brightnessLevels());
      valueBb=random(1,SoftPWM.brightnessLevels());
    }
    program[0+(positionID-1)*3][0]=valueRb;  //update the end brightness
    program[1+(positionID-1)*3][0]=valueGb;    
    program[2+(positionID-1)*3][0]=valueBb;   
    program[0+(positionID-1)*3][1]=millis()+fadeDelay;  //update the end time
    program[1+(positionID-1)*3][1]=millis()+fadeDelay;      
    program[2+(positionID-1)*3][1]=millis()+fadeDelay;
}

void standard(){
    if(programStep==1){  //position one color shift
    colorShift();
  }
  if(programStep==2){  //crossfade position 1 to position2
    crossFade();
  }
}
void fadeUpDown(){
    if(programStep==1){  //position one color shift
    fadeUp();
  }
  if(programStep==2){  //crossfade position 1 to position2
    crossFade();
  }
}

void colorShift(){
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
    newColor(1);  //set new color for position 1
    programStep=2;  //switch to the crossfade
    //digitalWrite(13, HIGH);  //for debugging
  }
}
void crossFade(){
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
    newColor(2);  //new color on position 2
    program[3][2]=1;  //update the position 2 current brightness(0)
    program[4][2]=1;    
    program[5][2]=1;
    program[3][3]=millis();  //update the position 2 last change time
    program[4][3]=millis();      
    program[5][3]=millis();
 
    program[0][0]=1;  //update the position 1 end brightness(0)
    program[1][0]=1;    
    program[2][0]=1;
    program[0][1]=millis()+fadeDelay;  //update the position 1 end time
    program[1][1]=millis()+fadeDelay;      
    program[2][1]=millis()+fadeDelay;
    programStep=1;  //switch back to the shift
//      digitalWrite(13, LOW);  //for debugging
  }
}
void fadeUp(){  //this needs to be reworked to 
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
    pos1size=0;
    pos2size=3;
    for(byte w=1;w<SoftPWM.size()/3;w++){
      pos2[w-1]=w;
    }
    for(byte b=0;b<=5;b++){
      program[b][0] = 255;  //set target brightness
      program[b][1] = millis()+10000;  //set fade end time
    }
   programStep=4;
  } 
}
void fadeDown(){
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
    for(byte b=0;b<=5;b++){
      program[b][0] = 1;  //set target brightness 
      program[b][1] = millis()+10000;  //set fade end time
    }
   programStep=3;
  } 
}
void fader(){
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
void strobe(){  //this needs to save the program array in a different array and then set the program to full on and then after the strobeTime set it back to the original program array with strobeTime added to all end and last times in the array
  if(strobeFlag==1){
    if(millis()>=strobeOffTime){  //this will just hit the min every time but it will make it still work until I do the real strobe with the program array updates
      digitalWrite(13, LOW);
      strobeFlag=0;
    }
  }
  if(millis()>=strobeNextTime){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    strobeOffTime=millis()+random(strobeTimeMin,strobeTimeMax);  //randomly choose how long the strobe will be on
    strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax)+strobeOffTime;  //randomly choose the next time it will strobe
    strobeFlag=1;  //strobe is on
  }
}

void loop(){
script(scriptID);
fader();
//interruptLoadCheck()  //for debugging
//sramCheck();  //for debugging
}

/*
void interruptLoadCheck(){  //prints the interrupt load at random times - don't use this function and the sramCheck at the same time because they use the same lastTime variable
  if(millis()>lastCheckTime+4000){  //4000 is the minimum time between checks
    byte x;
    if(x==random(0,4)){  //randomizes the interval
      SoftPWM.printInterruptLoad();
    }
    lastCheckTime=millis();
  }
}*/

/*
void sramCheck(){  //prints the free sram at random times
  if(millis()>lastCheckTime+200){  //200 is the time between checks
    //Serial.print("sramFree:");  //uses sram I think so best just to use the led
    //Serial.println(freeMemory());  //uses sram I think so best just to use the led
    digitalWrite(13, LOW);
    if(freeMemory()<1000){  //checks to make sure there is 1000 bytes of sram free, that it plenty enough to spare
      digitalWrite(13, HIGH);  //turns on the onboard led if there is less than 1000 bytes free
    }
    lastCheckTime=millis();
  } 
}
*/
