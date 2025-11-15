//this tab must be named so that it will sort first after the top tab and be concatenated in the proper order
//uncomment the settings you want and leave the rest commented out

//test profile
const int fadeDelayMinSet = 3000;  //shortest shift time
const int fadeDelayMaxSet = 16000;  //the longest time that it will color shift.
const int fadeDelayDiffMin = 700;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMaxMinSet=15;  //the minimum bound of the randomly selected maximum change in value of each color. not yet implemented
//const int valueDiffMaxMaxSet=45;  //the maximum bound of the randomly selected maximum change in value of each color. not yet implemented
const long strobeTimeMin=3;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const long strobeTimeMax=30;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const long strobeDelayMin=800;  //the minimum time between strobes
const long strobeDelayMax=4000;  //the maximum time between strobes
//const byte strobeIterationsMin;  //the minimum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationsMax;  //the maximum number of strobe pulses each time it fires - not yet implemented
//const byte strobeIterationDelayMin;  //the minimum length of time between strobe pulses - not yet implemented
//const byte strobeIterationDelayMax;  //the minimum length of time between strobe pulses - not yet implemented
const int fadeUpBackDelayMin=8000;  //this should be setup randomized eventually
const int fadeUpBackDelayMax=10000;  //this should be setup randomized eventually
const int allShiftRandomDelayMin=10000;  //this should be setup randomized eventually
const int allShiftRandomDelayMax=18000;  //this should be setup randomized eventually
const byte allShiftRandomMax=4;  //the maximum number of allShifts it will consecutively do before returning to the previous program - not implemented yet right now the minimum is 1
byte offLevel=1;  //with 256 brightness levels the jump from 0 to 1 is too dramatic and makes it blinky, with more brightness levels this could change this is not a constant because the strobeMain changes it
const byte scriptID=0;  //the ID number of the script in the scripts tab that is used in this profile


/*
//standard shift/crossfade
const int fadeDelayMinSet = 5000;  //shortest shift time
const int fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
const int fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
const byte strobeTimeMin=5;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const byte strobeTimeMax=20;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const int strobeDelayMin=500;  //the minimum time between strobes
const int strobeDelayMax=2000;  //the maximum time between strobes
const byte scriptID=0;
*/
