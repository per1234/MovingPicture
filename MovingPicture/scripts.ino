//these are the functions that are put together in the script function
//Main scripts: - move flag changes from the components to the actual scripts to make the components more multifunctional, this could be done via the step or via a return

void standard(){  //shift/crossfade
  if(programControl==0){  //check if the main script has control
    if(standardStep==0){  //position one color shift
      colorShift();
    }
    if(standardStep==1){  //crossfade position 1 to position2
      crossFade();
    }
  }
}

void allShift(){  //color shifts all leds simultaneously to random colors this will most likely not work with standard()
  if(programControl==0){  //check if the main program has control
    allShiftOnce();
  }
}

void strobeMain(){  //sets the brightness levels to 0 for the maximum strobe only effect add color option
  if(programControl==0 || strobeStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(strobeStep==0){
      strobeOn();
    }
    if(strobeStep==1){
      strobeOffFull();
    }
  }
}

//add-on scripts:

void strobe(){  //randomly turns all leds on full and then back to the previous value and program add random color option - this doesn't look good on scripts with lots of all-led on time because the strobe only has a dramatic effect when there is a significant contrast, otherwise it just looks like a hiccup in the program
  if(programControl==0 || strobeStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(strobeStep==0){
      strobeOn();
    }
    if(strobeStep==1){
      strobeOff();
    }
  }
}

void fadeUpBack(){  //randomly fades all leds to full brightness and then chooses new positions and a new color and fades one position to the new color and the other to 0 and then sends it back to the crossfade
  if(programControl==0 || fadeUpBackStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(millis()>=fadeUpBackNextTime && standardStep==1){  //start after the color shift on the standard script
      if(fadeUpBackStep==0){  //setup
        fadeUpBackStep=1;  //this flags the fadeUpBack script as being in action
        programControl=1;  //this takes control of the program from the main script
      }
      if(fadeUpBackStep==1){
        fadeUp();
      }
      if(fadeUpBackStep==2){
        fadeBack();
      }
    }
  }
}

void allShiftRandom(){  //fades all leds up to a random color and then shifts the color of all leds a random number of times, then picks new positions and new colors for position 1 and fades to the color on position 1 and to 0 on position 2 and then goes to crossfade
  if(programControl==0 || allShiftRandomStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(millis()>=allShiftRandomNextTime && standardStep==1){  //check if it's time for an AllShiftRandom and it is ready for a crossfade
      programControl=1;  //this takes control of the program from the main script
      if(allShiftRandomStep<=allShiftRandomIterations){
        allShiftOnce();
      }
      if(allShiftRandomStep>allShiftRandomIterations){  //finish the allShiftRandom
        allShiftRandomNextTime=millis()+random(allShiftRandomDelayMin,allShiftRandomDelayMax)+fadeDelay;
        allShiftRandomStep=0;  //reset for next time
        fadeBack();
      }
    }
  }
}

//script components:
void allShiftOnce(){
  if(millis()>= program[0][1] && millis()>= program[3][1]){  //check if the fade is complete
    if(allShiftRandomStep==0){  //setup
      allShiftRandomIterations=random(1,allShiftRandomMax+1);  //decide how many times to allShift used only by the allShiftRandom add-on
      fillAllPos2();  //position 2 gets all the RGBLEDs not already in position 1
    }
    newColor(2);  //set new color for position 2
    for(byte i=0;i<=2;i++){  //copy over the new program from position 2 to position 1
      for(byte j=0;j<=1;j++){
        program[i][j]=program[i+3][j];
      }
    }  
    allShiftRandomStep=allShiftRandomStep+1;
  }
}

void colorShift(){  //fades to a different color on position 2
  if(millis()>= program[0][1] && millis()>= program[3][1]){  //check if the previous program is complete
    newColor(2);  //set new color for position 2
    standardStep=1;  //switch to the crossfade
    //digitalWrite(10, LOW);  //for debugging
  }
}

void crossFade(){  //fades out position 1 and fades position 2 to a new color
  if(millis()>= program[0][1] && millis()>=program[3][1]){  //check if the previous program is complete
    pos1equalsPos2();
    newPos2();  //new position 2
    newColor(2);  //new color on position 2
    for(byte i=3;i<=5;i++){
      program[i][2]=offLevel;  //set the current brightness level for position 2 to off
    }
    fadeOutPos1();  //fade postition 1 to offLevel
    standardStep=0;  //switch back to the colorShift
    //digitalWrite(10, HIGH);  //for debugging
  }
}

