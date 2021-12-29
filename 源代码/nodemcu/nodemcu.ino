
#define LED_BUILTIN 2 
#include <ESP8266WiFi.h>
#include <dht11.h> 
dht11 DHT11;
// 这里定义了DHT11 要连接 的引脚
#define DHT11_PIN 05
#define led_PIN 16

const char* ssid = "ABing";
const char* password = "123456789";

const char* host = "192.168.43.229";
const int port = 9000;


const char* id = "abing-Nodemcu";
int tick = 1000;
char dht_data[9];   //串口接收数据区



WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //连接WIFI
  WiFi.begin(ssid, password);
  //设置读取socket数据时等待时间为100ms（默认值为1000ms，会显得硬件响应很慢）
  client.setTimeout(100.);

  //等待WIFI连接成功
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connecting...");
    delay(500);
  }
  Serial.println("WiFi connected!.");
}

void loop() {
  
    int chk = DHT11.read(DHT11_PIN); 
  // put your main code here, to run repeatedly:
  if (client.connect(host, port))
  {
    Serial.println("host connected!");
    client.println("TCP client connected!");
    //发送第一条TCP数据，发送ID号
    client.print(id);
  }
  else
  {
    // TCP连接异常
    Serial.println("host connecting...");
    delay(500);
  }

  while (client.connected()) {
    //      接收到TCP数据
    if (client.available())
    {
      String line = client.readStringUntil('\n');
      if (line == "1") {
        Serial.println("command:open led.");
        digitalWrite(LED_BUILTIN,LOW);
        client.print("OK");
      }
      else if (line == "0") {
        Serial.println("command:close led.");
        digitalWrite(LED_BUILTIN, HIGH);
        client.print("OK");
      }
    }
    else {
      int i=0;        //串口接收数据计数用
      /*dht_data[0]='T';
      dht_data[1]=':';
      dht_data[2]='0'+DHT11.temperature/10;
      dht_data[3]='0'+DHT11.temperature%10;
      dht_data[4]=' ';
      dht_data[5]='H';
      dht_data[6]=':';
      dht_data[7]='0'+DHT11.humidity/10;
      dht_data[8]='0'+DHT11.humidity%10;*/
//      int chk = DHT11.read(DHT11_PIN);
      while (!Serial.available());
      while(Serial.available())
      {
         dht_data[i]=Serial.read();
        i++;
        }
      Serial.println(dht_data);
      delay(100);
      client.print(dht_data);
      delay(100);
    }
  }

}
