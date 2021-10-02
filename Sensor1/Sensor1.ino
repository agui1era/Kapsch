#include <esp_task_wdt.h>
//20 seconds WDT
#define WDT_TIMEOUT 120
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define QoS 1

#define pin4 14
#define pin3 25
#define pin2 26
#define pin1 27

//Serial para conectar con arduino
#define RXD2 16
#define TXD2 17

//Definición red wifi
const char* ssid = "WWW_IGROMI_COM";
const char* password = "wifiiGromi12";

//Definicion MQTT
const char* mqtt_server = "iot.igromi.com";
const char* mqtt_id = "Kapsch1";
const char* mqtt_user = "Kapsch";
const char* mqtt_pass = "imagina12";
const char* topic="v1/devices/me/telemetry";

//Variables
WiFiClient espClient;
PubSubClient client(espClient);
String str;
char payload[100];
int pinState;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
}

//Conexión wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP : ");
  Serial.println(WiFi.localIP());
  
};

//Reconección MQTT
void reconnect() 
{
  // Loop de reconexion MQTT
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    // Intentado conectar
    if (client.connect(mqtt_id,mqtt_user,mqtt_pass)) {
      Serial.println("conectado al server");
      
    } else {
      Serial.print("RC-");
      Serial.println(client.state());
      Serial.println("Reintentando....");
      delay(500);
        
  }
 }
};

//Rutina para el envio de datos por MQTT
void EnvioMQTT(float Data,String ID) {
      //Se genera estructura de thingsboard
      str= "{\""+ID+"\":\""+String(Data)+"\"}";
      str.toCharArray(payload,100);
      Serial.println(payload);
      client.publish(topic,payload);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  };

    pinState=digitalRead(pin4);
 
    if (pinState == HIGH) {
      EnvioMQTT(1,"estado4"); 
    } else {
      EnvioMQTT(0,"estado4");
    };

    pinState=digitalRead(pin3);
 
    if (pinState == HIGH) {
      EnvioMQTT(1,"estado3"); 
    } else {
      EnvioMQTT(0,"estado3");
    };

    pinState=digitalRead(pin2);
 
    if (pinState == HIGH) {
      EnvioMQTT(1,"estado2"); 
    } else {
      EnvioMQTT(0,"estado2");
    };

    pinState=digitalRead(pin1);
 
    if (pinState == HIGH) {
      EnvioMQTT(1,"estado1"); 
    } else {
      EnvioMQTT(0,"estado1");
    };
 
  delay(60000);
  esp_task_wdt_reset();
 }
