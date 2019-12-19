// ESP8266 WIFI Manager iQQ-V5
//#include <SoftwareSerial.h>

#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <SocketIOClient.h>  //https://github.com/ngohuynhngockhanh/Socket.io-v1.x-Library
#include <ArduinoJson.h>  //https://github.com/bblanchon/ArduinoJson
#include "DHT.h" 
//#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP udp;
NTPClient ntp(udp, "2.asia.pool.ntp.org", 3600 * 7);
String rTime;
String sTime ="1126";

#define DHTPIN 10     // what digital pin we're connected to
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temp;
float humi;

SocketIOClient client;
ESP8266WebServer webServer(80);
//SoftwareSerial mySerial(3, 1, false, 256);

//char host[] = "192.168.1.6";  //IP address of socket.
char host[] = "testiqq.herokuapp.com";  //Web host.
int port = 80;                  //Port of server
char nsp_esp8266[] = "esp8266";   //ESP namespace as same as Web js!

// RID: Event
extern String RID;
// Rfull: JSON string - msg payload
extern String Rfull;

byte output[] = {14, 12, 13, 15, 2};

unsigned long previousMillis = 0;
long interval = 50000;

WiFiManager wifiManager;

void setup() {
  Serial.begin(115200);
  //mySerial.begin(57600);

  for (int i = 0; i < sizeof(output); i++) {
    pinMode(output[i], OUTPUT);
    digitalWrite(output[i],HIGH);
  }

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "iQQSmartHome"
  wifiManager.autoConnect("iQQSmartHome");
  Serial.println("Connected.");
  if (!client.connect(host, port, nsp_esp8266)) {
    Serial.println(F("Ket noi den socket server that bai!"));
    return;
  }
  
  getDeviceStatus();
  delay(100);
  dht.begin();
  //ntp.begin();
}

void loop() {
  if (client.monitor()) {
    Serial.print(RID);
    Serial.println(Rfull);
    setDevice();
  }

  temp = dht.readTemperature();
  humi = dht.readHumidity();

  //if (mySerial.available()) {
  //Serial.println(mySerial.read());
  //}

 if (millis() - previousMillis > interval) {
   previousMillis = millis();
   client.reconnect(host, port, nsp_esp8266);
   Serial.println("Reconnect server");
   getTemHum();
 }
    //Kết nối lại!
 if (!client.connected()) {
    client.reconnect(host, port, nsp_esp8266);
    Serial.println("Disconnect server");
 }

  ntp.update();
  //delay(1000);
  //Serial.println(ntp.getFormattedTime());
  //Serial.println(ntp.getDay());
  rTime = String(ntp.getHours()) + String(ntp.getMinutes());
  
  if (rTime == sTime){
    Serial.println(rTime);
    digitalWrite(output[1],LOW);
    getDeviceStatus();
  }
  //Serial.println(ntp.getMinutes());
}
void setDevice() {
  if (RID == "THIETBI") {
    getDeviceStatus();
  }
  else if (RID=="WEATHER"){
    getTemHum();
  }
  else if (RID.length() > 7) {
    RID.remove(0, 7); // Cắt chuỗi RID nhận được để lấy số thứ tự thiết bị
    Serial.println(RID);
    digitalWrite(output[RID.toInt()], Rfull.toInt());
    getDeviceStatus();
  }
}
void getDeviceStatus() {
  StaticJsonDocument<256> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  //đọc giá trị cảm biến rồi in ra root
  for (int i = 0; i < sizeof(output); i++) {
    String tb = "THIETBI";
    tb = tb += i;
    root[tb] = digitalRead(output[i]) ;
  }
  String jsonStr;
  serializeJson(root, jsonStr);
  client.send("THIETBI", jsonStr);
  RID = "";
  //Serial.println(jsonStr);
}
void getTemHum(){
  if (isnan(temp) || isnan(humi)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }
  else{
      StaticJsonDocument<256> jsonBuffer;
      JsonObject root = jsonBuffer.to<JsonObject>();
      root["TEMP"] = temp;
      root["HUMI"] = humi;
      String jsonStr;
      serializeJson(root, jsonStr);
      client.send("WEATHER", jsonStr);
      Serial.println(jsonStr);
  }
}
