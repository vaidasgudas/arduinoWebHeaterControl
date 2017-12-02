#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 200);
EthernetServer server(200);

String relay1State = "Off";
const int relay = 7;
float rellayTurnedOnTemp = 0;
int secondsAfterOn = 0;
char linebuf[80];
int charcount=0;

DHT dht(6, DHT11);

void setup() { 
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  dht.begin();
}

void dashboardPage(EthernetClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  
  client.println("<!DOCTYPE HTML><html><head><title>Heater Controll</title>");
  client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>");                                                             
  client.println("<h3><a href=\"/\">Refresh</a></h3>");
  client.println("<h4>Heater State: " + relay1State + "</h4>");

  float t = dht.readTemperature();
  client.println("<h5>Room Temperature: ");client.print(t);client.print("*C</h5>");

  if(rellayTurnedOnTemp > 0){
    client.println("<h5>Header turned on when temp was: ");client.print(rellayTurnedOnTemp);client.print("*C.<br/> Heater is running for ");
    if(secondsAfterOn < 60){
      client.print(secondsAfterOn);client.print(" sec</h5>");
    }else{
      int minutesAfterOn = secondsAfterOn / 60;
      int secondsAfterOn2 = secondsAfterOn % 60;
      client.print(minutesAfterOn);client.print(" min ");client.print(secondsAfterOn2);client.print(" sec</h5>");
    }
  }

  if(relay1State == "Off"){
    client.println("<form action='relay1on' method='get'>");
    client.println("<input type='password' name='pass'>");
    client.println("<input type='submit' value='ON'>");
    client.println("</form>");
  }
  else if(relay1State == "On"){
    client.println("<form action='relay1off' method='get'>");
    client.println("<input type='password' name='pass'>");
    client.println("<input type='submit' value='OFF'>");
    client.println("</form>");                                                                   
  }
  client.println("</body></html>"); 
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;

        if (c == '\n' && currentLineIsBlank) {
            dashboardPage(client);
            break;
        }
        
        if (c == '\n') {
          if (strstr(linebuf, "GET /relay1off") > 0 && strstr(linebuf, "<password here>") > 0){
            digitalWrite(relay, LOW);
            relay1State = "Off";
            rellayTurnedOnTemp = 0;
            secondsAfterOn = 0;
          }
          else if (strstr(linebuf, "GET /relay1on") > 0 && strstr(linebuf, "<password here>") > 0){
            digitalWrite(relay, HIGH);
            relay1State = "On";
            float t = dht.readTemperature();
            rellayTurnedOnTemp = t;
            secondsAfterOn = 0;
          }
                    
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(10);
    client.stop();
  }

  if(rellayTurnedOnTemp > 0){
    secondsAfterOn++;
    float t = dht.readTemperature();
    if(isnan(t) || t > rellayTurnedOnTemp){
        digitalWrite(relay, LOW);
        relay1State = "Off";
        rellayTurnedOnTemp = 0;
        secondsAfterOn = 0;
    }    
  }

  delay(990);
}
