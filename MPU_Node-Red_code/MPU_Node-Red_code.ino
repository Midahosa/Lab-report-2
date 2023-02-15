#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
MPU6050 mpu6050(Wire);
// --

// Replace the next variables with your SSID/Password combination
const char* ssid = "";                      //CHANGE ME
const char* password = "";              //CHANGE ME    

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";                      
const char* mqtt_server = "";          //CHANGE ME

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


float x=0;
float y=0;
float z=0;
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
  // --
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
     
      client.subscribe("esp32/output");
       
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;

    int xaxis = mpu6050.getAngleX();
    int yaxis = mpu6050.getAngleY();
    int zaxis = mpu6050.getAngleZ();
   
    mpu6050.update();
   
    char xString[8];
    dtostrf(xaxis, 1, 1, xString);
    Serial.print("x axis=");    
    Serial.println(xaxis);
    client.publish("esp32/x-axis", xString);

    char yString[8];
    dtostrf(yaxis, 1, 1, yString);
    Serial.print("y axis=");
    Serial.println(yaxis);
    client.publish("esp32/y-axis", yString);

    char zString[8];
    dtostrf(zaxis, 1, 1, zString);
    Serial.print("z axis=");
    Serial.println(zaxis);
    client.publish("esp32/z-axis", zString);
   
  }
}
