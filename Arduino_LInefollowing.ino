#include <Wire.h> // include Wire library
#include <Servo.h>    //include the servo library
#define servoPin 4
Servo myservo;        // create servo object to control a servo
float currentSteer;  // variable to store the servo position
void moveSteering();
int getgyro();
int angle,motorcontrol,servo,servo2,y;



#define enA 5   //EnableA command line - should be a PWM pin
#define enB 6   //EnableB command line - should be a PWM pin



//name the motor control pins - replace the CHANGEME with your pin number, digital pins do not need the 'D' prefix whereas analogue pins need the 'A' prefix
#define INa A0  //Channel A direction 
#define INb A1  //Channel A direction 
#define INc A2  //Channel B direction 
#define INd A3  //Channel B direction 


void setup() {
  Serial.begin(9600); // start serial to visualise data
Wire.onReceive(recieveevent);
  analogWrite(enA, 220);
  analogWrite(enB, 220);
  // put your setup code here, to run once:
  Wire.begin(0x08); // join i2c bus with address 8


  myservo.attach(servoPin);  //attach our servo object to pin D4
  //the Servo library takes care of defining the PinMode declaration (libraries/Servo/src/avr/Servo.cpp line 240)

  //configure the motor control pins as outputs
  pinMode(INa, OUTPUT);
  pinMode(INb, OUTPUT);
  pinMode(INc, OUTPUT);
  pinMode(INd, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);   

  //initialise serial communication

  Serial.println("Arduino Nano is Running"); //Check if Arduino is running


   motors(); //make a call to the "motors" function and provide it with a value for each of the 2 motors - can be different for each motor - using same value here for expedience
  Serial.print("Motor Speeds: ");

}


void loop() {

 if (servo==240)
{ 
      
       myservo.write(0);
      goBackwards();    
}

 else if (servo==245)
{ 
      
       myservo.write(100);
      goBackwards();    
}

  else
  {
  moveSteering();
  goForwards();
 
  }

}





void motors() {
//   //set individual motor speed
//   //direction is set separately

  analogWrite(enA, 220);
  analogWrite(enB, 220);

}

 void moveSteering() 
 {


   //you may need to change the maximum and minimum servo angle to have the largest steering motion
    myservo.write(servo);   //tell servo to go to position in variable 'steeringAngle'
    //Serial.println("testing at steering");

}


// //for each of the below function, two of the 'IN' variables must be HIGH, and two LOW in order to move the wheels - use a trial and error approach to determine the correct combination for your EEEBot
void goForwards() 
{

  if(servo>56)
  {
  moveSteering();
  
   analogWrite(enA, 150);
  analogWrite(enB,100);

  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
  }

 if(servo<56)
  {
  moveSteering();
  
   analogWrite(enA, 100);
  analogWrite(enB,150);

  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
  }


}


void goBackwards() {

   analogWrite(enA, 130);
  analogWrite(enB, 130);

  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}

void stopMotors() {

  digitalWrite(INa, LOW);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, LOW);
}

void recieveevent()
{
while (Wire.available()) {    // loop whilst master sends data
  
    servo = Wire.read();       // receive servo value
    
     Serial.println(servo);
     //Serial.println("...");
     
  }  
}
