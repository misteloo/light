//#include <SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define lamp D6

#include "Untitled_css.h"
#include "index.h"
#include "index_css.h"

String command;


SSD1306 display(0x3c,4,5);
ESP8266WebServer server(80);

const char* ssid("IO");
const char* pass("mobin1382");

const char* username("admin");
const char* www_password("admin");

void setup() {
  
  display.init();
  display.flipScreenVertically();
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.begin(ssid,pass);

  while(WiFi.status() != WL_CONNECTED)
  {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0,0,"Connection ... ");
    display.display();
  }

  pinMode(lamp,OUTPUT);
  
  digitalWrite(D6,0);

  String ip = WiFi.localIP().toString();
  display.clear();
  display.drawString(0,0,"IP address :");
  display.drawString(0,17, ip);
  display.display();
  server.begin();
  server.on("/" ,Handle_root);
  server.on("/LED" ,LED);
  server.on("/Untitled1.css" , untitledCss);
  server.on("/index.css" , indexCss);
  server.on("/authen" ,cheak);
  server.on("/voice" , voice_command);
  
}
void loop() {
 server.handleClient();


}

void Handle_root()
{
    if (!server.authenticate(username, www_password))
    return server.requestAuthentication();
  server.send(200,"text/html" , index_page);
}

void LED()
{ 
  if (server.arg("user")== username && server.arg("password")== www_password){}
  
  else
  {   
    if (!server.authenticate(username, www_password))
    return server.requestAuthentication();
  }
  if (server.arg("led") == "on")
  {
    digitalWrite(D6,1);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawString(52,20,"ON");
    display.display();
  }

  if (server.arg("led") == "off")
  {
    digitalWrite(D6,0);
    //digitalWrite(D6,1);
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawString(48,20,"OFF");
    display.display();
  }
  server.send(204,"");
}
void untitledCss()
{
  server.send(200 ,"text/css" ,untitledcss);
}
void indexCss()
{
    server.send(200 ,"text/css",indexcss);

}

void cheak()
{
  if (server.arg("user")== username && server.arg("password")== www_password)
  {
    server.send(200, "text/plain","Correct");
  }
  else 
  {
    server.send(200,"text/plain","Wrong Password");
  }
}
void voice_command()
{
  if (server.arg("user")== username && server.arg("password")== www_password){}
  command=server.arg("Command");
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(32,22,"Voice");
  display.drawString(48,38,command);

  if(command == "turnon" || command == ("????????"))
  {
    digitalWrite(D6,HIGH);
  }
  if(command == "turnoff" || command == ("??????????"))
  {
    digitalWrite(D6,LOW);
  }
  display.display();
  server.send(204,"");
}
