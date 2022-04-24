//NodeMCU 1.0 programing board
/* Create a WiFi access point and provide a web server on it. */
//program wykorzystywany do czolgu LEGO na pokazy 
//LEGO Tank program used on shows

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>

//#define SERVER //mozlwe do skomentowania

#ifndef APSSID
#define APSSID "Orzel-7"
#define APPSK  "RzIT@2o20"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

SoftwareSerial esp01(D0,D1); // RX/TX (D0,D1)
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

String motor;
String pwm_;
String time1;
String frame;
int length1;

void handleRoot() 
{
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void handleNotFound();

void setup() {
  delay(1000);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);
  
  Serial.begin(9600);
  esp01.begin(9600);
  
  //AP connecting
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
//  server.on("/", handleRoot);
//  server.begin();
//  Serial.println("HTTP server started");

#ifdef SERVER
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
#endif

}
void loop() 
{
  #ifdef SERVER
  server.handleClient();
  MDNS.update();
  #endif
}

void handleNotFound() {
  
  digitalWrite(BUILTIN_LED, 1);
  
  //dzielenie tekstu
 String _uri = server.uri();
  _uri.remove(0,1);
  int ind1 = _uri.indexOf('/');
  motor=_uri.substring(0,ind1);
  int ind2 = _uri.indexOf('/', ind1+1 );
  pwm_ = _uri.substring(ind1+1, ind2);
  time1 = _uri.substring(ind2+1);
  Serial.println(motor + " " + pwm_ + " " + time1);

  if (motor == "go" || motor == "forward" || motor == "przód")
    frame = "#9_mGo_" + pwm_ + "_" + time1 + "\r\n";
  else if ((motor == "tył") || (motor == "back" ))
    frame = "#9_mGo_-" + pwm_ + "_" + time1 + "\r\n"; //-pwm
  else if (motor == "lewo" || motor == "left" )
    frame = "#1_mGo_" + pwm_ + "_" + time1 + "\r\n";
  else if (motor == "right" || motor == "prawo" )
    frame = "#2_mGo_" + pwm_ + "_" + time1 + "\r\n";
  else if (motor == "stop" || motor == "halt" )
    frame = "#9_mGo_0_1000\r\n"; //stop
  else if (motor == "wieża lewy" || motor == "obrót lewy" || motor == "obrotl" || motor =="rotL")
    frame = "#5_mGo_" + pwm_ + "_" + time1 + "\r\n";
  else if (motor == "wieża prawy" || motor == "obrót prawy" || motor == "obrotp"|| motor =="rotR" )
    frame = "#5_mGo_-" + pwm_ + "_" + time1 + "\r\n";
  else if (motor == "wieża góra" || motor == "góra" || motor == "wiezag" || motor =="turretUp" )
    frame = "#6_mGo_" + pwm_ + "_" + time1 + "\r\n";
  else if (motor == "wieża dół" || motor == "dół" || motor == "wiezad" || motor =="turretDown" )
    frame = "#6_mGo_-" + pwm_ + "_" + time1 + "\r\n";
    
  Serial.println(server.uri());
  Serial.println(frame);
  esp01.print(frame); //wysylanie do stm ramki uart
  server.send(200, "OKK", "OK"); //test
}
