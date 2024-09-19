//https://www.youtube.com/watch?v=7rcVeFFHcFM

#include <RF24.h>
#include <nRF24L01.h>

//long int lastUpdate;

RF24 radio(7,8);//CNS,CE
const byte adress[6]="00001";
//const byte adress[6]={'0','0','0','0','1','0'}; //also works
char text[32];
char buffer[512];//multiple of 32, must be the same as the one in emitter code

void setup(){
  //lastUpdate = millis();
  Serial.begin(115200);
  InitRadio();
  Serial.println("waiting for radio message...");
}

void loop(){
  if(radio.available()){
    radio.read(&text,sizeof(text));
    /*Serial.println("vvvvvvvvvvvvvv");
    Serial.println(text);
    Serial.println("^^^^^^^^^^^^^^");*/
    strcat(buffer,text);//a string is received in chunks //we put the chunks together
    if(strchr(text, '\x4')!=NULL){//'' are important, not"" //if EOT char found in this message we can stop receiving and start processing gathered info //https://cplusplus.com/reference/cstring/strchr/
      Serial.println(buffer);
      strcpy(buffer,"");//clears buffer
    }  
  }
  //no delay, we process received data as soon as we receive it
}

void InitRadio(){
  radio.begin();
  radio.setChannel(0);
  radio.openReadingPipe(0,adress);
  radio.setPALevel(RF24_PA_MIN);//set according to distance between modules
  radio.startListening();
}

