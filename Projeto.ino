#include <WiFi.h>
#include <FirebaseESP32.h>
#include <NTPClient.h>
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define pinTemperatura 34
#define pinAzul 35
#define pinVerde 32
#define pinVermelho 33
#define pinBuzzer 25
FirebaseData firebaseData;
FirebaseJson json;
float temperatura=0;
String _data;
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
short int isOnline;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(pinVermelho,OUTPUT);
  pinMode(pinVerde,OUTPUT);
  pinMode(pinAzul,OUTPUT);
  pinMode(pinBuzzer,OUTPUT);
  pinMode(pinTemperatura,INPUT); 
  ntp.begin();
  ntp.forceUpdate();
}
void setLedBuzzer(){
  if (temperatura >=20) {
      digitalWrite(pinVermelho, 1);
      digitalWrite(pinVerde, 0);
      digitalWrite(pinAzul, 0);
      digitalWrite(pinBuzzer,1);
   }//Vermelho
  else if (temperatura >=15) {
    digitalWrite(pinVermelho, 1);
    digitalWrite(pinVerde, 1);
    digitalWrite(pinAzul, 0);
    digitalWrite(pinBuzzer,0);
   }//Amarelo
  else {
    digitalWrite(pinVermelho, 0);
    digitalWrite(pinVerde, 1);
    digitalWrite(pinAzul, 0);
    digitalWrite(pinBuzzer,0);
   }//Verde
};
void getTemperatura(){
    temperatura = analogRead(pinTemperatura) / 9.31;
}
void setTemperatura(){
    json.set("Temperatura",temperatura);
    Firebase.updateNode(firebaseData,"/ESP32/",json);
    _data = ntp.getFormattedDate ();
    json.set("Data",_data);
    Firebase.updateNode(firebaseData,"/ESP32/",json);
}
void checkOnline(){
    Firebase.getInt(firebaseData,"/ESP32/Online");
    isOnline = firebaseData.intData();
};
void ledOFF(){
    digitalWrite(pinVermelho,0);
    digitalWrite(pinVerde,0);
    digitalWrite(pinAzul,0);
    digitalWrite(pinBuzzer,0);
};
void loop(){
  checkOnline();
  if(isOnline){
    getTemperatura();
    setTemperatura();
    setLedBuzzer();
  }else{
    ledOFF();
  }
  delay(1000);
}
