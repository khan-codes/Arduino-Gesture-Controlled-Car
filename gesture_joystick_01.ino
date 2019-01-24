#include <VirtualWire.h>
#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

String str_out;


//VARIABLES FOR BUTTON #01
int switchPin = 2;
int switch_value = 0;

bool last = LOW;
bool now = LOW;


//Correction for button #01
bool button_correct(bool last)
{
  bool curr = digitalRead(switchPin);

  if (last != curr)
  {
    delay(5);
    curr = digitalRead(switchPin);
  }
  return curr;
}

//Checking if button #01 is pressed or not
void is_button_pressed()
{
  now = button_correct(last);
  
  if (/*last == LOW &&*/ now == HIGH)
  {
    switch_value = now;
  }
}

void setup() 
{
  //pinMode(switchPin, INPUT);
  attachInterrupt(0, is_button_pressed, RISING);

  Serial.begin(115200);
  
  vw_setup(1000);
  vw_set_tx_pin(12);

  setupMPU();
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() 
{
  //Updating both button variables
  //AND checking wheather they are within the limits
  
  last = now;
  
  recordAccelRegisters();
  //printData();



  //    ********************                       Control         START !                *******************

  if (switch_value == 0)
  {
    check_MPU();
    Serial.println("MODE 1");
  }

  if (switch_value == 1)
  {
    Serial.println("MODE 2");
  }

  delay(500);
}



void setupMPU()
{
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x6B); 
  Wire.write(0b00000000); 
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x1B); 
  Wire.write(0x00000000); 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); 
  Wire.write(0b00000000); 
  Wire.endTransmission(); 
}

void recordAccelRegisters() 
{
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData()
{
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void printData() 
{
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}

void check_MPU()
{
  //FORWARD
  if(((gForceX > 0.0 &&  gForceX < 0.15) || (gForceX < 0.0 &&  gForceX > -0.15))  && gForceY > 0.15 &&  gForceY < 0.8)
  {
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH); 

    const char* msg = "fore";
  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    //delay(50);
  
  }

  //BACKWARD
  else if(((gForceX > 0.0 &&  gForceX < 0.15) || (gForceX < 0.0 &&  gForceX > -0.15))  && gForceY < -0.15 &&  gForceY > -0.8)
  {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);

    const char* msg = "back";  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
   
    //delay(50);
  }

  //RIGHT
  else if(gForceX < -0.15 && gForceX > -0.8  &&  ((gForceY > 0.0 &&  gForceY < 0.15) || (gForceY < 0.0 &&  gForceY > -0.15)))
  {
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    const char* msg = "righ";  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();

    //delay(50);
  }

  //LEFT
  else if(gForceX > 0.15 && gForceX < 0.8  &&  ((gForceY > 0.0 &&  gForceY < 0.15) || (gForceY < 0.0 &&  gForceY > -0.15)))
  {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);

    const char* msg = "left";  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    
    //delay(50);
  }

  //STATIONARY
  else if(gForceX >= 0.0 && gForceX <= 0.15  && gForceY >= 0.0 &&  gForceY <= 0.15    &&    gForceX <= 0.0 && gForceX >= -0.15  && gForceY <= 0.0 &&  gForceY >= -0.15)
  {
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    const char* msg = "stop";  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    
    //delay(50);
  }

  else
  {
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    const char* msg = "stop";  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    
    //delay(50);
  }
}
