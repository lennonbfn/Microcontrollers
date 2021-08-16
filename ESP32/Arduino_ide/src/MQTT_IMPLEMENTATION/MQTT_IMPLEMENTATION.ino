#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid="BrandaoNascimento";
const char* pass="jesuseosenhor";
const char* brokerUser = "lennon";
const char* brokerPass = "960616";
const char* broker = "192.168.0.13";
const char* outTopic ="out/esp/task1";
const char* inTopic ="in/test";
float currentTime, lastTime;
 
WiFiClient espClient;
PubSubClient client(espClient);

int count = 0;
char messages[50];
 
void setupWifi(){

 
  WiFi.begin(ssid, pass);
 
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("-");
  }
 
  Serial.print("\n Conectado a rede");
  Serial.println(ssid);
}
 
void reconnect(){
  while(!client.connected()){
    Serial.print("\n Tentativa de conexão com broker ");
    Serial.println(broker);
    if(client.connect("esp 01", brokerUser, brokerPass))
    {
      Serial.print("\nConectado ao broker ");
      Serial.println(broker);
      client.subscribe(inTopic);
    } else {
      Serial.println("\n tentando reconectar");
      delay(5000);
    }
  }
}
 
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Nachricht empfangen: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
}
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    setupWifi();
    client.setServer(broker, 1883);
    client.setCallback(callback);
}


void loop() {
    // put your main code here, to run repeatedly:
    if (!client.connected()){
      reconnect();
    }
  client.loop();
 
 currentTime = millis();
  if(currentTime - lastTime > 2000){
    count++;
    snprintf(messages, 75, "Count: %ld", count);
    Serial.print("Sending Messages: ");
    Serial.println(messages);
    client.publish(outTopic, messages);
    lastTime = millis();
  }
}
