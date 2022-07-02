//==============================================\\
//     +----------------------------------+     \\
//     |       MOTORS' CONTROL PINS       |     \\
//     +----------------------------------+     \\
//     | ######### |   LEFT   |   RIGHT   |     \\
//     |----------------------------------|     \\
//     |   CLOSE   |   D3  O  |   D2  P   |     \\
//     |----------------------------------|     \\
//     |   OPEN    |   D1  Y  |   D0  B   |     \\
//     +----------------------------------+     \\
//==============================================\\

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "web_interface.h"

#define INTERVAL_BETWEEN_GATES          1000

#define OPEN_R_PIN                      D0
#define OPEN_L_PIN                      D1
#define CLOSE_R_PIN                     D2
#define CLOSE_L_PIN                     D3

const char *ssid =      "E-GateController";
const char *password =  "eGateController";

ESP8266WebServer server(80);

bool isOpeningL =        LOW;
bool isClosingL =        LOW;
bool isOpeningR =        LOW;
bool isClosingR =        LOW;
bool single_gate_state = LOW;

void response();
void open_gates();
void close_gates();
void stop_all_gates();
void open_close_single_gate();


void setup() {
    delay(1000);

    pinMode(OPEN_R_PIN, OUTPUT);  digitalWrite(OPEN_R_PIN, isOpeningR);
    pinMode(CLOSE_R_PIN, OUTPUT); digitalWrite(CLOSE_R_PIN, isClosingR);

    pinMode(OPEN_L_PIN, OUTPUT);  digitalWrite(OPEN_L_PIN, isOpeningL);
    pinMode(CLOSE_L_PIN, OUTPUT); digitalWrite(CLOSE_L_PIN, isClosingL);

    Serial.begin(115200);
    Serial.println("\t\te-Gate Controller");

    WiFi.softAP(ssid, password);
    IPAddress softAP = WiFi.softAPIP();

    server.on("/", response);
    server.on("/CloseAllGates", close_gates);
    server.on("/OpenAllGates", open_gates);
    server.on("/StopAllGates", stop_all_gates);
    server.on("/OpenCloseSingleGate", open_close_single_gate);
    server.begin();
}

void loop() {
    server.handleClient();
}


void response(){
    server.send(200, "text/html", HTML);
}

void close_gates() {
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

void open_gates() {
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

void stop_all_gates(){

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

void open_single_gate() {
    isClosingR = LOW;
    digitalWrite(CLOSE_R_PIN, isClosingR);

    if (!isOpeningR && !isClosingR && !isOpeningL && !isClosingL) {
        isOpeningR = !isOpeningR;
        digitalWrite(OPEN_R_PIN, isOpeningR);
    }
}

void close_single_gate() {
    isOpeningR = LOW;
    digitalWrite(OPEN_R_PIN, isOpeningR);

    if (!isOpeningR && !isClosingR && !isOpeningL && !isClosingL) {
        isClosingR = !isClosingR;
        digitalWrite(CLOSE_R_PIN, isClosingR);
    }
}

void open_close_single_gate(){

    if (!single_gate_state){
        open_single_gate();
        single_gate_state = true;
    } else {
        close_single_gate();
        single_gate_state = false;
    }

    response();
}