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
    char buffer[512];//multiple of 32 //message to send, will be splitted in smaller 32 bytes packets
    char message[32];//message to be sent
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

  void AddToBuffer(char data[]){
    strcat(buffer, data);
  }

  void AddNValuesToBuffer(char prefix[],int n,float array[]){
    strcat(buffer,prefix);
    for(int i=0;i<n;i++){
      strcat(buffer,";");
      char temp[32];//overkill//let's arbitrarily say the float, at worst case, will fit in 32 characters (-9999999999999999999999999999.99)//if smaller, the produced cstring will be finished by \0 so not all 32 bits are used
      int nbDecimal=2;
      dtostrf(array[i], 0, (array[i]-(int)array[i])==0?0:nbDecimal, temp);//https://www.programmingelectronics.com/dtostrf/ //send an int instead of float if the float decimals are only 0's
      //Serial.print("float:");Serial.print(array[i],2);Serial.print("\tstr:");Serial.println(temp);//debug float display
      strcat(buffer,temp);
    }
    strcat(buffer,"\n");
  }

  void SendBuffer(){//splits buffer in packet then send them
    strcat(buffer,"\x4");//0x4 is "end of transmission" ASCII character //this one'' //"" are important, not''
    Serial.print("buffer:\n");
    Serial.println(buffer);
    int readableMessageSize=sizeof(message)-1;//we need to let place for the terminating '\0' //you can remove n chars by substracting n(and use bytes after '\0' to add non cstring metadata)
    int nbLoops=(strlen(buffer)/readableMessageSize)+(strlen(buffer)%readableMessageSize!=0);//calculate the number of loops needed to send the whole message
    for(int i=0;i<nbLoops;i++){
      strncpy(message, buffer+i*readableMessageSize, readableMessageSize);//buffer is a pointer,e.g. by adding an offset of 32 bits we skip and get the next 32 characters of the array
      radio.write(&message,sizeof(message));//32 bytes is the max NRF24L01 max message size
    }
    ClearBuffer();
  }

  void ClearBuffer(){
    strcpy(buffer, "");//clears buffer
  }
};