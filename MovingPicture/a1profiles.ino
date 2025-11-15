//this tab must be named so that it will sort first after the top tab and be concatenated in the proper order
//uncomment the settings you want and leave the rest commented out

//test profile
const unsigned int fadeDelayMinSet = 3000;  //shortest shift time
const unsigned int fadeDelayMaxSet = 8000;  //the longest time that it will color shift.
const unsigned int fadeDelayDiffMin = 800;  //the minimum difference in the range of fade speeds
const unsigned int valueTotalMinMinSet = 40; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const unsigned int valueTotalMinMaxSet = 65; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMaxMinSet=15;  //the minimum bound of the randomly selected maximum change in value of each color. not yet implemented
//const int valueDiffMaxMaxSet=45;  //the maximum bound of the randomly selected maximum change in value of each color. not yet implemented
const byte strobeTimeMin=3;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const byte strobeTimeMax=30;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const unsigned int strobeDelayMin=800;  //the minimum time between strobes
const unsigned int strobeDelayMax=4000;  //the maximum time between strobes
//const byte strobeIterationsMin;  //the minimum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationsMax;  //the maximum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationDelayMin;  //the minimum length of time between strobe pulses - not yet implemented
//const byte strobeIterationDelayMax;  //the minimum length of time between strobe pulses - not yet implemented
const unsigned int fadeUpBackDelayMin=15000;  //this should be setup randomized eventually
const unsigned int fadeUpBackDelayMax=35000;  //this should be setup randomized eventually
const unsigned int allShiftRandomDelayMin=45000;  //this should be setup randomized eventually
const unsigned int allShiftRandomDelayMax=65000;  //this should be setup randomized eventually
const byte allShiftRandomMax=3;  //the maximum number of allShifts it will consecutively do before returning to the previous program
const byte offLevel=8;  //with 256 brightness levels the jump from 0 to 1 is too dramatic and makes it blinky, with more brightness levels this could change

void script(){
  //add-ons:
  fadeUpBack();  //works
  //allShiftRandom();  //locks up
  //strobe();  //works
  
  //main scripts:
  //strobeMain();  //works
  //allShift();  //works
  standard();  //works
}

/*
const unsigned int fadeDelayMinSet = 7000;  //shortest shift time
const unsigned int fadeDelayMaxSet = 20000;  //the longest time that it will color shift.
const unsigned int fadeDelayDiffMin = 5000;  //the minimum difference in the range of fade speeds
const unsigned int valueTotalMinMinSet = 40; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const unsigned int valueTotalMinMaxSet = 65; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMaxMinSet=15;  //the minimum bound of the randomly selected maximum change in value of each color. not yet implemented
//const int valueDiffMaxMaxSet=45;  //the maximum bound of the randomly selected maximum change in value of each color. not yet implemented
const byte strobeTimeMin=3;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const byte strobeTimeMax=30;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const unsigned int strobeDelayMin=800;  //the minimum time between strobes
const unsigned int strobeDelayMax=4000;  //the maximum time between strobes
//const byte strobeIterationsMin;  //the minimum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationsMax;  //the maximum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationDelayMin;  //the minimum length of time between strobe pulses - not yet implemented
//const byte strobeIterationDelayMax;  //the minimum length of time between strobe pulses - not yet implemented
const unsigned int fadeUpBackDelayMin=45000;  //this should be setup randomized eventually
const unsigned int fadeUpBackDelayMax=65000;  //this should be setup randomized eventually
const unsigned int allShiftRandomDelayMin=45000;  //this should be setup randomized eventually
const unsigned int allShiftRandomDelayMax=65000;  //this should be setup randomized eventually
const byte allShiftRandomMax=3;  //the maximum number of allShifts it will consecutively do before returning to the previous program
const byte offLevel=8;  //with 256 brightness levels the jump from 0 to 1 is too dramatic and makes it blinky, with more brightness levels this could change

void script(){
  //add-ons:
  //fadeUpBack();  //locks up
  //allShiftRandom();  //locks up
  //strobe();  //works
  
  //main scripts:
  //strobeMain();  //works
  //allShift();  //works
  standard();  //works
}
*/
