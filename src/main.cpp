//==============================================\\
//     +----------------------------------+     \\
//     |             RFID pins            |     \\
//     +----------------------------------+     \\
//     |   RFID   |  COLOR WIRE |   ESP   |     \\
//     |----------------------------------|     \\
//     |   SDA    |   BLUE      |   D4    |     \\
//     |----------------------------------|     \\
//     |   RST    |   PURPLE    |   D3    |     \\
//     |----------------------------------|     \\
//     |   SCK    |   GREEN     |   D5    |     \\
//     |----------------------------------|     \\
//     |   MISO   |   WHITE     |   D6    |     \\
//     |----------------------------------|     \\
//     |   MOSI   |   BROWN     |   D7    |     \\
//     +----------------------------------+     \\
//==============================================\\
//     +----------------------------------+     \\
//     |       MOTORS' CONTROL PINS       |     \\
//     +----------------------------------+     \\
//     | ######### |   LEFT   |   RIGHT   |     \\
//     |----------------------------------|     \\
//     |   CLOSE   |   D1  O  |   D0  P   |     \\
//     |----------------------------------|     \\
//     |   OPEN    |   D2  Y  |   D8  B   |     \\
//     +----------------------------------+     \\
//==============================================\\

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include "../lib/MFRC/MFRC522.h"
#include "web_interface.h"

#define RST_PIN                         D3
#define SDA_PIN                         D4

#define INTERVAL_BETWEEN_GATES          1000

#define RESET_PERIOD                    300000

#define TIME_OPEN_CLOSE_SINGLE_GATE     4000
#define TIME_WAIT_SINGLE_GATE           3000

#define TIME_OPEN_CLOSE_ALL_GATES_RFID  10000
#define TIME_WAIT_ALL_GATES_RFID        5000

#define CLOSE_L_PIN                     D1
#define OPEN_L_PIN                      D2

#define CLOSE_R_PIN                     D0
#define OPEN_R_PIN                      D8

const char *ssid =      "e-Gate Controller";
const char *password =  "ESP8266eGateController";

ESP8266WebServer server(80);
MFRC522 mfrc522(SDA_PIN, RST_PIN);

bool isOpeningL = LOW;
bool isClosingL = LOW;

bool isOpeningR = LOW;
bool isClosingR = LOW;

bool state = false;

const uint32_t uid_SINGLE_GATE_keys[] =     { 1251961520,742051740   };
const uint32_t uid_ALL_GATE_keys[] =        { 1421461590};

uint32_t timer = 0;

void scheduledReset(uint32_t period);
void response();
void OPEN_GATE();
void CLOSE_GATE();
void STOP_ALL_GATES();
void OPEN_CLOSE_SINGLE_GATE_RFID();
void OPEN_CLOSE_ALL_GATES_RFID();
void OPEN_CLOSE_SINGLE_GATE_WIFI();
void rfIdCheck();

//----------------------------------------------------------------------------------------------------------------------

void setup() {
    delay(1000);

    Serial.begin(9600);
    mfrc522.PCD_Init();

    pinMode(OPEN_R_PIN, OUTPUT);  digitalWrite(OPEN_R_PIN, isOpeningR);
    pinMode(CLOSE_R_PIN, OUTPUT); digitalWrite(CLOSE_R_PIN, isClosingR);

    pinMode(OPEN_L_PIN, OUTPUT);  digitalWrite(OPEN_L_PIN, isOpeningL);
    pinMode(CLOSE_L_PIN, OUTPUT); digitalWrite(CLOSE_L_PIN, isClosingL);

    Serial.begin(115200);
    Serial.println("\t\te-Gate Controller");

    WiFi.softAP(ssid, password);
    IPAddress apip = WiFi.softAPIP();

    server.on("/", response);
    server.on("/CloseAllGates", CLOSE_GATE);
    server.on("/OpenAllGates", OPEN_GATE);
    server.on("/StopAllGates", STOP_ALL_GATES);
    server.on("/OpenCloseSingleGate", OPEN_CLOSE_SINGLE_GATE_WIFI);
    server.begin();
}

void loop() {
    rfIdCheck();
    server.handleClient();
    scheduledReset(RESET_PERIOD);
}

// ---------------------------------------------------------------------------------------------------------------------

void scheduledReset(uint32_t period){

    if (millis() - timer > period) {
        Serial.println("Hard reset!");
        timer = millis();
        pinMode(RST_PIN, OUTPUT);
        delay(2);
        digitalWrite(RST_PIN, LOW);
        delay(2);
        digitalWrite(RST_PIN, HIGH);
        delay(2);
        mfrc522.PCD_Init();
        delay(2);
    }
}

