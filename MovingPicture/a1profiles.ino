//it is named aiprofiles so that it will sort under the top tab and be concatenated in the proper order
//I could get rid of this and just pass the parameters to the scripts in the script function and do the comment out thing there
//uncomment the settings you want and leave the rest commented out

//test profile
const int fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
const int fadeDelayMinSet = 5000;  //shortest shift time
const int fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
const byte strobeTimeMax=20;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const byte strobeTimeMin=5;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const int strobeDelayMin=500;  //the minimum time between strobes
const int strobeDelayMax=200;  //the minimum time between strobes
const byte scriptID=0;

/*
//standard shift/crossfade
const int fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
const int fadeDelayMinSet = 5000;  //shortest shift time
const int fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
const int valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
const int valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
//const int valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
//const int valueDiffMinMaxSet=45;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
const byte strobeTimeMax=20;  //the maximum length of time the strobe is on - I don't think it needs powerup randomization
const byte strobeTimeMin=5;  //the minimum length of time the strobe is on - I don't think it needs powerup randomization
const int strobeDelayMin=500;  //the minimum time between strobes
const int strobeDelayMax=200;  //the minimum time between strobes
const byte scriptID=0;
*/
