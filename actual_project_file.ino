#include <Servo.h>
#define motorPin 5
#define buttonPin 2
#define buzzerPin 7
#define sensorPin 11
#define ledPin 13
#define OFF 180
#define NEUTRAL 110
#define ON 20

Servo myservo;  // create servo object to control a servo
long start = 0, timer = 0;
boolean val = 0, pushed = 0, switchOn;

void setup(){
  //Serial.begin(9600); Serial monitoring for debugging
  myservo.attach(motorPin);  // attaches the servo on pin 9 to the servo object
  pinMode(buttonPin, INPUT); // sets the sensors to Arduino pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  myservo.write(ON); //turn the switch on so we know what state it's in
  delay(1000);
  switchOn = 1;
  myservo.write(NEUTRAL);
  delay(1000);
  myservo.detach(); // turn the servo off when not in use to save power
}
  
void loop (){
  //delay(1800000); //check every 30 minutes
  timer = millis();
  while(millis() - timer > 1800000){//wait for 30 mins but continually listen
    if(digitalRead(sensorPin)){//if there's noise
      if(switchOn){//if the switch was last turned on
         myservo.attach(motorPin); //turn on the motor again
         myservo.write(OFF); //turn the switch off
         delay(1000); //wait for the motor to move
         switchOn = 0; //keep track of the last move the motor made
         myservo.write(NEUTRAL);//go to the neutral position so the switch can be manually flipped
         delay(1000);
         myservo.detach(); // turn off he motor again
         return; //reset
      }
      else{
         myservo.attach(motorPin);
         myservo.write(ON);
         delay(1000);
         switchOn = 1;
         myservo.write(NEUTRAL);
         delay(1000);
         myservo.detach();
         return;
      }
    }
  }
  
  for(int x = 0; x < 3; x++){  //sound buzzer alarm 3 times
    //Serial.println("Buzz");  //debugging
    tone(buzzerPin, 2000, 500); //turn on the buzzer
    delay(1000); // wait a second
    noTone(buzzerPin); // turn off the buzzer
    delay(1000);
  }

  start = millis();//start the response timer
  pushed = 0;
  while(millis() - start < 30000){ //repeat this loop for 30 seconds
    int volume = digitalRead(sensorPin); //check if there's sound
    if(volume == HIGH){ // if there's sound over the threshold (clap)
      //digitalWrite(ledPin, HIGH); //Turn ON LED for debugging
      pushed = 1; //acknowledge a person's presence in the room
      //digitalWrite(ledPin, LOW); //Turn OFF LED
      break; //break out of the response loop after response
    }
    else if(digitalRead(buttonPin) == HIGH){ //if a person pressed the buzzer
      pushed = 1;
      break;
    }
  }

  if(pushed){ //if there was a user detected
    //Serial.println("returning"); // for debugging
    return; // reset
  }
  else{
    //Serial.println("moving"); // for debugging
    myservo.attach(motorPin);
    myservo.write(OFF); //turn off the light if no one is in the room
    delay(1000);
    myservo.write(NEUTRAL);
    delay(1000);
    myservo.detach();
    return; //reset
  } 
}