void CLOSE_GATE() {
    delay(2);

    if (!isOpeningR && !isOpeningL) {
        isClosingL = !isClosingL;   isClosingR = !isClosingR;
        digitalWrite(CLOSE_L_PIN, isClosingL);
        delay(INTERVAL_BETWEEN_GATES);
        digitalWrite(CLOSE_R_PIN, isClosingR);
    }
    response();
    delay(2);
}

void OPEN_GATE() {
    delay(2);

    if (!isClosingR && !isClosingL) {
        isOpeningL = !isOpeningL;   isOpeningR = !isOpeningR;
        digitalWrite(OPEN_R_PIN, isOpeningL);
        delay(INTERVAL_BETWEEN_GATES);
        digitalWrite(OPEN_L_PIN, isOpeningR);
    }

    response();
    delay(2);
}

void STOP_ALL_GATES(){

    if (isClosingR || isClosingL){
        isClosingL = LOW;                               isClosingR = LOW;
        digitalWrite(CLOSE_L_PIN, isClosingL);  digitalWrite(CLOSE_R_PIN, isClosingR);
    }

    if (isOpeningR || isOpeningL){
        isOpeningL = LOW;                               isOpeningR = LOW;
        digitalWrite(OPEN_L_PIN, isOpeningL);   digitalWrite(OPEN_R_PIN, isOpeningR);
    }

    response();
}

void OPEN_SINGLE_GATE() {
    isClosingR = LOW;
    digitalWrite(CLOSE_R_PIN, isClosingR);

    if (!isOpeningR && !isClosingR && !isOpeningL && !isClosingL) {
        isOpeningR = !isOpeningR;
        digitalWrite(OPEN_R_PIN, isOpeningR);
    }
}

void CLOSE_SINGLE_GATE() {
    isOpeningR = LOW;
    digitalWrite(OPEN_R_PIN, isOpeningR);

    if (!isOpeningR && !isClosingR && !isOpeningL && !isClosingL) {
        isClosingR = !isClosingR;
        digitalWrite(CLOSE_R_PIN, isClosingR);
    }
}

void OPEN_CLOSE_SINGLE_GATE_WIFI(){

    if (!state){
        OPEN_SINGLE_GATE();
        state = true;
    } else {
        CLOSE_SINGLE_GATE();
        state = false;
    }

    response();
}

void OPEN_CLOSE_SINGLE_GATE_RFID(){
    isOpeningR = !isOpeningR;
    digitalWrite(OPEN_R_PIN, isOpeningR);

    delay(TIME_OPEN_CLOSE_SINGLE_GATE);

    isOpeningR = !isOpeningR;
    digitalWrite(OPEN_R_PIN, isOpeningR);

    delay(TIME_WAIT_SINGLE_GATE);

    isClosingR = !isClosingR;
    digitalWrite(CLOSE_R_PIN, isClosingR);

    delay(TIME_OPEN_CLOSE_SINGLE_GATE);

    isClosingR = !isClosingR;
    digitalWrite(CLOSE_R_PIN, isClosingR);
}

void OPEN_CLOSE_ALL_GATES_RFID(){
    OPEN_GATE();
    delay(TIME_OPEN_CLOSE_ALL_GATES_RFID);
    STOP_ALL_GATES();
    delay(TIME_WAIT_ALL_GATES_RFID);
    CLOSE_GATE();
    delay(TIME_OPEN_CLOSE_ALL_GATES_RFID);
    STOP_ALL_GATES();
}

void response(){
    server.send(200, "text/html", HTML);
}

void rfIdCheck() {

    if (isClosingR || isClosingL || isOpeningR || isOpeningL) {
        return;
    }

    byte found = 0;
    String rfId;
    uint32_t rUid;

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

        rfId = String(mfrc522.uid.uidByte[1]) +
               String(mfrc522.uid.uidByte[2]) +
               String(mfrc522.uid.uidByte[3]) +
               String(mfrc522.uid.uidByte[4]);
        rUid = rfId.toInt();

        for (uint32_t key: uid_SINGLE_GATE_keys) {
            if (rUid == key) {
                delay(2);
                found = 1;
                delay(2);
                break;
            }
        }

        for (uint32_t key: uid_ALL_GATE_keys) {
            if (rUid == key) {
                delay(2);
                found = 2;
                delay(2);
                break;
            }
        }

        switch (found) {
            case 0:
                Serial.println("Card UID: \t" + rfId + "\t\t->" + "\tAccess Denied");
                break;
            case 1:
                Serial.println("Card UID: \t" + rfId + "\t\t->" + "\tAccess Granted");
                OPEN_CLOSE_SINGLE_GATE_RFID();
                break;
            case 2:
                Serial.println("Card UID: \t" + rfId + "\t\t->" + "\tAccess Granted");
                OPEN_CLOSE_ALL_GATES_RFID();
                break;
            default:
                Serial.println("ERROR!");
                STOP_ALL_GATES();
        }
    }
}