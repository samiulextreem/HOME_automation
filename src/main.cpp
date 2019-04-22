#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* SSID = "goofy";
const char* passwd = "gooberxx";

String header;

WiFiServer Server(80);


String LED5_status = "off";
String LED4_status = "off";

const int output5 = 5;
const int output4 = 4;



void setup() {
	pinMode(output5,OUTPUT);
	pinMode(output4,OUTPUT);

	digitalWrite(output5,LOW);
	digitalWrite(output4,LOW);

	Serial.begin(115200);
	Serial.print("connecting to ");
	Serial.print(SSID);
	WiFi.begin(SSID,passwd);
	delay(100);
	while(WiFi.status() != WL_CONNECTED){
		delay(1000);
		Serial.print('.');
	}
	Serial.println("connected to goofy");
	Serial.println(WiFi.localIP());
	Server.begin();
}


void loop(){
	
}

void loop() {
	WiFiClient client = Server.available();
	if (client){
		Serial.println("new client connected");
		String currentline = "";
		while(client.connected()){
		if(client.available()){
			char c = client.read();
			Serial.write(c);
			header = header +c;
			if(c == '\n'){
			if(currentline.length() == 0){
				client.println("HTTP/1.1 200 ok");
				client.println("content-type:text/html");
				client.println("connection: close");
				client.println();

				if(header.indexOf("GET /5/on") >= 0){
				Serial.println("GPIO 5 on");
				LED5_status = "on";
				digitalWrite(output5,HIGH);
				}
				else if(header.indexOf("GET /5/off") >=0){
				Serial.println("GPIO 5 off");
				LED5_status = "off";
				digitalWrite(output5,LOW);
				}
				else if(header.indexOf("GET /4/on") >=0){
				Serial.println("GPIO 4 on");
				LED4_status = "on";
				digitalWrite(output4,HIGH);
				}
				else if(header.indexOf("GET /4/off") >= 0){
				Serial.println("GPIO 4 off");
				LED4_status = "off";
				digitalWrite(output4,LOW);
				}

				client.println("<!DOCTYPE html><html>");
				client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
				client.println("<link rel=\"icon\" href=\"data:,\">");
				client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
				client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
				client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
				client.println(".button2 {background-color: #77878A;}</style></head>");
				client.println("<body><h1>ESP8266 Web Server</h1>");
				client.println("<p>GPIO 5 - State " + LED5_status + "</p>");
				if (LED5_status == "off") {
				client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
				} 
				else {
				client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
				} 
				client.println("<p>GPIO 4 - State " + LED4_status + "</p>");
				if (LED4_status=="off") {
				client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
				} else {
				client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
				}

				client.println("</body></html>");

				client.println();
				break;
			}
			else{
				currentline = "";
			}
			}
			else if( c!= '\r'){
			currentline = currentline + c;
			}
		}

		}

		header = "";
		client.stop();
		Serial.println("client disconnected");
		Serial.print("");


	}
}


