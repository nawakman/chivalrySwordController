#include "HardwareSerial.h"
#include <stdint.h>
class ADS1115ToJoystick{
  public:
    ADS1115ToJoystick(){}

    float ToJoyValue(uint16_t value){return float(map(value,0,4095,-2048,2048))/2048;}//4095 is ADS1115 max value I get on the "ADS1115_PGA_4_096" programmable gain amplifier setting
    
    bool ButtonDown(uint16_t value){return value==8191;}
};