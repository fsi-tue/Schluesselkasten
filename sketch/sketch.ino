#include <SPI.h>       //SPI 
#include <MFRC522.h>   //RFID Reader
#include "SevenSegmentTM1637.h"    // 7-Segment Display https://github.com/bremme/arduino-tm1637
#include <WiFiClientSecure.h> //SSL-Connection
#include <WiFiClient.h>       //Wifi-Connection
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//RIFD Reader
#define RST_PIN  05 //Pin D1
#define SS_PIN   04 //Pin D2
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

//DoorSwitch
#define SWITCH_PIN A0 //Pin A0

//Motor Pins
#define MOTOR_PIN1 3  //Pin RX
#define MOTOR_PIN2 15 //Pin D8
#define MOTOR_PIN3 2  //Pin D4
#define MOTOR_PIN4 0  //Pin D3
#define STEP_DELAY 3  //Minumum (stable) delay
const int maxSteps = 130; //Steps to make a quarter turn

//4-Digit LED Display
const int LED_CLK = 16; //Set the CLK pin connection to the display
const int LED_DIO = 05; //The DIO Pin
SevenSegmentTM1637 display(LED_CLK, LED_DIO); //set up the 4-Digit Display.

//Customize----------------------------------------------------------------------------------------------------------------------------
const int maxUser = 100; //Maxminum amount of Users
const char *masterUID = "12ab12ab"; //UID of the Master Card, this card is able to open the door even without an Internet connection
const char *debugUID_left = "23bc23bc"; //UID of the Debug Card, Stepper motor will turn 30 steps cw (in case of Jamming)
const char *debugUID_right = "34cd34bc"; //UID of the Debug Card, Stepper motor will turn 30 steps ccw (in case of Jamming)
const int debugSteps = 30; //Number of steps Motor will turn while using a Debug Card
const char *ssid = "WIFI-SSID";  //WIFI SSID
const char *password = "WIFI-password"; //WPA/PSK password
const char *host = "sub.domain.tld"; //Host of the API
const int port = 443; //Port of the API Host
const char *param = "/dir/to/api.php";
const char *apikey = "apiKey";
const char fingerprint[] PROGMEM = "AA 00 BB 11 CC 22 DD 33 EE 44 FF 66 77 88 00 AA 11 BB 22 CC"; //SHA1 Fingerprint of SSL Certificate
const int maxWifiRetry = 40; // Maximum number of steps for successful wifi connection (each step 500ms) 
const int maxFetchRetry = 80; // Maximum number of attempts to contact the API
const bool fetchIfUnknownCard = false; //Refresh List of Allowed UID if Unknown Card was read. (Slow)
//End Customize------------------------------------------------------------------------------------------------------------------------


String allowedUID[maxUser];
int totalUser = -1;
// Init array that will store new NUID
byte nuidPICC[4];
String currentUID;

//State 0=boot, 1=wifi, 2=fetch,3=ready
//closed, is door closed (does not say anything about the locking state)
int state = 0;
boolean closed = true;
int dispNum = 0;

//SSL-Client
WiFiClientSecure httpsClient;

//#####################################################
void setup() {
  delay(500);
  Serial.begin(115200); // start serial connection to print out debug messages and data
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  //Setting up display
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.print(1000);
//Print license
Serial.println("--------------------------------------------------------------------------------------------------");
  Serial.println("Schluesselkasten for the Computer-Science Student Union of the Univertity of Tuebingen");
  Serial.println("Copyright (C) 2019  Jules Kreuer / not-a-feature");
  Serial.println();
  Serial.println("This program is free software: you can redistribute it and/or modify");
  Serial.println("it under the terms of the GNU General Public License as published by");
  Serial.println("the Free Software Foundation, either version 3 of the License, or");
  Serial.println("(at your option) any later version.");
  Serial.println();
  Serial.println("This program is distributed in the hope that it will be useful,");
  Serial.println("but WITHOUT ANY WARRANTY; without even the implied warranty of");
  Serial.println("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
  Serial.println("See https://github.com/not-a-feature/Schluesselkasten/blob/master/LICENSE for more. details.");
Serial.println("--------------------------------------------------------------------------------------------------");

 

  //Motor Pins
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_PIN3, OUTPUT);
  pinMode(MOTOR_PIN4, OUTPUT);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  connectWifi();

  Serial.println("boot complete");
  fetchTeri();
  disableWifi();
  printallowedUID();
  doorStatus();
  display.print(2000);
}

void loop() {
  //If ready
  if (state == 3 && closed == true) {
    // Look for new cards
    if ( ! rfid.PICC_IsNewCardPresent())
      return;

    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
      return;

    //Read UID
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    //Convert UID to Hex
    currentUID = convertHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.print(currentUID);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    if (currentUID == debugUID_right) {
      display.print("D-R");
      for (int i = 0; i < debugSteps; i++) {
        step_right();
      }
      reset_pins();
      display.clear();
      display.print("OK");
    }
    else if (currentUID == debugUID_left) {
      display.print("D-L");
      for (int i = 0; i < debugSteps; i++) {
        step_left();
      }
      reset_pins();
      display.clear();
      display.print("OK");
    }
    else if (checkUser()) {
      display.print(3001);
      Serial.print(" is allowed");
      Serial.println("");
      openDoor(maxSteps);
      display.print("OPEN");
      delay(5000);
      doorStatus();
      if (closed) {
        closeDoor();
      display.clear();
      display.print("OK");
      }
    }
    else {
      display.print(3002);
      Serial.print(" is not allowed");
      Serial.println("");
      if (fetchIfUnknownCard) {
        Serial.println("Fetching new Cards");
        state = 5;
        die(true);
      }
      delay(750);
    }
  }
  else if (state == 3 && closed == false) {
    display.print(2003);
    while (!closed) {
      doorStatus();
      delay(400);
    }
    closeDoor();
      display.clear();
      display.print("OK");
  }
  
  else {
    die(false);
  }

}


//###############################################
//
void die(bool quick) {
      Serial.print("Dying State: ");
      Serial.println(state);
      display.print(4000 + state);
      reset_pins();
      disableWifi();
      
      if(!quick) {
        delay(5000);
      }
      
      display.clear();
      display.off();
      ESP.restart();
}
//check if UID in allowedUID array
boolean checkUser() {
  if (currentUID == masterUID) {
    return true;
  }
  for (int i = 0; i < maxUser; i++) {
    if (currentUID == allowedUID[i]) {
      return true;
    }
  }
  return false;
}

void printallowedUID() {
  Serial.println("----------------------");
  Serial.println("allowed UID");
  for (int i = 0; i < maxUser; i++) {
    if (allowedUID[i] != "") {
      Serial.println(allowedUID[i]);
    }
  }
  Serial.println("----------------------");
}


String convertHex(byte *buffer, byte bufferSize) {
  String currentUID;
  for (byte j = 0; j < bufferSize; j++)
      {
        currentUID += String(buffer[j] < 0x10 ? "0" : "");
        currentUID += String(buffer[j], HEX);
      }
      currentUID.toLowerCase();
      return currentUID;
}
