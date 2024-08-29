#include <math.h>
#include <stdlib.h>
#include "HardwareSerial.h"
#include <string.h>
//https://www.youtube.com/watch?v=7rcVeFFHcFM

#include <RF24.h>
#include <nRF24L01.h>

class CustomNRF24L01{
  private:
    RF24 radio;//CNS,CE
    bool isReceiver;
    const byte adress[6]={'0','0','0','0','1'};
    const char text[32];//message to be sent
    //const byte adress[6]="00001";//stopped working when moved into this class

  public:
  CustomNRF24L01(int CNS, int CE, bool _isReceiver):radio(CNS,CE),isReceiver(_isReceiver){}//https://arduino.stackexchange.com/questions/52004/creating-public-instance-of-class-in-another-classs-initialization

  void Setup(){
    radio.begin();
    radio.setChannel(0);
    radio.setPALevel(RF24_PA_MIN);//set according to distance between modules
    if(isReceiver){
      radio.openReadingPipe(0,adress);
      radio.startListening();
    }else{
      radio.openWritingPipe(adress);
      radio.stopListening();
    }
  }

  void SendMessage(char array[]){//32 characters max,this is a NRF24L01 hardware restriction
    if(strlen(array)>32){
      Serial.println("MESSAGE SIZE OVER 32 CHARACTERS, MESSAGE NOT SENT");
    }else{
      strcpy(text, array);//for some reason directly sending "array" does not work so we pass through "text"
      radio.write(&text,strlen(text));
    }
  }

  void SendNValues(char prefix[],int n,float array[]){
    strcpy(text, prefix);//clears and start filing char array content, else it will concatenate new data and overflow
    for(int i=0;i<n;i++){
      strcat(text,"\t");
      char temp[32];//overkill//let's arbitrarily say the float, at worst case, will fit in 32 characters (-9999999999999999999999999999.99)//if smaller, the produced cstring will be finished by \0 so not all 32 bits are used
      int nbDecimal=2;
      dtostrf(array[i], 0, (array[i]-(int)array[i])==0?0:nbDecimal, temp);//https://www.programmingelectronics.com/dtostrf/ //send an int instead of float if the float decimals are only 0's
      strcat(text,temp);
    }
    Serial.print("sent values chararray size in bytes:\t");
    Serial.println(strlen(text));
    Serial.println(text);
    radio.write(&text,sizeof(text));//32 bytes is the max NRF24L01 max message size
  }

  void SendNValues(char prefix[],int startIndex,int endIndex,float array[]){//overload to split data in chunks to reduce message size and not exceed 32bytes
    strcpy(text, prefix);//clears and start filing char array content, else it will concatenate new data and overflow
    for(int i=startIndex;i<endIndex;i++){
      strcat(text,"\t");
      char temp[32];//overkill//let's arbitrarily say the float, at worst case, will fit in 32 characters (-9999999999999999999999999999.99)//if smaller, the produced cstring will be finished by \0 so not all 32 bits are used
      int nbDecimal=2;
      dtostrf(array[i], 0, (array[i]-(int)array[i])==0?0:nbDecimal, temp);//https://www.programmingelectronics.com/dtostrf/ //send an int instead of float if the float decimals are only 0's
      strcat(text,temp);
    }
    Serial.print("sent values chararray size in bytes:\t");
    Serial.println(strlen(text));
    //Serial.println(text);
    radio.write(&text,sizeof(text));//32 bytes is the max NRF24L01 max message size
  }


  void ReceiveMessage(){
    if(radio.available()){
    char text[32]="";
    radio.read(&text,sizeof(text));
    Serial.println(text);
    }
  }
};