// ESP8266 WIFI Manager iQQ-V5
//#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <SocketIOClient.h>  //https://github.com/ngohuynhngockhanh/Socket.io-v1.x-Library
#include <ArduinoJson.h>  //https://github.com/bblanchon/ArduinoJson

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

byte output[] = {14, 12, 13, 15, 2};

unsigned long previousMillis = 0;
long interval = 50000;

void setup() {
  Serial.begin(115200);
  //mySerial.begin(57600);

  for (int i = 0; i < sizeof(output); i++) {
    pinMode(output[i], OUTPUT);
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
  delay(100);
  getDeviceStatus();
}

void loop() {
  if (client.monitor()) {
    Serial.print(RID);
    Serial.println(Rfull);
    setDevice();
  }

  //if (mySerial.available()) {
  //Serial.println(mySerial.read());
  //}

  if (millis() - previousMillis > interval) {
     previousMillis = millis();
     client.reconnect(host, port, nsp_esp8266);
     //Serial.println("Reconnect server");
  }
    //Kết nối lại!
  if (!client.connected()) {
     client.reconnect(host, port, nsp_esp8266);
     //Serial.println("Disconnect server");
  }
}
void setDevice() {  
  if (RID == "THIETBI") {
    getDeviceStatus();
  }
  else if (RID.length() > 7){
      RID.remove(0,7); // Cắt chuỗi RID nhận được để lấy số thứ tự thiết bị
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
      tb = tb +=i;
      root[tb] = digitalRead(output[i]) ;
    }
    String jsonStr;
    serializeJson(root, jsonStr);
    client.send("THIETBI", jsonStr);
    RID = "";
    Serial.println(jsonStr);
}
