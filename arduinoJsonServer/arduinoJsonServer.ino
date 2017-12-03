#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#include "Timer.h"

const String password = "pass";   // Set password here
const int relayPin = 7;           // Relay pin
const int sensorPin = 6;          // DHT temp sensor pin

DHT dht(sensorPin, DHT11);        // Can be used DHT11 or DHT22
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
byte ip[] = { 192, 168, 1, 200 }; // Set ip here
EthernetServer server(80);        // Set port here
Timer t;

String readString;
int temperature;
int humidity;
int turnOffAfterSecs = 0;
int turnOffOnTemp = 0;

uint8_t functionsIndex;
int (*functions[5])(String);
char * functionsNames[5];

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  dht.begin();
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  setFunction("turnOnLimitedTime", turnOnLimitedTime);
  setFunction("turnOnForDeltaT", turnOnForDeltaT);
  setFunction("turnOff", turnOff);

  updateSensorData();
  t.every(5000, runEvery5Sec);
}

void loop() {
  EthernetClient client = server.available();
  processClient(client);
  t.update();
}

void turnOnLimitedTime(String params){
  turnOffAfterSecs = params.toInt(); 
  digitalWrite(relayPin, HIGH);
}

void turnOnForDeltaT(String params){
  int deltaT = params.toInt();
  turnOffOnTemp = temperature + deltaT;  
  digitalWrite(relayPin, HIGH);
}

void turnOff(String param){
  digitalWrite(relayPin, LOW);
  turnOffAfterSecs = 0;
  turnOffOnTemp = 0;
}

void runEvery5Sec(){
  updateSensorData();
  
  if(turnOffAfterSecs > 0){
    turnOffAfterSecs = turnOffAfterSecs - 5;
    if(turnOffAfterSecs <= 0){
      turnOffAfterSecs = 0;
    }
  }
  
  if(turnOffOnTemp != 0 && temperature >= turnOffOnTemp){
    turnOffOnTemp = 0;
  }

  if(digitalRead(relayPin) == HIGH && turnOffAfterSecs == 0 && turnOffOnTemp == 0){
      digitalWrite(relayPin, LOW);
  }
}

int updateSensorData(){
    float fTemperature = dht.readTemperature();
    float fHumidity = dht.readHumidity();

    temperature = (int)(fTemperature * 100);
    humidity = (int)fHumidity;
}

void setFunction(char * functionName, int (*f)(String)){
    functionsNames[functionsIndex] = functionName;
    functions[functionsIndex] = f;
    functionsIndex++;
}

void processClient(EthernetClient& client){
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }

        if (c == '\n') {
          processGet(readString);
          if(readString.startsWith("GET /")){
            response(client);
          }
                  
          delay(1);
          client.stop();
          readString = "";
        }
      }
    }
  }
}

void processGet(String request){ // "GET /?functionName=parameter:pass HTTP/1.1"
  for (uint8_t i = 0; i < functionsIndex; i++){
    if(readString.startsWith("GET /?" + String(functionsNames[i]) + "=")) {
      int from = request.indexOf('=');
      int to = request.length();
      String param = request.substring(from + 1, to); // "parameter:pass HTTP/1.1
      to = param.indexOf(' ');
      param = param.substring(0, to); // "parameter:pass"
      if(param.length() > 0){
        from = param.indexOf(":");
        to = param.length();
        String requestPassword = param.substring(from + 1, to);
        if(requestPassword == password){
          param = param.substring(0, from);
          functions[i](param);
        }
      }
    }
  }
}

void response(EthernetClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println(); 
  client.println("{\"t\" : " + String(temperature) + ", \"h\" : " + String(humidity) + ", \"toas\" : " + turnOffAfterSecs + ", \"toot\" : " + turnOffOnTemp + "}");
}
