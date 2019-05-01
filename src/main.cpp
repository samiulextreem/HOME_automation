
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "goofy";  // Enter SSID here
const char* password = "gooberxx";  //Enter Password here

ESP8266WebServer server(80);

void handle_OnConnect();
void handle_appliance0_on();
void handle_appliance0_off();
void handle_appliance1_on();
void handle_appliance1_off();
void handle_appliance2_on();
void handle_appliance2_off();
void handle_appliance3_on();
void handle_appliance3_off();
void handle_NotFound();
String SendHTML(uint8_t appliance_0_stat,uint8_t appliance_1_stat,uint8_t appliance_2_stat,uint8_t appliance_3_stat,float home_appliance_0_totalruntime_inmin,  float home_appliance_1_totalruntime_inmin,  float home_appliance_2_totalruntime_inmin,float home_appliance_3_totalruntime_inmin,float home_appliance_0_watthour,float home_appliance_1_watthour,float home_appliance_2_watthour,float home_appliance_3_watthour);
uint16 cpu_sclock_count = 0;







uint8_t home_appliance_0 = D1;
bool home_appliance_0_status = LOW;
float home_appliance_0_totalruntime_insec = 0;
float home_appliance_0_totalruntime_inmin = 0;
float home_appliance_0_watt = 100;
float home_appliance_0_watthour = 0;

uint8_t home_appliance_1 = D2;
bool home_appliance_1_status = LOW;
float home_appliance_1_totalruntime_insec = 0;
float home_appliance_1_totalruntime_inmin = 0;
float home_appliance_1_watt = 100;
float home_appliance_1_watthour = 0;

uint8_t home_appliance_2 = D5;
bool home_appliance_2_status = LOW;
float home_appliance_2_totalruntime_insec = 0;
float home_appliance_2_totalruntime_inmin = 0;
float home_appliance_2_watt = 400;
float home_appliance_2_watthour = 0;


uint8_t home_appliance_3 = D6;
bool home_appliance_3_status = LOW;
float home_appliance_3_totalruntime_insec = 0;
float home_appliance_3_totalruntime_inmin = 0;
float home_appliance_3_watt = 400;
float home_appliance_3_watthour = 0;









void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(home_appliance_0, OUTPUT);
  pinMode(home_appliance_1, OUTPUT);
  pinMode(home_appliance_2, OUTPUT);
  pinMode(home_appliance_3, OUTPUT);
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led0on", handle_appliance0_on);
  server.on("/led0off",handle_appliance0_off);
  server.on("/led1on", handle_appliance1_on);
  server.on("/led1off",handle_appliance1_off);
  server.on("/led2on", handle_appliance2_on);
  server.on("/led2off", handle_appliance2_off);
  server.on("/led3on", handle_appliance3_on);
  server.on("/led3off", handle_appliance3_off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void loop() {
  server.handleClient();
  if(home_appliance_0_status){
    digitalWrite(home_appliance_0, HIGH);
    home_appliance_0_totalruntime_insec = home_appliance_0_totalruntime_insec + .1;
    home_appliance_0_totalruntime_inmin = home_appliance_0_totalruntime_insec / 60;
    home_appliance_0_watthour = home_appliance_0_watt * home_appliance_0_totalruntime_inmin/60;
    Serial.printf("time in hour %f watthour %f\n",home_appliance_0_totalruntime_inmin,home_appliance_0_watthour);

  }
  else{
    digitalWrite(home_appliance_0, LOW);
  }
  if(home_appliance_1_status){
    digitalWrite(home_appliance_1, HIGH);
    home_appliance_1_totalruntime_insec = home_appliance_1_totalruntime_insec + .1;
    home_appliance_1_totalruntime_inmin = home_appliance_1_totalruntime_insec / 60;
    home_appliance_1_watthour = home_appliance_1_watt * home_appliance_1_totalruntime_inmin/60;
    Serial.printf("time in hour %f watthour %f\n",home_appliance_1_totalruntime_inmin,home_appliance_1_watthour);

  }
  else{
    digitalWrite(home_appliance_1, LOW);
  }
  if(home_appliance_2_status){
    digitalWrite(home_appliance_2, HIGH);
    home_appliance_2_totalruntime_insec = home_appliance_2_totalruntime_insec + .1;
    home_appliance_2_totalruntime_inmin = home_appliance_2_totalruntime_insec/60;
    home_appliance_2_watthour = home_appliance_2_watt * home_appliance_2_totalruntime_inmin/60;
    Serial.printf("time in hour %f watthour %f\n",home_appliance_2_totalruntime_inmin,home_appliance_2_watthour);
  }
  else{digitalWrite(home_appliance_2, LOW);
  }
  if(home_appliance_3_status){
    digitalWrite(home_appliance_3, HIGH);
    home_appliance_3_totalruntime_insec = home_appliance_3_totalruntime_insec + .1;
    home_appliance_3_totalruntime_inmin = home_appliance_3_totalruntime_insec/60;
    home_appliance_3_watthour = home_appliance_3_watt * home_appliance_3_totalruntime_inmin/60;
    Serial.printf("time in hour %f watthour %f\n",home_appliance_3_totalruntime_inmin,home_appliance_3_watthour);
  }
  else{digitalWrite(home_appliance_3, LOW);
  }
  delay(100);
}

void handle_OnConnect() {
  home_appliance_1_status = LOW;
  home_appliance_2_status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,home_appliance_1_status,home_appliance_2_status,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}


void handle_appliance0_on() {
  home_appliance_0_status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,home_appliance_1_status,home_appliance_2_status,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}

void handle_appliance0_off() {
  home_appliance_0_status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,home_appliance_1_status,home_appliance_2_status,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}







void handle_appliance1_on() {
  home_appliance_1_status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,true,home_appliance_2_status,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}

void handle_appliance1_off() {
  home_appliance_1_status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,false,home_appliance_2_status,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}

void handle_appliance2_on() {
  home_appliance_2_status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,home_appliance_1_status,true,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}

void handle_appliance2_off() {
  home_appliance_2_status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,home_appliance_1_status,false,home_appliance_3_status,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}


void handle_appliance3_on() {
  home_appliance_3_status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,home_appliance_1_status,home_appliance_2_status,true,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}

void handle_appliance3_off() {
  home_appliance_3_status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(home_appliance_0_status,home_appliance_1_status,home_appliance_2_status,false,home_appliance_0_totalruntime_inmin,home_appliance_1_totalruntime_inmin,home_appliance_2_totalruntime_inmin,home_appliance_3_totalruntime_inmin,home_appliance_0_watthour,home_appliance_1_watthour,home_appliance_2_watthour,home_appliance_3_watthour)); 
}







