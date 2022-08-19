//#include<HTTPClient.h>
//#include<WiFi.h>
//#include<ESP8266HTTPClient.h>

//#define led D2

//  bool checkpoint = true;

//  pinMode(led,OUTPUT);
//  digitalWrite(led,LOW);

//    if(checkpoint)
//    {
//      HTTPClient http;
//      Serial.println("[HTTP] begin...\n");
//      String message = "Pollution level has reached its Threshold!!! \n Get your vehicle Serviced. ";
//      http.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key= ... &message="+message);
//      Serial.print("[HTTP] GET...\n");
//
//      int httpCode=http.GET();
//
//      if(httpCode > 0)
//      {
//        Serial.printf("[HTTP] GET...code: %d\n",httpCode);
//        if(httpCode == HTTP_CODE_OK)
//        {
//          String payload = http.getString();
//          Serial.println(payload);
//          checkpoint=false;
//        }
//      }
//    }
//  }

//    ThingSpeak.writeField(myChannelNo,1,mq7_value,myWriteAPIKey);
//    ThingSpeak.writeField(myChannelNo,2,mq2_value,myWriteAPIKey);
//    ThingSpeak.writeField(myChannelNo,3,mq135_value,myWriteAPIKey);
//  

#include <ESP8266WiFi.h>
#include<ThingSpeak.h>
#include <SPI.h>
#include <Wire.h>

int mq135_value;
int mq2_value;
int mq7_value;

#define S0 D0
#define S1 D1
#define analogpin A0

WiFiClient client;

 unsigned long myChannelNo = 1578494; //Enter ThingSpeak Channel Number
  String myWriteAPIKey = "WQS5AOL8ZEYEVHHF"; //Enter Channel Write API Key
  const char* server = "api.thingspeak.com";
  String ssid = "Kiwi"; //Enter Wifi Name
  String pass = "9767760605"; //Enter Password
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  WiFi.begin(ssid,pass);
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected to " + ssid + " network");

  pinMode(analogpin,INPUT);
  pinMode(S0,OUTPUT);
  pinMode(S1,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  ThingSpeak.begin(client);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Taking value of MQ7 sensor
  digitalWrite(S0,LOW);
  digitalWrite(S1,LOW);
  delay(1000);
  mq7_value = analogRead(analogpin);//mq7
  Serial.print("CO_val: ");
  Serial.print(mq7_value);

  //Taking value of MQ2 sensor
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  delay(1000);
  mq2_value = analogRead(analogpin);//mq2
  Serial.print(",Smoke_val: ");
  Serial.print(mq2_value);

  //Taking value of MQ135 sensor
  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  delay(1000);
  mq135_value = analogRead(analogpin);//mq135
  Serial.print(",AQI_val: ");
  Serial.println(mq135_value);

  if(mq7_value>=300 || mq2_value>=400 || mq135_value>=300)
  {
    digitalWrite(LED_BUILTIN,LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }

  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = myWriteAPIKey;
    postStr += "&field1=";
    postStr += String(mq7_value);
    postStr += "&field2=";
    postStr += String(mq2_value);
    postStr += "&field3=";
    postStr += String(mq135_value);
    postStr += "r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + myWriteAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Data Send to Thingspeak");
  }
    client.stop();
    Serial.println("Waiting...");
 
  delay(12000);   
}