void fadeUp(){  //set the target brightness to 255 for all leds and the fade end time to a random length of time
  if(millis()>= program[0][1] && millis()>= program[3][1]){  //check if the fade is complete
    digitalWrite(10, HIGH);  //for debugging
    pos1equalsPos2();
    fillAllPos2();  //make sure all RGBLEDs are in position 1 or position 2
    fadeDelay=random(fadeDelayMin,fadeDelayMax);  //pick a length of time for the fadeUp
    for(byte b=0;b<=5;b++){  //step through the program rows
      program[b][0] = SoftPWM.brightnessLevels()-1;  //set target brightness
      program[b][1] = millis()+fadeDelay;  //set fade end time
      program[b][3] = millis();  //set last brightness change time
    }
    fadeUpBackStep=2;  //next step is fadeBack
  } 
}

void fadeBack(){  //this picks new positions and a new color and fades one position to the new color and the other to 0
  if(millis()>= program[0][1] && millis()>= program[3][1]){  //check if the fade is complete
    //newPos2();  //new position 2
    //pos1equalsPos2(); //fills position 1
    //fillAllPos2();  //new position 2 - this needs to take all the free pins
    newColor(2);  //new color for position 2
    fadeOutPos1();  //fade out position 1
    fadeUpBackNextTime=millis()+random(fadeUpBackDelayMin,fadeUpBackDelayMax)+fadeDelay;  //when the next fadeUpBack will occur
    programControl=0;  //give the program control back to the main script
    standardStep=1;  //next step is the crossfade
    digitalWrite(10, LOW);  //for debugging
      fadeUpBackStep=0;  //fadeUpBack is no longer in action
  } 
}

void strobeOn(){
  if(millis()>=strobeNextTime){  //time to turn on the strobe
    for(byte i=0;i<=5;i++){    //step through the program rows for position 1 and 2
      lastTargetBrightness[i]=program[i][0];  //save the target brightness for the 6 rows
      //Serial.println(program[i][0]);
      program[i][0]=program[i][2];  //set the target brightness to the current brightness to disable the fader while the strobe is on
    }
    for(byte i=0;i<SoftPWM.size();i++){  //turn them all on
      SoftPWM.set(i,SoftPWM.brightnessLevels()-1);
    }
    programControl=1;  //this takes control of the program from the main script
    strobeStep=1;  //next step is strobeOff/strobeOffFull
    strobeTime=random(strobeTimeMin,strobeTimeMax);  //chose the lenth of time the strobe will be on
    strobeNextTime=millis()+strobeTime;
  }
}

void strobeOffFull(){  //turns the pins all off to 0 for use with the strobeMain for the maximum strobe effect
  if(millis()>=strobeNextTime){
    for(byte i=0;i<SoftPWM.size();i++){  //turn them all off
      SoftPWM.set(i,0);
    }
    strobeStep=0;  //reset for next time
    programControl=0;  //give control back to the main script
    strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax);  //randomly choose the next time it will strobe
  }
}

void strobeOff(){
  if(millis()>=strobeNextTime){
    for(byte i=0;i<=5;i++){    //step through the RGB for position 1 and 2
      program[i][0]=lastTargetBrightness[i];  //reset the program to the pre-strobe values
      program[i][1]=program[i][1]+strobeTime;  //add on the length of time the strobe was on to the fade end time in the program
    //Serial.println(program[row][0]);
    }
    setter();  //set all leds back to their previous brightness
    strobeStep=0;  //reset for next time
    programControl=0;  //give control back to the main script    
    strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax);  //randomly choose the next time it will strobe
    //digitalWrite(13, HIGH);   //debugging
  }
}

void fadeOutPos1(){ //sets position 1 brightness to 1, make sure to do a newColor() or otherwise set the fadeDelay first
  for(byte t=0;t<=2;t++){
    program[t][0]=offLevel;  //update the position 1 end brightness(0)
    program[t][1]=millis()+fadeDelay;  //update the position 1 end time
    program[t][3]=millis();  //update the last brightness change time
  }
}
  
