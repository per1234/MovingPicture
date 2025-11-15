//these are the functions that are put together in the script function
//Main Scriptlets:
void standard(){  //shift/crossfade
//hotPotato=0 means the base script has control and can change values, hotPotato>0 means an addon script has control and the main program is disabled
  if(programControl==0){  //check if the main script has control
    if(standardStep==0){  //position one color shift  - use a different programStep variable for each program and then a hotpotato variable to decide which program currently has control and then it can return to where it was before hot potato array with premain program id,  via lasthotpotato variable
      colorShift();
    }
    if(standardStep==1){  //crossfade position 1 to position2
      crossFade();
    }
  }
}

void allShift(){  //color shifts all leds simultaneously to random colors this will most likely not work with standard()
  if(programControl==0){  //check if the main program has control
    if(millis()>= program[3][1]){  //check if the position 2 color shift is complete
      pos2size=4;  //position one is set to contain all the RGBs this is not optimum to set the variable over and over again
      for(byte u=0;u<SoftPWM.size()/3;u++){  //step through the RGB leds;
        pos2[u]=u;  //put ell RGB leds in position 1
      }
      newColor(2);  //set new color for position 1
    }
  }
}

//add-on scriptlets:

void fadeUpBack(){  //randomly fades all leds to full brightness and then chooses new positions and a new color and fades one position to the new color and the other to 0 and then sends it back to the crossfade
  if(millis()>=fadeUpBackNextTime){
    programControl=1;  //this takes control of the program from the main script
    if(fadeUpBackStep==0){  //crossfade position 1 to position2
      fadeUp();
    }
    else{
      fadeBack();
    }
  }
}

void strobe(){  //randomly turns all leds on full and then back to the previous value and program
  if(millis()>=strobeNextTime){  //check if it's time for a strobe
    programControl=1;  //this takes control of the program from the main script
    if(strobeStep==0){  //is the strobe already on?
      strobeOn();  //if so check if it needs to be turned off
    }
    else{  //the strobe is not on already
      strobeOff();  //turn on the strobe
    }
  }
}

void allShiftRandom(){  //fades all leds up to a random color and then shifts the color of all leds a random number of times, then picks new positions and new colors for position 1 and fades to the color on position 1 and to 0 on position 2 and then goes to crossfade
  if(millis()>=allShiftRandomNextTime){  //check if it's time for an AllShiftRandom
    programControl=1;  //this takes control of the program from the main script
    if(allShiftRandomStep==0){
      allShiftRandomTimes=random(1,allShiftRandomMax+1);  //decide how many times to allShift
      allShiftRandomStep=1;
    }
    if(allShiftRandomStep<=allShiftRandomTimes){
      allShiftOnce();
    }
    else{  //finish the allShiftRandom
      newColor(2);  //color shift on position2
      fadeOut();  //fadeOut position1
      programControl=0;
      allShiftRandomNextTime=millis()+random(allShiftRandomDelayMin,allShiftRandomDelayMax)+fadeDelay;
    }
  }
}

//scriptlet components:

void allShiftOnce(){
  if(millis()>= program[3][1]){  //check if the position 2 color shift is complete
    pos2size=4;  //position one is set to contain all the RGBs this is not optimum to set the variable over and over again
    for(byte u=0;u<SoftPWM.size()/3;u++){  //step through the RGB leds;
      pos2[u]=u;  //put ell RGB leds in position 1
    }
    newColor(2);  //set new color for position 1
    allShiftRandomStep=allShiftRandomStep+1;
  }
}  

void colorShift(){
  if(millis()>= program[0][1]){  //check if the previous program is complete
    newColor(2);  //set new color for position 1
    standardStep=1;  //switch to the crossfade
    //digitalWrite(13, HIGH);  //for debugging
  }
}

void crossFade(){
  if(millis()>=program[0][1]){  //check if the previous program is complete
    pos1equalsPos2();
    newPos();  //new position 2
    newColor(2);  //new color on position 2
    fadeOut();  //fade postition 1 to brightness 1
    standardStep=0;  //switch back to the shift
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
    fadeUpBackStep=1;
  } 
}

void fadeBack(){  //this picks new positions and a new color and fades one position to the new color and the other to 0
  if(millis()>= program[3][1]){  //check if the fade for position 2 is complete
//    newPos(2);  //the newPos() function doesn't currently have a positionID parameter, maybe I could just put all RGBLEDs in one position
//    newPos(1);
    newColor(1);  //new color for position 1
    for(byte b=3;b<=5;b++){  //step through the position 2 rows
      program[b][0] = 1;  //set target brightness 
      program[b][1] = millis()+fadeDelay;  //set fade end time the fadeDelay variable has just been randomly set by the newColor() function so this will match the position 2 color shift
    }
  fadeUpBackNextTime=millis()+random(fadeUpBackDelayMin,fadeUpBackDelayMax);
  fadeUpBackStep=0;  //the next step will be the crossfade - it might be best to make it go back to the previous(the value before this function was called) programStep to make it compatible with more combos
  programControl=0;  //give the program control back to the main script
  } 
}

void strobeOn(){  //this needs to save the program array in a different array and then set the program to full on and then after the strobeTime set it back to the original program array with strobeTime added to all end and last times in the array
    if(millis()>=strobeNextTime){  //turn on the strobe
      for(byte row=0;row<=5;row++){    //step through the RGB for position 1 and 2
        lastTargetBrightness[row]=program[row][0];  //save the current program - I only need to save target brightness for the 6 rows
        program[row][0]=program[row][2];  //set the target brightness to the current brightness so the fader will not change brightness while the strobe is on
      }
      //digitalWrite(13, HIGH);   //for debugging
      strobeStep=1;  //next step is strobeOff
      strobeOffTime=millis()+random(strobeTimeMin,strobeTimeMax);  //randomly choose how long the strobe will be on
      strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax)+strobeOffTime;  //randomly choose the next time it will strobe
    }
}

void strobeOff(){
  if(millis()>=strobeOffTime){
    for(byte row=0;row<=5;row++){    //step through the RGB for position 1 and 2
      program[row][0]=lastTargetBrightness[row];  //reset the program to the pre-strobe values - this would be better if it added the strobetime to the end time but that will require another long
    }
    programControl=1;  //this takes control of the program from the main script
    strobeStep=0;
    //digitalWrite(13, LOW);   //debugging
  }
}

void fadeOut(){ //sets position 1 brightness to 1, make sure to do a newColor() or otherwise set the fadeDelay first
  for(byte t=0;t<=2;t++){
    program[t][0]=1;  //update the position 1 end brightness(0)
    program[t][1]=millis()+fadeDelay;  //update the position 1 end time
  }
}
  
