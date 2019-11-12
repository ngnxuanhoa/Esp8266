// ESP8266 WIFI Manager iQQ-V2
//#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <SocketIOClient.h>  //https://github.com/ngohuynhngockhanh/Socket.io-v1.x-Library
#include <ArduinoJson.h>  //https://github.com/bblanchon/ArduinoJson
#include <WiFiClient.h>  

WiFiManager wifiManager;
SocketIOClient client;
ESP8266WebServer webServer(80);
//SoftwareSerial mySerial(3, 1, false, 256);

//char host[] = "192.168.1.6";  //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
char host[] = "testiqq.herokuapp.com";  //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
int port = 80;                  //Cổng dịch vụ socket server do chúng ta tạo!
char nsp_esp8266[] = "esp8266";   //Thêm Arduino!
// RID: Tên hàm (tên sự kiện
// Rfull: Danh sách biến (được đóng gói lại là chuối JSON)
extern String RID;
extern String Rfull;

const int output1 = 14;
const int output2 = 12;
const int output3 = 13;
const int output4 = 15;
const int output5 = 2;

unsigned long previousMillis = 0;
long interval = 50000;

void setup() {
  Serial.begin(115200);
  //mySerial.begin(57600);
  
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);
  
  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  wifiManager.autoConnect("iQQSmartHome");
  Serial.println("Connected.");
  if (!client.connect(host, port, nsp_esp8266)) {
   Serial.println(F("Ket noi den socket server that bai!"));
   return;
 }
 delay(100);
 getDeviceStatus();
}

void loop(){
  if (client.monitor()) {
   setDevice();
  }

//if (mySerial.available()) {
   //Serial.println(mySerial.read());
//}
  
if (WiFi.status() == WL_CONNECTED) {
 if (millis() - previousMillis > interval) {
        //lệnh:
        previousMillis = millis();
        //Serial.println("Ket noi lai server");
        client.reconnect(host, port, nsp_esp8266);
        //Serial.println("Da san sang nhan lenh 2");
 }
    //Kết nối lại!
 if (!client.connected()) {
      client.reconnect(host, port, nsp_esp8266);
      //Serial.println("Da san sang nhan lenh 3");
 }
}
else{
  WiFiManager wifiManager;
  wifiManager.autoConnect("iQQSmartHome");
}
}
void setDevice(){
   Serial.println(RID);
   Serial.println(Rfull);
  if (RID =="THIETBI"){
        getDeviceStatus();
      }
  if(RID =="THIETBI0"){
        digitalWrite(output1, Rfull.toInt());
        getDeviceStatus();
      }
  if(RID =="THIETBI1"){
        digitalWrite(output2, Rfull.toInt());
        getDeviceStatus();
      }
  if(RID =="THIETBI2"){
        digitalWrite(output3, Rfull.toInt());
        getDeviceStatus();
      }
  if(RID =="THIETBI3"){
        digitalWrite(output4, Rfull.toInt());
        getDeviceStatus();
      }
  if(RID =="THIETBI4"){
        digitalWrite(output5, Rfull.toInt());
        getDeviceStatus();
      }
}
void getDeviceStatus(){    
    StaticJsonDocument<256> jsonBuffer;
    JsonObject root = jsonBuffer.to<JsonObject>();
    //đọc giá trị cảm biến rồi in ra root
    root["THIETBI0"] = digitalRead(output1) ;
    root["THIETBI1"] = digitalRead(output2) ;
    root["THIETBI2"] = digitalRead(output3) ;
    root["THIETBI3"] = digitalRead(output4) ;
    root["THIETBI4"] = digitalRead(output5) ;
    String jsonStr;
    serializeJson(root, jsonStr);
    client.send("THIETBI", jsonStr);
    RID = "";
    Serial.println(jsonStr);
}
