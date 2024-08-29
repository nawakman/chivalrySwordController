class Joystick{
  private:
    int xPin;
    int yPin;
    int buttonPin;
  
  public:
    Joystick(int _xPin,int _yPin,int _buttonPin){
      xPin=_xPin;
      yPin=_yPin;
      buttonPin=_buttonPin;
      pinMode(_buttonPin,INPUT_PULLUP);
      digitalWrite(_buttonPin, HIGH);
    }

    float GetX(){return float(map(analogRead(xPin),0,1023,-512,512))/512.0;}

    float GetY(){return float(map(analogRead(yPin),0,1023,-512,512))/512.0;}
    
    bool ButtonDown(){return !digitalRead(buttonPin);}//NOT because of pullup //https://www.locoduino.org/spip.php?article122
};