void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t appliance_0_stat,uint8_t appliance_1_stat,uint8_t appliance_2_stat,uint8_t appliance_3_stat,float home_appliance_0_totalruntime_inmin,  float home_appliance_1_totalruntime_inmin,  float home_appliance_2_totalruntime_inmin,float home_appliance_3_totalruntime_inmin,float home_appliance_0_watthour,float home_appliance_1_watthour,float home_appliance_2_watthour,float home_appliance_3_watthour){
  String html = "<!DOCTYPE html> <html>";
  html +="<head><meta name=\"viewport\" http-equiv='refresh' content='2'>";
  html +="<title>LED Control</title>";
  html +="<style>html { font-family: Comic Sans MS; display: inline-block; margin: auto; text-align:left;}";
  html +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}";
  html +=".button {width: 30px;background-color: #1abc9c;border: none;color: white;cursor: pointer;border-radius: 4px;}";
  html +=".button-on {background-color: #1abc9c;}";
  html +=".button-on:active {background-color: #16a085;}";
  html +=".button-off{background-color: #34495e;}";
  html +=".button-off:active {background-color: #2c3e50;}";
  html +="p {font-size: 15px;color: #888;margin-bottom: 10px;}";
  html +="</style>";
  html +="</head>";
  html +="<body>";
  html +="<h1>ESP8266 Web Server</h1>";
  html +="<h3>LIVE PREVIEW........</h3>";
  if(appliance_0_stat){
    html +="<p>home appliance-0:Status: ON&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_0_watthour;
    html += "watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_0_totalruntime_inmin ;    
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-off\" href=\"/led0off\">Off</a></p>";
  }else{
    html +="<p>home appliance-1:Status: OFF&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_1_watthour;
    html +="watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_1_totalruntime_inmin ;
    html += "min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-on\" href=\"/led0on\">ON</a></p>";
  }
  if(appliance_1_stat){
    html +="<p>home appliance-1:Status: ON&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_1_watthour;
    html += "watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_1_totalruntime_inmin ;    
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-off\" href=\"/led1off\">Off</a></p>";
  }else{
    html +="<p>home appliance-1:Status: OFF&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_1_watthour;
    html +="watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_1_totalruntime_inmin ;
    html += "min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-on\" href=\"/led1on\">ON</a></p>";
  }
  if(appliance_2_stat){
    html +="<p>home appliance-2:Status: ON&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_2_watthour; 
    html +="watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_2_totalruntime_inmin ;   
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-off\" href=\"/led2off\">Off</a></p>";
  }else{
    html +="<p>home appliance-2:Status: OFF&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_2_watthour; 
    html += "watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_2_totalruntime_inmin;
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-on\" href=\"/led2on\">ON</a></p>";
  }
  if(appliance_3_stat){
    html +="<p>home appliance-3:Status: ON&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_3_watthour; 
    html +="watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_3_totalruntime_inmin ;   
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-off\" href=\"/led3off\">Off</a></p>";
  }else{
    html +="<p>home appliance-3:Status: OFF&nbsp;&nbsp;&nbsp;&nbsp;total used power:";
    html += home_appliance_3_watthour; 
    html += "watt-hour&nbsp;&nbsp;&nbsp; total run time:";
    html += home_appliance_3_totalruntime_inmin;
    html +="min&nbsp;&nbsp;&nbsp;&nbsp;<a class=\"button button-on\" href=\"/led3on\">ON</a></p>";
  }
  html +="</body>";
  html +="</html>";
  return html;
}