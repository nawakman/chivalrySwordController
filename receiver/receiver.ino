//https://www.youtube.com/watch?v=7rcVeFFHcFM

#include <RF24.h>
#include <nRF24L01.h>

//long int lastUpdate;

RF24 radio(7,8);//CNS,CE
const byte adress[6]="00001";
//const byte adress[6]={'0','0','0','0','1','0'}; //also works
char message[32];
char buffer[512];//multiple of 32, must be the same as the one in emitter code

void setup(){
  //lastUpdate = millis();
  Serial.begin(115200);
  InitRadio();
  Serial.println("waiting for radio message...");
}

void loop(){
  /*Serial.print("radio available:");
  Serial.println(radio.available());*/
  if(radio.available()){//if available() evaluate to true, we know there won't be error in incoming packett
    radio.read(&message,sizeof(message));

    char* eotPointer=strchr(message, '\x4');//'' are important, not"" //if EOT char found in this message, remove it //https://cplusplus.com/reference/cstring/strchr/
    if(eotPointer!=NULL){
      *eotPointer='\0';//replace final eot with terminating '\0'
    }
    strcat(buffer,message);//a string is received in chunks //we put the chunks together

    if(eotPointer!=NULL){//if EOT char found in this message we can stop receiving and start processing gathered info
      Serial.println(buffer);//buffer without previously removed EOT character
      strcpy(buffer,"");//clears buffer
    }
  }
  //no delay, we process received data as soon as we receive it
}

void InitRadio(){
  radio.begin();
  radio.setChannel(0);
  radio.setPALevel(RF24_PA_MIN);//set according to distance between modules
  radio.openReadingPipe(0,adress);
  radio.startListening();
}

