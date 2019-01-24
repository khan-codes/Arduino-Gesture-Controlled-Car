#include <VirtualWire.h>

String got;

//MOTOR 01
int in1_01 = 7;
int in1_02 = 8;
int ena = 5;

int speed_straight = 150;
int speed_side = 75;
int FW_speed = 90;
int count = 0;

//MOTOR 02
int in2_01 = 13; 
int in2_02 = 12;
int ena_02 = 3;


void setup() 
{
  vw_setup(1000);
  vw_set_rx_pin(11);
  vw_rx_start();
  
  Serial.begin(9600);
}

void loop() 
{  
  uint8_t msg[4];
  uint8_t msg_size = 4;
  
  vw_wait_rx();

  bool msg_recv = vw_get_message(msg, &msg_size);
  
  //if(vw_get_message(msg, &msg_size))
  if(msg_recv == true)
  {
    got = String((char*) msg);

    //check();

    Serial.println(got);

    if(got == "fore")
    {
      Serial.println("We're going to North");
      if(FW_speed < 220)
      {
        FW_speed = FW_speed + 5;
      }
      else if(FW_speed >= 220)
      {
        FW_speed = 220;
      }

      digitalWrite(in1_01, HIGH);
      digitalWrite(in1_02, LOW);
      analogWrite(ena, speed_straight);
      //analogWrite(ena, FW_speed);
  
      digitalWrite(in2_01, HIGH);
      digitalWrite(in2_02, LOW);
      analogWrite(ena_02, speed_straight);
      //analogWrite(ena, FW_speed);
    }


    else if(got == "back")
    {
      Serial.println("We're going to South");
      //FW_speed = 90;

      digitalWrite(in1_01, LOW);
      digitalWrite(in1_02, HIGH);
      analogWrite(ena, speed_straight);
      
      digitalWrite(in2_01, LOW);
      digitalWrite(in2_02, HIGH);
      analogWrite(ena_02, speed_straight); 
    }


    else if(got == "righ")
    {
      //going towards right we are rotating the left tyre, because in this way it will go to the right (some newtonian physics shit)
      Serial.println("We're going to West");

      digitalWrite(in2_01, HIGH);
      digitalWrite(in2_02, LOW);
      analogWrite(ena_02, speed_side);
      
      digitalWrite(in1_01, LOW);
      digitalWrite(in1_02, LOW);
    }

    else if(got == "left")
    {
      //going towards left we are rotating the right tyre, because in this way it will go to the left (some newtonian physics shit)
      Serial.println("We're going to East");
      
      digitalWrite(in1_01, HIGH);
      digitalWrite(in1_02, LOW);
      analogWrite(ena, speed_side);
      
      digitalWrite(in2_01, LOW);
      digitalWrite(in2_02, LOW);      
    }


    else if(got == "stop")
    {
      Serial.println("Camp here!");
      //FW_speed = 90;

      digitalWrite(in1_01, LOW);
      digitalWrite(in1_02, LOW);
  
      digitalWrite(in2_01, LOW);
      digitalWrite(in2_02, LOW);
    }

    else if(got == "none")
    {
      Serial.println("Camp here!");
      //FW_speed = 90;

      digitalWrite(in1_01, LOW);
      digitalWrite(in1_02, LOW);
  
      digitalWrite(in2_01, LOW);
      digitalWrite(in2_02, LOW);
    }

    else
    {
      Serial.println("Camp here!");
      //FW_speed = 90;

      digitalWrite(in1_01, LOW);
      digitalWrite(in1_02, LOW);
  
      digitalWrite(in2_01, LOW);
      digitalWrite(in2_02, LOW);
    }

    got = "none"; 
    //delay(500);
  }

  else if(msg_recv == false)
  {
    Serial.println("Camp here!");

    digitalWrite(in1_01, LOW);
    digitalWrite(in1_02, LOW);
  
    digitalWrite(in2_01, LOW);
    digitalWrite(in2_02, LOW);

    //delay(500);
  }

  delay(100);        //changing it from delay(100);
  //count++;
  //count = count % 10;
  /*
  if(count == 0)          //at first it was if(count == 10)
  {
    digitalWrite(in1_01, LOW);
    digitalWrite(in1_02, LOW);
    
    digitalWrite(in2_01, LOW);
    digitalWrite(in2_02, LOW);
    count = 0;
    delay(10);
  }
  */
}

