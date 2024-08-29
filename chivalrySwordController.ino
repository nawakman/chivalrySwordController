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

//USED FOR TESTING NOT IN THE FINAL CODE
/*#include "joystick.h"
#include "ads1115ToJoystick.h"
#include "movingAverage.h"
Joystick j(1,0,3);//do not use digital pin2, it is already used as mpu6050 interrupt pin
ADS1115ToJoystick atj;
MovingAverage ma;//change num Readings in the class file*/

CustomNRF24L01 radio(7,8,false);
CustomMPU6050 mpu1(0x68);//sword mpu
CustomMPU6050 mpu2(0x69);//head mpu
CustomADS1115 ads1(0x48);//sword ads
CustomADS1115 ads2(0x49);//joystick ads
CustomNunchuk nchk;
long int lastUpdate=0;

void setup() {
  Serial.begin(115200);
  radio.Setup();
  mpu1.Setup();
  mpu2.Setup();
  ads1.Setup();
  //ads2.Setup();
  nchk.Setup();
}

void loop() {
  
  /*
  Serial.print("joy\t");
  Serial.print(j.GetX());
  Serial.print("\t");
  Serial.print(j.GetY());
  Serial.print("\t");
  Serial.println(j.ButtonDown());
  */
  
  float acc1[3];
  mpu1.GetRealAccel(acc1);
  radio.SendNValues("acc1",3,acc1);
  PrintNVal("acc1", 3, acc1);

  float ypr1[3];
  mpu1.GetYawPitchRoll(ypr1);
  radio.SendNValues("ypr1",3,ypr1);
  PrintNVal("ypr1", 3, ypr1);
  
  float ypr2[3];
  mpu2.GetYawPitchRoll(ypr2);
  radio.SendNValues("ypr2",3,ypr2);
  PrintNVal("ypr2", 3, ypr2);
  
  

 /* 
  uint16_t ads1Values[4];
  ads1.ReadAllValues(ads1Values);
  PrintNVal("ads1",4,ads1Values);
  */

  /*
  uint16_t ads2Values[4];
  ads2.ReadAllValues(ads2Values);
  //PrintNVal("ads2",4,ads2Values);
  Serial.print("joy\t");
  Serial.print(atj.ToJoyValue(ads2Values[0]));
  Serial.print("\t");
  Serial.print(atj.ToJoyValue(ads2Values[1]));
  Serial.print("\t");
  Serial.println(atj.ButtonDown(ads2Values[2]));
  */

  float nchkState[9];
  if(nchk.GetNunchuckState(nchkState)){
    radio.SendNValues("nchk1",3,nchkState);
    radio.SendNValues("nchk2",3,6,nchkState);
    radio.SendNValues("nchk3",6,9,nchkState);
    PrintNVal("nchk",9,nchkState);
  }

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

