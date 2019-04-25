
//wifi acess point


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string.h>
#include <iostream>



#define Home_appliance_1 D1
#define Home_appliance_2 D2







const char* SSID_name = "goofy";
const char* passwd = "gooberxx";




WiFiServer webserver(80);

int Home_appliance_1_state = 0;        //D1 pin
int Home_applience_2_state = 0;        //D2 pin

// magic functions
String prepareHtmlPage();
int check_home_appliance_status(char home_appliance_num);
void switch_appliance(char home_appliance_num);














void setup(){
    //serial communication
    delay(100);
    Serial.begin(115200);
    //pinmode decleration
    pinMode(Home_appliance_1,1);
    pinMode(Home_appliance_2,1);              
    digitalWrite(D1,0);
    digitalWrite(D2,0);


    //wifi and stuff
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID_name,passwd);
    delay(100);
    IPAddress myIP = WiFi.softAPIP();
    // Serial.printf("HotSpt IP :%s",myIP.toString().c_str());
    // Serial.println(".");
    webserver.begin(); 
    // Serial.println("http server begin..");
    
}


void loop(){
  WiFiClient client = webserver.available();
  while(client.connected()){
    if(client.available()){ 
      String client_rq = client.readStringUntil('\r');
      char const* req= client_rq.c_str();
      Serial.printf(req);

      if(client_rq.indexOf("/4/off") >= 0 && client_rq.indexOf("\n") >= 0){
        int current_status = check_home_appliance_status(Home_appliance_1);
        if(current_status == 1){
          switch_appliance(Home_appliance_1);
        }else{
          continue;
        }
      }
      
      if(client_rq.indexOf("/4/on") >= 0 && client_rq.indexOf("\n") >= 0){
        int current_status = check_home_appliance_status(Home_appliance_1);
        if(current_status == 0){
          switch_appliance(Home_appliance_1);
        }else{
          continue;
        }
      }
      if(client_rq.length() == 1 && client_rq[0] == '\n'){
        // Serial.println("client connected");
        client.println(prepareHtmlPage());
        break;
      }
    }
  }
  delay(1000);
  client.stop();
}




String prepareHtmlPage(){
  Home_appliance_1_state = check_home_appliance_status(Home_appliance_1);
  String htmlPage ="HTTP/1.1 200 OK\r\n";
  htmlPage +="Content-Type: text/html\r\n";
  htmlPage +="Connection: close\r\n";
  htmlPage +="\r\n" ;
  htmlPage +="<!DOCTYPE HTML><head><meta name='viewport' content='width=device-width, initial-scale=1'></head>";
  htmlPage +="<html>" ;
  htmlPage +="<p>LIVE PREVIEW-------------</p><p></p><p></p><p></p>";
  htmlPage +="<p style='font-family:garamond;font-size:14px;font-style:italic;'>";
  htmlPage += "home_appliance-1&nbsp;&nbsp;&nbsp;&nbsp; ";
  if(Home_appliance_1_state == 1){
    htmlPage += "<i id='home_ap_1'>on!</i>&nbsp;&nbsp;&nbsp;&nbsp;";
  }else if(Home_appliance_1_state == 0){
    htmlPage += "<i id='home_ap_1'>off</i>&nbsp;&nbsp;&nbsp;&nbsp;";
  }
  htmlPage += "total used power-- &nbsp;&nbsp;&nbsp;&nbsp;total run time---&nbsp;&nbsp;&nbsp;&nbsp;<a id='button_1'><button  onclick='myFunction_1()'>switch</button></a>";
  htmlPage +="<script> function myFunction_1() {alert('switching -1',);var button_1 = document.getElementById('home_ap_1');if (button_1.innerHTML === 'off') {button_1.innerHTML = 'on!';} else {button_1.innerHTML = 'off';}}</script>";
  htmlPage +="</body></html>";
  htmlPage += "\r\n";
  return htmlPage;
}




int check_home_appliance_status(char home_appliance_num){
  return digitalRead(home_appliance_num);
}

void switch_appliance(char home_appliance_num){
  digitalWrite(home_appliance_num,!check_home_appliance_status(home_appliance_num));
}