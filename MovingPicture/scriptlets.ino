//these are the functions that are put together in the script function
//Main Scriptlets: - move flag changes from the components to the actual scriptlets to make the components more multifunctional, this could be done via the step or via a return

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

void strobeMain(){  //sets the brightness levels to 0 for the maximum strobe only effect add color option - unless I can make it compatible with the add-ons better to just have a simple sketch with delay and digitalWrite
  if(programControl==0 || strobeStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(strobeStep==0){
      //fillAllPos();
      for(byte i=0;i<=5;i++){  //this is not ideal to set all the variables every time
        program[i][2]=0;
      }
      strobeOn();
      offLevel=0;
    }
    if(strobeStep==1){
      strobeOff();
    }
  }
}

//add-on scriptlets:

void strobe(){  //randomly turns all leds on full and then back to the previous value and program add random color option
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
}

void allShiftRandom(){  //fades all leds up to a random color and then shifts the color of all leds a random number of times, then picks new positions and new colors for position 1 and fades to the color on position 1 and to 0 on position 2 and then goes to crossfade
  if(programControl==0 || allShiftRandomStep>0){  //check if the main program has control, don't want to activate if another add-on has control
    if(millis()>=allShiftRandomNextTime){  //check if it's time for an AllShiftRandom
      programControl=1;  //this takes control of the program from the main script
      if(allShiftRandomStep<=allShiftRandomIterations){
        allShiftOnce();
      }
      else{  //finish the allShiftRandom
        fillAllPos();
        newPos();
        pos1equalsPos2();
        newPos();
        newColor(2);  //color shift on position2
        fadeOut();  //fadeOut position1
        standardStep=0;  //next step is the crossfade
        programControl=0;  //give control back to the main scriptlet
        allShiftRandomNextTime=millis()+random(allShiftRandomDelayMin,allShiftRandomDelayMax)+fadeDelay;
        allShiftRandomStep=0;  //reset for next time
      }
    }
  }
}

//scriptlet components:

void allShiftOnce(){
  if(millis()>= program[3][1]){  //check if the position 2 color shift is complete
//  if(allShiftRandomStep  fillAllPos();
    if(allShiftRandomStep==0){  //setup
      allShiftRandomIterations=random(1,allShiftRandomMax+1);  //decide how many times to allShift
      pos2size=4;  //position one is set to contain all the RGBs this is not optimum to set the variable over and over again
      for(byte u=0;u<SoftPWM.size()/3;u++){  //step through the RGB leds;
        pos2[u]=u;  //put ell RGB leds in position 1
      }
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
    fillAllPos();  //make sure all RGBLEDs are in position 1 or position 2
    fadeDelay=random(fadeDelayMin,fadeDelayMax);  //pick a length of time for the fadeUp
    for(byte b=0;b<=5;b++){  //step through the program rows
      program[b][0] = 255;  //set target brightness
      program[b][1] = millis()+fadeDelay;  //set fade end time
    }
    fadeUpBackStep=1;
  } 
}

void fadeBack(){  //this picks new positions and a new color and fades one position to the new color and the other to 0
  if(millis()>= program[0][1]){  //check if the fade is complete
    newPos();  //new position 2
    pos1equalsPos2(); //fills position 2
    newPos();  //new position 2
    newColor(2);  //new color for position 2
    fadeOut();  //fade out position 1
    fadeUpBackNextTime=millis()+random(fadeUpBackDelayMin,fadeUpBackDelayMax);  //when the next fadeUpBack will occur
    fadeUpBackStep=0;  //reset for next time
    programControl=0;  //give the program control back to the main script
    standardStep=0;  //next step is the crossfade
  } 
}

void strobeOn(){  //this needs to save the program array in a different array and then set the program to full on and then after the strobeTime set it back to the original program array with strobeTime added to all end and last times in the array
  if(millis()>=strobeNextTime){  //turn on the strobe
    for(byte i=0;i<=5;i++){    //step through the RGB for position 1 and 2
      lastTargetBrightness[i]=program[i][0];  //save the current program - I only need to save target brightness for the 6 rows
      //Serial.println(program[i][0]);
      program[i][0]=program[i][2];  //set the target brightness to the current brightness so the fader will not change brightness while the strobe is on
    }
    for(byte i=0;i<SoftPWM.size();i++){  //turn them all on
      SoftPWM.set(i,255);
    }
    programControl=1;  //this takes control of the program from the main script
    strobeStep=1;  //next step is strobeOff
    strobeNextTime=millis()+random(strobeTimeMin,strobeTimeMax);  //randomly choose how long the strobe will be on
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
      program[i][0]=lastTargetBrightness[i];  //reset the program to the pre-strobe values - this would be better if it added the strobetime to the end time but that will require another long
  //        Serial.println(program[row][0]);
    }
    setter();
    strobeStep=0;  //reset for next time
    programControl=0;  //give control back to the main script    
    strobeNextTime=millis()+random(strobeDelayMin,strobeDelayMax);  //randomly choose the next time it will strobe
    //digitalWrite(13, HIGH);   //debugging
  }
}

void fadeOut(){ //sets position 1 brightness to 1, make sure to do a newColor() or otherwise set the fadeDelay first
  for(byte t=0;t<=2;t++){
    program[t][0]=offLevel;  //update the position 1 end brightness(0)
    program[t][1]=millis()+fadeDelay;  //update the position 1 end time
  }
}
  
