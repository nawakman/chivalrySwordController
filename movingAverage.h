//https://docs.arduino.cc/built-in-examples/analog/Smoothing/
class MovingAverage{
  private:
    const int numReadings=8;
    float readings[8];  // the readings from the analog input //MUST EQUALS "numReadings"
    int readIndex = 0;          // the index of the current reading
    float total = 0;              // the running total
    float average = 0;

  public:
    MovingAverage(){
      for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      readings[thisReading] = 0;
      }
    }

    float AddPoint(float value){
      total = total - readings[readIndex];
      readings[readIndex] = value;// read from the sensor:
      total = total + readings[readIndex];// add the reading to the total:
      readIndex = readIndex + 1;// advance to the next position in the array:
      if (readIndex >= numReadings) {// if we're at the end of the array...
      readIndex = 0;// ...wrap around to the beginning:
      average = total / numReadings;// calculate the average:
      return average;
      }
    }
};

class ThreeMovingAverage{
  private:
    MovingAverage m1;
    MovingAverage m2;
    MovingAverage m3;
  
  public:

    void AddPoints(float valueArray[],float toArray[]){
      toArray[0]=m1.AddPoint(valueArray[0]);
      toArray[1]=m2.AddPoint(valueArray[1]);
      toArray[2]=m3.AddPoint(valueArray[2]);
    }
};