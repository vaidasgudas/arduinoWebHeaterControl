#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>
#include "DHT.h"
#include "Timer.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };
IPAddress ip(192,168,1,200);
EthernetServer server(80);
Timer t;

const int relayPin = 7;
const int sensorPin = 6;
const String password = "pass"; // Set password here

const String version = "1";

int temperature;
int humidity;
int turnOffAfterSecs = 0;
int turnOffOnTemp = 0;

aREST rest = aREST();
DHT dht(sensorPin, DHT11); // Can be used DHT11 or DHT22

void setup(void)
{
  Serial.begin(115200);
  Ethernet.begin(mac, ip);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  rest.set_name("arduinoHeater_" + version);
  rest.variable("relayPin", &relayPin);
  rest.variable("t", &temperature);
  rest.variable("h", &humidity);
  rest.variable("toas", &turnOffAfterSecs);
  rest.variable("toot", &turnOffOnTemp);
  
  rest.function("turnOnLimitedTime", turnOnLimitedTime);
  rest.function("turnOnForDeltaT", turnOnForDeltaT);
  rest.function("turnOff", turnOff);
  
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  dht.begin();
  updateSensorData();
  t.every(1000, runEverySecond);

  // Start watchdog
  wdt_enable(WDTO_4S);
}

void loop() {
  EthernetClient client = server.available();
  rest.handle(client);
  t.update();
    
  if(digitalRead(relayPin) == HIGH && turnOffAfterSecs == 0 && turnOffOnTemp == 0){
      digitalWrite(relayPin, LOW);
  }
  
  wdt_reset();
}

void runEverySecond(){
  updateSensorData();
  
  if(turnOffAfterSecs > 0){
    turnOffAfterSecs = turnOffAfterSecs - 1;
  }
  
  if(turnOffOnTemp != 0 && temperature >= turnOffOnTemp){
    turnOffOnTemp = 0;
  }
}

int updateSensorData(){
    float fTemperature = dht.readTemperature();
    float fHumidity = dht.readHumidity();

    temperature = (int)(fTemperature * 100);
    humidity = (int)fHumidity;
}

int turnOnLimitedTime(String params){
  String requestPassword = getValueFromParam(params, ':', 1);
  if(requestPassword != password){
    return 0;  
  }
  
  turnOffAfterSecs = getValueFromParam(params, ':', 0).toInt(); 
  digitalWrite(relayPin, HIGH);
  return 1;
}

int turnOnForDeltaT(String params){
  String requestPassword = getValueFromParam(params, ':', 1);
  if(requestPassword != password){
    return 0;  
  }
  
  int deltaT = getValueFromParam(params, ':', 0).toInt();
  turnOffOnTemp = temperature + deltaT;  
  digitalWrite(relayPin, HIGH);
  return 1;
}

int turnOff(String param){
  if(param != password){
    return 0;  
  }
  
  digitalWrite(relayPin, LOW);
  turnOffOnTemp = 0;
  turnOffAfterSecs = 0;
  return 1;
}

String getValueFromParam(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
