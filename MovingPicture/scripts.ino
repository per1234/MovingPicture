//if program space gets tight I can just comment out all but the one I want and skip the parameter
void script(byte scriptID){
  if(scriptID==0){
    standard();
  }
  if(scriptID==1){
    standard();
    strobe();
  }
}
