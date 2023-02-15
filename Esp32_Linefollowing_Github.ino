#include <Wire.h>  // includes library for I2C communication
// Define the sensor pins using their ESP32 GPIO numbers
#define sensor1 2 //Green wire
#define sensor2 0 //pink wire
#define sensor3 4 //orange wire
#define sensor4 14 //white wire
#define sensor5 27 //yellow wire
#define sensor6 26 //purple wire


// Define variables
float servoangle,difference1,difference2,inter,dif;
float temper=0;
int temp, DR, NDR; 
int counter;
int datasend;
void setup() {
  Serial.begin(9600);  // begin serial communication
  while(!Serial) {}    // wait for Serial to become ready
  
  // Start the I2C bus 
  Wire.begin();
  Wire.setClock(400000);
  // I2C bus, 0x68 address 
  counter = 0;
}

void loop() {
  float a = analogRead(sensor1);  // read analog value from sensor1
  float b = analogRead(sensor2);  // read analog value from sensor2
  float c = analogRead(sensor3);  // read analog value from sensor3
  float d = analogRead(sensor4);  // read analog value from sensor4
  float e = analogRead(sensor5);  // read analog value from sensor5
  float f = analogRead(sensor6);  // read analog value from sensor6

  // calculate the difference between the sensors' values
  difference1 = (-(15*(4090-a)/3295)-(10*(4090-b)/1295)-(5*(4090-c)/1395)+(5*(4090-d)/1295)+(10*(4090-e)/1295)+(15*(4090-f)/2395));

  delay(1);  // add a delay of 1 millisecond

  a = analogRead(sensor1);  // read analog value from sensor1 again
  b = analogRead(sensor2);  // read analog value from sensor2 again
  c = analogRead(sensor3);  // read analog value from sensor3 again
  d = analogRead(sensor4);  // read analog value from sensor4 again
  e = analogRead(sensor5);  // read analog value from sensor5 again
  f = analogRead(sensor6);  // read analog value from sensor6 again

  // calculate the difference between the sensors' values again
  difference2 = (-(15*(4090-a)/3000)-(10*(4090-b)/1180)-(5*(4090-c)/1270)+(5*(4090-d)/1180)+(10*(4090-e)/1180)+(15*(4090-f)/2180));

  inter = inter + 0.5*(difference1+difference2)*0.001;  // calculate the new integration value
  dif = (difference2-difference1)/0.001;  // calculate the gradient
  servoangle = 3.2*(10-difference2) + 0.32*inter + 0.02*dif;  // sum the difference, integration, and gradient

  if(DR != NDR)
  {
    DR = NDR;
    inter = 0;  // reset the integration value
  }

  servoangle = 3.2*(10-difference2) + 0.32*inter + 0.02*dif;  // sum the difference, integration, and gradient again

  if(servoangle<10)
{
  servoangle = 10;
    
}
if(servoangle > 110)
{
  servoangle = 110;  
}

datasend = servoangle;

// Check if all the sensors are reading max values
if(b>4080 && c>4080 && d>4080 && e>4080 )
{
  counter = counter + 1;
  temper = 0;  
  datasend = temp;
  delay(5);
}
else
{
  counter = 0;
  Serial.println(servoangle);
   temp = servoangle;
   if(temp<56)
   {
     NDR = 0;
   }
    if(temp>56)
   {
     NDR = 1;
   }
}

// If counter exceeds a threshold and temp is greater than 56, send data with value 240
if(counter>=40 && temp>56)
{
    Serial.println(servoangle);
  datasend = 240;
  sendData();
}

// If counter exceeds a threshold and temp is less than 56, send data with value 245
if(counter>=40 && temp<56)
{
    Serial.println(servoangle);
  datasend = 245;
  sendData();
}

// Print the value of servoangle to the serial monitor
Serial.println(servoangle);

// Send the value of datasend over the I2C bus
sendData();
}


void sendData() 
{

 Wire.beginTransmission(0x08);  // transmit to device #8
  Wire.write(datasend);          //sends speed value to arduino
   Wire.endTransmission();        // stop transmitting  
   
}
 
