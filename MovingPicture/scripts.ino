//if program space gets tight I can just comment out all but the one I want and skip the parameter
void script(byte scriptID){
  switch(scriptID){
    case 0:
      standard();
    break;
    case 1:
      standard();
      strobe();
    break;
    case 2:
      standard();
      fadeUpBack();
      allShiftRandom();
    break;
    case 3:  //woohoo!
      standard();
      fadeUpBack();
      allShiftRandom();
      strobe();
    break;
    case 4:
      allShift();
    break;
  }
}
