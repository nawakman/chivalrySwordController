#include <stdint.h>
#include "Arduino.h"
#include "ADS1115-Driver.h"

class CustomADS1115{
  private:
    ADS1115 ads1115;
  
  public:
    CustomADS1115(int adress):ads1115(adress){}

    void Setup() {
	    ads1115.reset();
	    ads1115.setDeviceMode(ADS1115_MODE_SINGLE);
	    ads1115.setDataRate(ADS1115_DR_128_SPS);
	    ads1115.setPga(ADS1115_PGA_4_096);//https://github.com/Wh1teRabbitHU/ADS1115-Driver/blob/master/src/ADS1115-Driver.h
      //gains are 2/3,1,2,4,8,16 //https://forums.adafruit.com/viewtopic.php?t=131714 //correspond to the PGA settings of the above line
    }

    uint16_t ReadValue(uint8_t input) {
	    ads1115.setMultiplexer(input);
	    ads1115.startSingleConvertion();
	    delayMicroseconds(25); // The ADS1115 needs to wake up from sleep mode and usually it takes 25 uS to do that
	    while (ads1115.getOperationalStatus() == 0);
	    return ads1115.readConvertedValue();
  }

    void ReadAllValues(uint16_t array[]){
      array[0]=ReadValue(ADS1115_MUX_AIN0_GND);
      array[1]=ReadValue(ADS1115_MUX_AIN1_GND);
      array[2]=ReadValue(ADS1115_MUX_AIN2_GND);
      array[3]=ReadValue(ADS1115_MUX_AIN3_GND);
    }
};