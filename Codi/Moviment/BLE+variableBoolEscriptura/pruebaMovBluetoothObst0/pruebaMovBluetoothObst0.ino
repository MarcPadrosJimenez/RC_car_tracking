/*ANDROID ARDUINO BLUETOOTH RC CAR  */
#include <ArduinoBLE.h>
//#include <Arduino_LSM6DS3.h> //IMU library

int outPin1 = 2;     //motor1
int outPin2 = 3;    //motor1

int outPin4 = 6;   //motor2
int outPin3 = 7;   //motor2

char bt = 0;       //BT 

bool obstacle = false;

BLEService moveService("1819"); // BLE Moving Service
BLECharCharacteristic moveCharacteristic("2A68", BLERead | BLEWrite);
BLEBoolCharacteristic obstacleCharacteristic("2A5D", BLEWrite);
/*----------------------------------------------------------*/

void setup()
{
  Serial.begin(9600);
  
  pinMode(outPin1,OUTPUT);
  pinMode(outPin2,OUTPUT);
  pinMode(outPin3,OUTPUT);
  pinMode(outPin4,OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(moveService);

  // add the characteristic to the service
  moveService.addCharacteristic(moveCharacteristic);
  moveService.addCharacteristic(obstacleCharacteristic);

  // add service
  BLE.addService(moveService);

  // set the initial value for the characteristic:
  moveCharacteristic.writeValue(0);
  obstacleCharacteristic.writeValue(obstacle);
  // start advertising
  BLE.advertise();


}
void loop()
{
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the car:
      if (moveCharacteristic.written() || obstacleCharacteristic.written()) {
        bt=moveCharacteristic.value();
        obstacle=obstacleCharacteristic.value();
        Serial.println(obstacle);
        Serial.println(bt);
        /*___________________________________________________*/

        if(obstacle && (bt == 'I' || bt == 'G' || bt == 'F')){
          bt='S';
        }
        
        if(bt == 'F')        //move forwards
        {
          digitalWrite(outPin1,HIGH);
          digitalWrite(outPin2,LOW);
          digitalWrite(outPin3,HIGH);
          digitalWrite(outPin4,LOW);
        }
        else if (bt == 'B')       //move backwards
        {
          digitalWrite(outPin1,LOW);
          digitalWrite(outPin2,HIGH);
          digitalWrite(outPin3,LOW);
          digitalWrite(outPin4,HIGH);
        }
        else if (bt == 'S')     //stop!!
        {   
          digitalWrite(outPin1,LOW);
          digitalWrite(outPin2,LOW);
          digitalWrite(outPin3,LOW);
          digitalWrite(outPin4,LOW);
        }
        else if (bt == 'R')    //right
        {
          digitalWrite(outPin1,HIGH);
          digitalWrite(outPin2,LOW);
          digitalWrite(outPin3,LOW);
          digitalWrite(outPin4,LOW);
        }
        else if (bt == 'L')     //left
        {
          digitalWrite(outPin1,LOW);
          digitalWrite(outPin2,LOW);
          digitalWrite(outPin3,HIGH);
          digitalWrite(outPin4,LOW);
        }
        else if (bt == 'I')    //forward right
        {
          digitalWrite(outPin1,HIGH);
          digitalWrite(outPin2,LOW);
          digitalWrite(outPin3,LOW);
          digitalWrite(outPin4,HIGH);
        }
        else if (bt == 'G')    //forward left
        {
          digitalWrite(outPin1,LOW);
          digitalWrite(outPin2,HIGH);
          digitalWrite(outPin3,HIGH);
          digitalWrite(outPin4,LOW);
        }//arduino//
      }
    }
  }
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
  digitalWrite(LED_BUILTIN, LOW);
 
}
  /*----------- E N  D -----------*/
