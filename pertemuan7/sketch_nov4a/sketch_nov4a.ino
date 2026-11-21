#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 4
#define LEDMERAH 2
#define LEDKUNING 18
#define LEDHIJAU 19
#define POTPIN 34

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "wedew";
const char* password = "yaelahcuy";

const char* mqtt_server = "m6669c4d.ala.asia-southeast1.emqxsl.com";
const int mqtt_port = 8883;
const char* mqtt_username = "Kelompok_2_IoT";
const char* mqtt_password = "12345678";

const char* topic_tanah = "res/iot/tanah";
const char* device_id = "ESP32-TNH";

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastPublish = 0;
unsigned long lastMqttAttempt = 0;
unsigned long lastWifiAttempt = 0;

const unsigned long publishInterval = 5000;

String clientId;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);

  clientId = "ESP32-" + WiFi.macAddress();

  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setBufferSize(512);

  pinMode(LEDMERAH, OUTPUT);
  pinMode(LEDKUNING, OUTPUT);
  pinMode(LEDHIJAU, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (now - lastWifiAttempt >= 10000) {
      lastWifiAttempt = now;
      WiFi.reconnect();

      Serial.println("[WIFI] Menghubungkan ulang...");
    }

    delay(10);
    return;
  }

  if (!client.connected()) {
    if (now - lastMqttAttempt >= 5000) {
      lastMqttAttempt = now;

      if (client.connect(
            clientId.c_str(),
            mqtt_username,
            mqtt_password
          )) {
        Serial.println("[MQTT] Terhubung");
      } else {
        Serial.print("[MQTT] Gagal, state=");
        Serial.println(client.state());
      }
    }

    return;
  }

  client.loop();
  now = millis();

  if (now - lastPublish < publishInterval) {
    return;
  }

  lastPublish = now;

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  int nilaiADC = analogRead(POTPIN);
  int nilaiTanah = map(nilaiADC, 0, 4095, 0, 100);

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("[DHT] Pembacaan gagal");
    return;
  }

    if (nilaiTanah < 30) {
      Serial.println("Kering");
      digitalWrite(LEDMERAH, HIGH);
    } else if ((nilaiTanah >= 30) && (nilaiTanah <= 70)) {
      Serial.println("Cukup");
      digitalWrite(LEDKUNING, HIGH);
    } else if (nilaiTanah > 70) {
      Serial.println("Basah");
      digitalWrite(LEDHIJAU, HIGH);
    }


  JsonDocument doc;
  doc["suhu"] = suhu;
  doc["kelembapan"] = kelembapan;
  doc["tanah"] = nilaiTanah;
  doc["device"] = device_id;

  char payload[256];
  serializeJson(doc, payload, sizeof(payload));

  if (client.publish(topic_tanah, payload, false)) {
    Serial.print("[PUBLISH] ");
    Serial.println(payload);
  } else {
    Serial.println("[PUBLISH] Gagal");
  }
}