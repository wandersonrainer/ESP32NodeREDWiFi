//Programa ESP32 (WebServer) - testes com Node-RED
#include  <Arduino.h>
#include  <WiFi.h>
#include  <WebServer.h>

#define ssid  "REDE_LAB2"
#define senha "automacaoeletrica"

#define TENSAO  26
#define BOT     14
#define POT     33
#define LED     2

WebServer servidor(80); 

void  criarJSON(){
  int leituraPOT = analogRead(POT);
  int leituraBOT = digitalRead(BOT);

  String json = "{";
  json += "\"potenciometro\":" + String(leituraPOT) + ",";
  json += "\"botao\":\"" + String(leituraBOT) + "\"";
  json += "}";

  servidor.send(200, "application/json", json);
}

void  funcaoLED(){
  String dados = servidor.uri();
  if(dados=="/led/on")
    digitalWrite(LED,1);
  else if(dados=="/led/off")
    digitalWrite(LED,0);
  servidor.send(200,"text;plain","ok");
}

void  setup(){
  //config GPIO
  pinMode(BOT,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  pinMode(TENSAO,OUTPUT);
  digitalWrite(TENSAO,1);

  Serial.begin(9600);
  WiFi.begin(ssid,senha);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("ESP32 conectada a rede WiFi");
  Serial.println(WiFi.localIP());

  servidor.on("/led/on",funcaoLED);
  servidor.on("/led/off",funcaoLED);
  servidor.on("/status",criarJSON);
  servidor.begin();
}

void  loop(){
  servidor.handleClient();
}