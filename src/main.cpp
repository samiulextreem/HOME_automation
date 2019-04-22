
#include <Arduino.h>
#include <ESP8266WiFi.h>




const char* SSID_name = "goofy";
const char* passwd = "gooberxx";


WiFiServer wifiserver(80);

String LED5_stat = "off";
String LED4_stat = "off";


String web_elem;


void setup(){
    delay(100);
    Serial.begin(115200);
    WiFi.begin(SSID_name,passwd);
    delay(100);
    Serial.print("conecting to ");
    Serial.print(SSID_name);
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        if(WiFi.status() == WL_CONNECTED){
            Serial.println("connected "); 
            Serial.println(WiFi.localIP());
            break;
        }
        Serial.print(".");
    }
}


void loop(){
    WiFiClient client = wifiserver.available();
    if(client == true){
        Serial.println("client is connected");
    }

}




