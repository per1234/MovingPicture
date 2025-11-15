/*
new position
new color
all fade
strobe
ministrobe
rainbow chase
*/
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
/*    fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new brightness values
    byte valueRb=random(1,SoftPWM.brightnessLevels());  //change
    byte valueGb=random(1,SoftPWM.brightnessLevels());
    byte valueBb=random(1,SoftPWM.brightnessLevels());
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
    program[2][1]=millis()+fadeDelay;*/
    programStep=2;
    //digitalWrite(13, HIGH);
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
/*    fadeDelay = random(fadeDelayMin,fadeDelayMax);  //FUNCTIONALIZE pick the new values
    byte valueRb=random(1,SoftPWM.brightnessLevels());  
    byte valueGb=random(1,SoftPWM.brightnessLevels());
    byte valueBb=random(1,SoftPWM.brightnessLevels());
    while(valueRb+valueGb+valueBb<valueTotalMin){  //make sure the brightness is greater than the minimum value
      valueRb=random(1,SoftPWM.brightnessLevels());
      valueGb=random(1,SoftPWM.brightnessLevels());
      valueBb=random(1,SoftPWM.brightnessLevels());
    }*/
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

/*    program[3][0]=valueRb;  //update the posiiton 2 end brightness
    program[4][0]=valueGb;    
    program[5][0]=valueBb;
    program[3][1]=millis()+fadeDelay;  //update the position 2 end time
    program[4][1]=millis()+fadeDelay;      
    program[5][1]=millis()+fadeDelay;*/

    programStep=1;
//      digitalWrite(13, LOW);
  }
}
void fadeUp(){
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
void fadeDown(){
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
    for(byte b=0;b<=5;b++){
      program[b][0] = 1;
      program[b][1] = millis()+10000;  //[pos1R, pos1G, pos1B, pos2R, pos2G, pos2B][target brightness, end time, current brightness, last brightness change time]
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
    if(millis()>=lastStrobeTime+strobeTime){
      digitalWrite(13, LOW);
      strobeFlag=0;
    }
  }
  if(millis()>lastStrobeTime+strobeDelayMin){
    byte x;
    if(x==random(0,strobeLikelyhood)){
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      strobeFlag=1;
    }
    lastStrobeTime=millis();
  }
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
}
void sramCheck(){  //prints the free sram at random times
  if(millis()>lastCheckTime+4000){  //4000 is the minimum time between checks
    byte x;
    if(x==random(0,4)){  //randomizes the interval
      Serial.print("sramFree:");
      Serial.println(freeMemory());
    }
    lastCheckTime=millis();
  }
}*/
