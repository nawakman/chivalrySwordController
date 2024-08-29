#include "HardwareSerial.h"
#include "src/Nunchuk/Nunchuk.h"//from https://github.com/infusion/Fritzing/tree/master/Nunchuk //because wiiChuck library is tto large

class CustomNunchuk{
  private:
    

  public:
    Setup(){
      nunchuk_init();
    }

    bool GetNunchuckState(float array[]){//return true if it worked //if nunchuck is disconnected once, the arduino must be reset
      if(nunchuk_read()){//program might stall here, in my case it was because the nunchuck was faulty
        array[0]=float(nunchuk_joystickX())/100;
        array[1]=float(nunchuk_joystickY())/100;
        array[2]=nunchuk_accelX();
        array[3]=nunchuk_accelY();
        array[4]=nunchuk_accelZ();
        array[5]=nunchuk_pitch()*RAD_TO_DEG;
        array[6]=nunchuk_roll()*RAD_TO_DEG;
        array[7]=nunchuk_buttonZ();
        array[8]=nunchuk_buttonC();
        return true;
      }else{
        Serial.println("cannot read nunchuck, is it plugged ?");
        return false;
      }
    }
};