//I need to figure out how to make these constants and I could just comment out all but the one I want and skip the parameter to save program space
void profile(byte profileID){
  switch(profileID){
    case 0:
      fadeDelayMaxSet = 18000;  //the longest time that it will color shift.
      fadeDelayMinSet = 5000;  //shortest shift time
      fadeDelayDiffMin = 8000;  //the minimum difference in the range of fade speeds
      valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
      valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
      //valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
      //valueDiffMinMaxSet=15;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
      strobeTime=100;  //the length of time the strobe is on
      strobeDelayMin=500;  //the minimum time between strobes
      strobeLikelyhood=4;  //the likelyhood of a strobe happening(1 in n, higher number=less likely to happen)
      scriptID=0;
    break;
    case 1:
      fadeDelayMaxSet = 5000;  //the longest time that it will color shift.
      fadeDelayMinSet = 500;  //shortest shift time
      fadeDelayDiffMin = 800;  //the minimum difference in the range of fade speeds
      valueTotalMinMinSet = 35; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
      valueTotalMinMaxSet = 70; //minimum sum of percentages of the RGB diodes of an LED. This avoids it picking too dim colors.
      //valueDiffMinMinSet=15;  //the minimum bound of the randomly selected minimum change in value of each color. not yet implemented
      //valueDiffMinMaxSet=15;  //the maximum bound of the randomly selected minimum change in value of each color. not yet implemented
      strobeTime=30;  //the length of time the strobe is on
      strobeDelayMin=500;  //the minimum time between strobes
      strobeLikelyhood=4;  //the likelyhood of a strobe happening(1 in n, higher number=less likely to happen)
      scriptID=1;
    break;
  }
}
