/*
-max pwm frequency without overloading the interrupt
-smooth fade - the fade is way too fast at the start of the fade
  -need more brightness levels but the softPWM only wants to do 256 - I have the new color values in bytes also now so they need to be changed to int if it is increased
  -the lowest brightness 1/3? of the fade will be at a slower speed and the fade determiner needs to be adjusted to account for this so that it will still reach the target
  -there needs to be a modifier on the fade speed that is 1 at the midpoint, less than 1 by a configurable percent before the midpoint of the fade and greater than 1 by the same amount after the midpoint of the fade, this way the fade will end up taking the same amount of time to get to the target 
  -if I can get a smooth transition to 0 then make 0 the minimum value instead of 1 like I have now
-make it millis() overflow friendly - long will hold millis up to 24 days but for the lights it needs to go forever.
-find compatible video frequency
-spectrum limitation - 2d array (Ra,Ga,Ba,Rb,Gb,Bb), each row defines a range of colors, the newColor() function randomly picks a row of this array(to disable have only one row with the full range) and limits the random colors picked to that range - this allows me to give a custom palette to the piece
  -the values will be used as a ratio of r:g:b and don't denote brightness
  -random range within that range that changes on every powerup.
-consecutive leds only in positions - it must choose a start led and number of leds and then create the position from the available leds consecutive to that start led
  -positions as a range rather than an array of leds. if the max value is less than the min value then that means it crosses the zero point of the circle
-script setup step instead of setting variables over and over
-minimum total number of RGBLEDs in positions - this would make it more consistently bright
-programFinish() function that runs the if(millis()>= program[0][1] && millis()>= program[3][1]){ thing? - it's on every script component so this would make it easier to change but I don't know if it's better
-combine nested if statements into one line wherever possible

-scripts - I can fill in extra memory with these and put different ones on each piece
  -rainbow chase - the leds crossfade in a circle while color shifting through the spectrum
  -catscradle - the light jumps back and forth across the board to the opposite LED while rotating around the circle
  -interlaced crossfade - every other led around the circle lights and crossfades to the others
  -SOS morse code + 420, 710, per, not fade away
  -heartbeat
  -most on and randomly turn off leds(random5Ledonoff with fade instead of on/off)
  -ministrobe - 3 different RGBs quickly turned full on and off in rapid series
  -moving position strobe where each strobe pulse is different random consecutive RGBLEDs - this will be less bright

-bugs:
  -fadeUpBack locks up
  -allShiftRandom locks up
  -brightness fluctuations
  -standard doesn't appear to be doing the colorShift, only crossfade
  -it almost looks like it bounces down below the offLevel, maybe make a safeguard so that the fader can never go below offLevel or over 255
  
-maybe
  -create a stripped down version of the softPWM library and see if it will change memory usage
    -the printInterupt serial output in the softPWM library is way too verbose and uses a lot of memory, cut it down and I think it used a floating number for the interupt readout, it seems like it could be a percentage instead
  -i could split the program array into 2 different arrays, one is target and current brightness(byte), the other is end and last brightness change time(long)
  -underclocking: I believe it would use less power at 8MHz if I can get it to still run at 5V but the pwm might need the faster speed
  -use internal thermometer(secret thermometer library?) to have a safety cutoff if it gets too hot
  -rotary selector input
    -bonus tracks - using the rotary encoder you can access these events by rotating way past the fastest setting(or just have it as an option in the push button menu and then turn to control speed) and then you can select the randomly triggered event to run continuously
    -interrupt system?
    -write value to the EPROM using the library so that it will be the same even when turned off - it only is rated for 100000 cycles so it should only write every certain amount of time and only if it has changed - research what the behavior will be after it is worn out and make the program still work
    -push toggles between standard, fixed color(rotary selects color), no movement all leds synched color shift, randomly triggered events/bonus tracks with rotary selector cycling through the options

-Program strip down - coment out:
  all unused script setup variables
  all unused scripts - check to see if unused functions increase the size
  all unused script components
  softPWM library
*/
