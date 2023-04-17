/*
   RFID SECURED GESTURE CONTROLLED ROBOTIC ARM WITH RECORD AND PLAYBACK FEATURE
   Code by: Akash Shinde
   Dated: 10-04-2023
*/
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <Servo.h>  // add servo library

#define RST_PIN 9         
#define SS_PIN 10        
#define AUTHORIZED_TAG "33BE01A6" // Change this to your authorized tag's UID

SoftwareSerial sSerial(2, 3); // RX, TX
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
Servo myservo;

// Setup section to run once

  // attach the servo to our servo object

  //myservo.write(90); 

void setup() {
  sSerial.begin(9600);
  myservo.attach(3); 
  while (!sSerial) {
    ; // Wait for serial port to connect
  }

  Serial.begin(9600); // Initialize serial communications with the computer
  while (!Serial); // Wait for the serial port to connect

  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522 card reader

  Serial.println("RFID Reader Initialized");

}

void loop() {
  myservo.write(180);
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      tagUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      tagUID += String(mfrc522.uid.uidByte[i], HEX);
    tagUID.toUpperCase();
    }
    
    if (tagUID == AUTHORIZED_TAG) {
      Serial.println("Access granted!");
      myservo.write(130); // stop the motor

  delay(100000); //
  // stay stopped

    } else {
      Serial.println("Access denied.");
     
    }
  }
}
