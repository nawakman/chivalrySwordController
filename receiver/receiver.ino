//https://www.youtube.com/watch?v=7rcVeFFHcFM

#include <RF24.h>
#include <nRF24L01.h>

//long int lastUpdate;

RF24 radio(7,8);//CNS,CE
const byte adress[6]="00001";
//const byte adress[6]={'0','0','0','0','1','0'}; //also works
char text[32];
char textCopy[32];//will be shredded into pieces, reduced to ashes by the mighty strtok function

char nchk[9][8];

void setup(){
  //lastUpdate = millis();
  Serial.begin(115200);
  InitRadio();
  Serial.println("waiting for radio message...");
}

void loop(){
  if(radio.available()){
    //lastUpdate = millis();
    radio.read(&text,sizeof(text));
    /*Serial.print("message:\t");
    Serial.println(text);*/
    strcpy(textCopy, text);
    ProcessRadioData();
  }/*else{
    radio.flush_rx();
  }*/
    /*else if((millis()-lastUpdate)>2000){//did not receive data for over 500ms
    Serial.println("did not receive data for over 500ms, restarting radio module");
    radio.powerDown();//https://arduino.stackexchange.com/questions/55042/how-to-automatically-reset-the-nrf24l01-with-code
    InitRadio();
    radio.powerUp();
    delay(2000);
  }*/
  
  //no delay, we process received data as soon as we receive it
}

void InitRadio(){
  radio.begin();
  radio.setChannel(0);
  radio.openReadingPipe(0,adress);
  radio.setPALevel(RF24_PA_MIN);//set according to distance between modules
  radio.startListening();
}

void ProcessRadioData(){
  char* pointer=strtok(textCopy, "\t");//https://forum.arduino.cc/t/char-array-splitting-with-strtok/633418/3
  
  if (strcmp(pointer,"nchk1")==0){
    for(int i=0;i<3;i++){
      pointer=strtok(NULL, "\t");
      strcpy(nchk[i],pointer);
    }
  }
  else if (strcmp(pointer,"nchk2")==0){
    for(int i=3;i<6;i++){
      pointer=strtok(NULL, "\t");
      strcpy(nchk[i],pointer);
    }
  }
  else if (strcmp(pointer,"nchk3")==0){//we received all nunchuk related data, let's concatenate the whole thing to a single string to display it
    for(int i=6;i<9;i++){
      pointer=strtok(NULL, "\t");
      strcpy(nchk[i],pointer);
    }
    Serial.print("nchk");
    for(int i=0;i<9;i++){
      Serial.print("\t");
      Serial.print(nchk[i]);
    }
    Serial.println();
  }
  else{
    Serial.println(text);
  }
}
