#include "customNRF24L01.h"
#include "customMPU6050.h"
#include "customADS1115.h"
#include "customNunchuk.h"
/*libraries used:
-RF24 by TMRh20
-MPU6050 by Electronic Cats
-ADS1115-Driver by Wh1teRabbitHU
-Nunchuk by infusion (not available in library manager, this dependency is in the src folder)

Please make sure these libraries are installed for the project to work
*/

//#define DEBUG//comment this line in customNRF24L01.h, customMPU6050.h and customNunchuk.h too to disable Serial printing 

CustomNRF24L01 radio(7,8,false);
CustomMPU6050 mpu1(0x68);//sword mpu
CustomMPU6050 mpu2(0x69);//head mpu
CustomADS1115 ads1(0x48);//sword ads
CustomADS1115 ads2(0x49);//joystick ads
CustomNunchuk nchk;
long int lastUpdate=0;

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  radio.Setup();
  mpu1.Setup();
  mpu2.Setup();
  nchk.Setup();
  /*
  ads1.Setup();
  //ads2.Setup();
  */
}

void loop() {

  float acc1[3];
  float ypr1[3];
  mpu1.GetAccelAndYawPitchRoll(acc1,ypr1);
  radio.AddNValuesToBuffer("acc1",3,acc1);
  radio.AddNValuesToBuffer("ypr1",3,ypr1);
  //PrintNVal("acc1", 3, acc1);
  //PrintNVal("ypr1", 3, ypr1);

  
  float ypr2[3];
  mpu2.GetYawPitchRoll(ypr2);
  radio.AddNValuesToBuffer("ypr2",3,ypr2);
  //PrintNVal("ypr2", 3, ypr2);

  float nchkState[9];
  if(nchk.GetNunchuckState(nchkState)){
    radio.AddNValuesToBuffer("nchk",9,nchkState);
    //PrintNVal("nchk",9,nchkState);
  }
  
  radio.SendBuffer();

  while((millis()-lastUpdate)<8){//125Hz polling rate, same as xbox controller

  }
  lastUpdate=millis();
}

void PrintNVal(char prefix[],int n,float array[]){
  Serial.print(prefix);
  for(int i=0;i<n;i++){
    Serial.print("\t");
    Serial.print(array[i]);
  }
  Serial.println();
}

void PrintNVal(char prefix[],int n,uint16_t array[]){
  Serial.print(prefix);
  for(int i=0;i<n;i++){
    Serial.print("\t");
    Serial.print(array[i]);
  }
  Serial.println();
}

