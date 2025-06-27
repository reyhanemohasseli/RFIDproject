#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Preferences.h>
#include <vector>
using namespace std;

#define SDA 5
#define RST 16
#define buzzer 33
#define LED 32
#define gate 2

MFRC522 rfid(SDA , RST); 
Preferences preferences;

vector<String> guys_uid;
String serialized = "";
String master_UID = "";
String new_UID = "";
String member_UID = "";
bool check_pushbutton = true;
bool check_exist = true;
bool member_check = true;


void saveVectorToNVS() {
  serialized = "";                                    // Reset the serialized string
  for (int i = 0; i < guys_uid.size(); i++) {
    serialized += guys_uid[i];
    if (i < guys_uid.size() - 1) {
      serialized += ",";                              // Add a comma separator
    }
  }
  preferences.putString("guys_uid", serialized);
  Serial.println("Vector saved to NVS: " + serialized);
}


void loadVectorFromNVS() {
  
  serialized = preferences.getString("guys_uid", "");
  if (serialized.length() == 0) {
    Serial.println("No data found in NVS.");
    return;
  }
 
  guys_uid.clear();                                 // Clear the vector before loading new data
  int start = 0;
  int end = serialized.indexOf(',');

  while (end != -1) {
    guys_uid.push_back(serialized.substring(start, end));
    start = end + 1;
    end = serialized.indexOf(',', start);
  }
  guys_uid.push_back(serialized.substring(start));   // Add the last element
  Serial.println("Vector loaded from NVS:");
  for (int i = 0; i < guys_uid.size(); i++) {
    Serial.println(guys_uid[i]);
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(4 , INPUT_PULLUP);
  pinMode(buzzer , OUTPUT);
  pinMode(LED , OUTPUT);
  pinMode(gate , OUTPUT);
  SPI.begin(18 , 19 , 23);
  rfid.PCD_Init();
  Serial.println("Place your RFID card/tag near the reader...");
  preferences.begin("rfid-storage", false);

  loadVectorFromNVS();
  if (guys_uid.size() == 0) {
    guys_uid.push_back("74a734a");
    saveVectorToNVS();
  }
}


void loop() {
  
  if(digitalRead(4) == LOW){
    check_pushbutton = false;
  }

  if(check_pushbutton == false){                              //button is pushed
    while(!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    }

    for (byte i = 0; i < rfid.uid.size; i++) {
      master_UID += String(rfid.uid.uidByte[i], HEX);         //get master card
    }
    rfid.PICC_HaltA();

    if(master_UID == ("74a734a" || "84787683" || "c9701b3f" || "amirhosein")){         //check master card
      digitalWrite(buzzer , HIGH);
      delay(100);
      digitalWrite(buzzer , LOW);

      while(!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {          //recive new card
      }
      for (byte i = 0; i < rfid.uid.size; i++) {
        new_UID += String(rfid.uid.uidByte[i], HEX);           //take new card uid
      }
      rfid.PICC_HaltA();
      

      if(new_UID != master_UID){
        
        for(int i = 0 ; i < guys_uid.size() ; i++){
          
          if(new_UID == guys_uid[i]){                           //check if not the saved one
            check_exist = false;
            // digitalWrite(buzzer , HIGH);
            // delay(200);
            // digitalWrite(buzzer , LOW);
            // Serial.println("no");
            break;
          }
        }

        if(check_exist == true){
            Serial.println("saved!");                           //save new card uid whatever not repeated

            guys_uid.push_back(new_UID);

            digitalWrite(LED , HIGH);
            digitalWrite(buzzer , HIGH);
            delay(500);
            digitalWrite(LED , LOW);
            digitalWrite(buzzer , LOW);

            saveVectorToNVS();
        }
      }
    }
    // else{                                                     //can't recognize master card
    //   digitalWrite(buzzer , HIGH);
    //   delay(500);
    //   digitalWrite(buzzer , LOW);
    // }

    check_pushbutton = true;
    master_UID  = "";
    new_UID = "";
    check_exist = true;
  }
  
  else if(check_pushbutton == true){                              //the button not pushed
    if(!rfid.PICC_IsNewCardPresent()){
      return;
    }
    if(!rfid.PICC_ReadCardSerial()){
      return;
    }

    for (byte i = 0; i < rfid.uid.size; i++){
      member_UID += String(rfid.uid.uidByte[i], HEX);
    }
    rfid.PICC_HaltA();

    loadVectorFromNVS();

    for(int i = 0 ; i < guys_uid.size() ; i++){
      if(member_UID == guys_uid[i]){                              //recognize you and 
        digitalWrite(buzzer , HIGH);
        digitalWrite(LED , HIGH);
        delay(500);
        digitalWrite(buzzer , LOW);
        digitalWrite(LED , LOW);

        digitalWrite(gate , HIGH);                                //the door opens
        delay(2000);
        digitalWrite(gate , LOW);
        member_check = false;
      }
    }
    if(member_check == true){                                      //can't recognize the card(strange person)
      digitalWrite(buzzer , HIGH);
      digitalWrite(LED , HIGH);
      delay(100);
      digitalWrite(buzzer , LOW);
      digitalWrite(LED , LOW);
      delay(100);
      digitalWrite(buzzer , HIGH);
      digitalWrite(LED , HIGH);
      delay(100);
      digitalWrite(buzzer , LOW);
      digitalWrite(LED , LOW);
    }

    member_check = true;
    member_UID = "";
  }
}